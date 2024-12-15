#include "ExecuteStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

ExecuteStage::ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe, HazardDetection* HDU, ForwardingUnit* FU, ZERO* ZU, Jump* JU)
	: clk(clock), IDEXEpipe(prev_pipe), EXEMEMpipe(next_pipe), HDU(HDU), FU(FU), ZU(ZU),JU(JU) {
	// Launch the decoding thread and store it in the class
	Executethread = std::thread([this]() { Executejob(); });
}


void ExecuteStage::Executejob() {

	//keep working 
	while (running) {
		ConsoleLog(3,"Executethread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(3,"Executethread starting new clock");

		//read data from critical section 
		IDEXEpipe->readdata(
		EXEdata.MemWriteEn, EXEdata.MemtoReg, //WB for the next pipe
		EXEdata.RegWriteEn,EXEdata.MemReadEn, //MEM
            //Check wheather the branch signal is appropiatly sent to the pipe.....
        EXEdata.ALUOp, EXEdata.RegDst, EXEdata.FC,EXEdata.FD,EXEdata.JrSignal,EXEdata.Branch,EXEdata.ZeroSignal, //exe
        PC,
        MC, //for display
        predictionE,
		EXEdata.readdata1,EXEdata.readdata2,//exe
		EXEdata.immediate,//exe
		EXEdata.rs,EXEdata.rt,EXEdata.rd//exe
		);
        ZU->ZeroInput(EXEdata.readdata1, EXEdata.readdata2, EXEdata.ZeroSignal);


        /////////////////////////////////////////////////////////JUMP UNIT REQUIRMENTSS
        bool AndGate11 = (predictionE && ZU->ZeroOutput());
        bool AndGate00 = (!(predictionE) && !(ZU->ZeroOutput()));
        bool is_Hit = AndGate11 || AndGate00; 
        HDU->HDUinputExecute(EXEdata.JrSignal, is_Hit, EXEdata.Branch);

        //branch address calculation...
        int32_t BranchAddress = PC + EXEdata.immediate; 
        int32_t TargetAddress = BranchAddress; 
        if (predictionE) { TargetAddress = PC;}

        ConsoleLog(3, "TargetAddress= ", TargetAddress);
        ConsoleLog(3, "EXEdata.readdata1= ", EXEdata.readdata1);
        ConsoleLog(3, "is_Hit= ", is_Hit);
        ConsoleLog(3, "EXEdata.JrSignal= ", EXEdata.JrSignal);
        ConsoleLog(3, "EXEdata.Branch= ", EXEdata.Branch);
        JU->JumpInputEXE(TargetAddress, EXEdata.readdata1, is_Hit, EXEdata.JrSignal,EXEdata.Branch);
        /////////////////////////////////////////////////////////JUMP UNIT REQUIRMENTSS

        
        //exe needs to wait for the memory input
        FU->WaitForMemoryInput();
        //Mux operation 
        ConsoleLog(3, "READDATA MEM B4 Mux= ", FU->MEMreaddata);
        Operand1=  Op1Mux(EXEdata.readdata1, PC , FU->MEMreaddata, EXEdata.FC);
      
        Operand2= Op2Mux(EXEdata.readdata2 ,EXEdata.immediate, FU->MEMreaddata ,EXEdata.FD);
        ConsoleLog(3, "READDATA MEM after Mux= ", FU->MEMreaddata);
        Out_RegDstM= RegDstMux(EXEdata.rt, EXEdata.rd, EXEdata.RegDst);

        //ALU
       int32_t ALUresult=  ALU(Operand1,Operand2, EXEdata.ALUOp);

       FU->FUinputEXE(ALUresult ,EXEdata.rs, EXEdata.rt, Out_RegDstM, EXEdata.RegWriteEn, EXEdata.MemReadEn);

       
		EXEMEMpipe->writedata(PC,MC,
            EXEdata.RegWriteEn, EXEdata.MemtoReg,//WBS
            EXEdata.MemWriteEn, EXEdata.MemReadEn,//MEMS
            ALUresult,
            EXEdata.readdata2,
            FU->MEMreaddata,
            Out_RegDstM);

        
		ConsoleLog(3, "ePC = ", std::hex, std::setw(8), std::setfill('0'),  PC, " eMC = ", MC);
        ConsoleLog(3, "READDATA MEM after execute is finish= ", FU->MEMreaddata);
	}
}

int32_t ExecuteStage::ALU(int32_t operand1, int32_t operand2, int32_t opSel) {
    int32_t result = 0;

    switch (opSel) {
    case _ADD:
        result = operand1 + operand2;
        break;
    case _SUB:
        result = operand1 - operand2;
        break;
    case _AND:
        result = operand1 & operand2;
        break;
    case _OR:
        result = operand1 | operand2;
        break;
    case _NOR:
        result = ~(operand1 | operand2);
        break;
    case _XOR:
        result = operand1 ^ operand2;
        break;
    case _SLT:
        result = (operand1 < operand2) ? 1 : 0;
        break;
    case _SLL:
        result = operand1 << ((operand2>>6) & 0x1F); //Shift amount is bits 10:6,
        break;                                       //so we shift the imm value to the right by 6 
    case _SRL:                                       //and then we mask the least 5 bits to extract shmat
        result = operand1 >> ((operand2>>6) & 0x1F); //then we perform the shift operation on Op1.
        break;
    case _SGT:
        result = (operand1 > operand2) ? 1 : 0;
        break;
    default:
        result = 0;
        break;
    }

    return result;
}


int32_t ExecuteStage::Op1Mux(int32_t readata1, int32_t PC, int32_t readmemdata, int32_t ForwardC) {
    int32_t placeholder = 0; 
    switch (ForwardC) {
    case 0: 
        placeholder = readata1;
        break;
    case 1 :
        placeholder = PC;
        break;
    case 2 : 
        placeholder = readmemdata;
        break;
    default: 
        placeholder = 0xFFFFFFFF;
        break;
    }
    return placeholder;
}

int32_t ExecuteStage::Op2Mux(int32_t readata2, int32_t Imm, int32_t readmemdata, int32_t ForwardD){
    int32_t placeholder2 = 0; 
    switch(ForwardD){
    case 0:
        placeholder2 = readata2;
        break;
    case 1:
        placeholder2 = Imm;
        break;
    case 2:
        placeholder2 = readmemdata;
        break;
    default:
        placeholder2 = 0xFFFFFFFF;
        break;
    }
    return placeholder2;
}
int32_t ExecuteStage::RegDstMux(int32_t rt, int32_t rd, int32_t RegDstS) {
    int32_t placeholder3 = 0; 
    switch (RegDstS) {
    case 0 : 
        placeholder3 = rt;
        break; 
    case 1 :
        placeholder3 = rd;
        break;
    case 2: 
        placeholder3 = 31;
        break;
    default: 
        placeholder3 = 0;
        break;
    }
    return placeholder3; 
}
void ExecuteStage::stop() {
	running = false;
}
ExecuteStage::~ExecuteStage() {
	// Join the thread to ensure proper cleanup
	if (Executethread.joinable()) {
		Executethread.join();
	}
}