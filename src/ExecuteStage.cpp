#include "ExecuteStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

ExecuteStage::ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe, HazardDetection* HDU)
	: clk(clock), IDEXEpipe(prev_pipe), EXEMEMpipe(next_pipe), HDU(HDU) {
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
		EXEdata.RegWriteEn, EXEdata.MemtoReg, //WB
		EXEdata.MemWriteEn,EXEdata.MemReadEn, //MEM
		EXEdata.ALUsrc,EXEdata.ALUOp,EXEdata.RegDst,EXEdata.JAL, PC,//exe
		MC, //for display
		EXEdata.readdata1,EXEdata.readdata2,//exe
		EXEdata.immediate,//exe
		EXEdata.rs,EXEdata.rt,EXEdata.rd//exe
		);

        HDU->setInputExecute(EXEdata.rt, EXEdata.MemReadEn);

		ConsoleLog(3, "AfterCritical sec read");
		ConsoleLog(3, "ePC = ", std::hex, std::setw(8), std::setfill('0'),  PC, " eMC = ", MC);





		//EXEMEMpipe->writedata(PC, MC);
	}
}

uint32_t ExecuteStage::ALU(uint32_t operand1, uint32_t operand2, uint8_t opSel) {
    uint32_t result = 0;

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



void ExecuteStage::stop() {
	running = false;
}
ExecuteStage::~ExecuteStage() {
	// Join the thread to ensure proper cleanup
	if (Executethread.joinable()) {
		Executethread.join();
	}
}