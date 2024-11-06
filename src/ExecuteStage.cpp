#include "ExecuteStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

ExecuteStage::ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe, HazardDetection* HDU, ForwardingUnit* FU)
	: clk(clock), IDEXEpipe(prev_pipe), EXEMEMpipe(next_pipe), HDU(HDU), FU(FU) {
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
		EXEdata.ALUsrc,EXEdata.ALUOp,EXEdata.RegDst,EXEdata.JAL, PC,//exe
		MC, //for display
		EXEdata.readdata1,EXEdata.readdata2,//exe
		EXEdata.immediate,//exe
		EXEdata.rs,EXEdata.rt,EXEdata.rd//exe
		);
        HDU->HDUinputExecute(EXEdata.rt, EXEdata.MemReadEn);
        RegDstMux(EXEdata.rt,EXEdata.rd,EXEdata.RegDst);
        //ForwardingUnit
        FU->FUinputEXE(EXEdata.rs, EXEdata.rt);
        FU->evaluateForwarding();

        //Mux operation 
        JalMux(PC, EXEdata.rs, EXEdata.JAL);
        Op1Mux(Out_JALM, FU->WBdata,FU->MEMdata, FU->ForwardA);
        BeforeOp2Mux(EXEdata.rt, FU->WBdata, FU->MEMdata, FU->ForwardB);
        Op2Mux(Out_BOP2M,EXEdata.immediate,EXEdata.ALUsrc);
        RegDstMux(EXEdata.rt, EXEdata.rd, EXEdata.RegDst);

        //ALU
       int32_t ALUresult=  ALU(Operand1,Operand2, EXEdata.ALUOp);

		EXEMEMpipe->writedata(PC,MC,
            EXEdata.RegWriteEn, EXEdata.MemtoReg,//WBS
            EXEdata.MemWriteEn, EXEdata.MemReadEn,//MEMS
            ALUresult,
            Out_BOP2M,
            Out_RegDstM);

		ConsoleLog(3, "ePC = ", std::hex, std::setw(8), std::setfill('0'),  PC, " eMC = ", MC);

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
        result = operand2 << (operand2 & 0x1F); // Shift amount is bits 10:6, equivalent to lower 5 bits
        break;
    case _SRL:
        result = operand2 >> (operand2 & 0x1F);
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

void ExecuteStage::JalMux(int32_t PC, int32_t Rs, bool JalSignal){
    if (JalSignal == 1)
        Out_JALM = PC;
    else
        Out_JALM = Rs;
}
void ExecuteStage::Op1Mux(int32_t JalMux, int32_t WB32, int32_t MEM32, int32_t ForwardA) {
    switch (ForwardA) {
    case 0: 
        Operand1 = JalMux;
        break;
    case 1 :
        Operand1 = WB32;
        break;
    case 2 : 
        Operand1 = MEM32;
        break;
    default: 
        Operand1 = 0xFFFFFFFF;
        break;
    }
}
void ExecuteStage::BeforeOp2Mux(int32_t Rt, int32_t WB32, int32_t MEM32, int32_t ForwardB){
    switch (ForwardB) {
    case 0:
        Out_BOP2M = Rt;
        break;
    case 1:
        Out_BOP2M = WB32;
        break;
    case 2:
        Out_BOP2M = MEM32;
        break;
    default:
        Out_BOP2M = 0xFFFFFFFF;
        break;
    }
}
void ExecuteStage::Op2Mux(int32_t BOP2Mux, int32_t Imm, int32_t AluSrc){
    switch(AluSrc){
    case 0:
        Operand2 = Imm;
        break;
    case 1:
        Operand2 = BOP2Mux;
        break;
    case 2:
        Operand2 = 0x00000000;
        break;
    default:
        Operand2 = 0xFFFFFFFF;
        break;
    }
}
void ExecuteStage::RegDstMux(int32_t rt, int32_t rd, int32_t RegDstS) {
    switch (RegDstS) {
    case 0: 
        Out_RegDstM = 31; 
        break;
    case 1 : 
        Out_RegDstM = rt; 
        break; 
    case 2 :
        Out_RegDstM = rd;
        break;
    default: 
        Out_RegDstM = 0;
        break;

}
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