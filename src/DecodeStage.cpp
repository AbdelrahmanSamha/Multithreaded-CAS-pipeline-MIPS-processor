#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe,ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU)
	: clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe), CU(Cu), RF(rf), HDU(HDU) {
	// Launch the decoding thread and store it in the class
	Decodethread = std::thread([this]() { Decodejob(); });
}


void DecodeStage::Decodejob() {

	//keep working 
	while (running) {
		ConsoleLog(2, "Decodethread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(2, "Decodethread starting new clock");

		//read data fro,m critical section 
		IFIDpipe->readdata(PC, MC);

		ConsoleLog(2, std::hex ,std::setw(8), MC);
		ConsoleLog(2, "AfterCritical sec read");
		ConsoleLog(2, "dPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " dMC =", MC);

		//**********************************************************Decode STAGE start **********************************************************//
		//Instruction decoding...
		uint8_t opcode = (MC >> 26) & 0x3F;  // 6-bit opcode
		uint8_t rs = (MC >> 21) & 0x1F;      // 5-bit source register
		uint8_t rt = (MC >> 16) & 0x1F;      // 5-bit target register
		uint8_t rd = (MC >> 11) & 0x1F;      // 5-bit destination register
		uint8_t shamt = (MC >> 6) & 0x1F;    // 5-bit shift amount
		uint8_t funct = MC & 0x3F;           // 6-bit function code
		uint32_t immediate = MC & 0xFFFF;     // 16-bit immediate value
		uint32_t address = MC & 0x03FFFFFF;   // 26-bit address (for jump)
		

		//******************************************************ControlUnit signals generation ******************************************************//
		CU->setControlSignals(opcode, funct);
		//get the control signals 
		//signals to the pipe
		uint8_t ALUOp = CU->getALUOp();
		bool ALUsrc = CU->getAluSrc();
		bool MemReadEn = CU->getMemReadEn();
		bool MemtoReg = CU->getMemtoReg();
		bool MemWriteEn = CU->getMemWriteEn();
		bool RegDst = CU->getRegDst();
		bool RegWriteEn = CU->getRegWriteEn();
		//signals to the fetch stage...
		bool JumpSel = CU->getJumpSel();
		bool Branch = CU->getBranch();

		//RF read will always happen
		uint32_t readdata1;
		uint32_t readdata2;
		RF->readRegisters(rs, rt, readdata1, readdata2);

		//zero signal, 1 if readdata1 and readdata2 are equal, 0 otherwise. sent to the Fetch stage.
		bool Zero = (readdata1 == readdata2);

		//adding PC + (Imm<<2). sending the value to the Fetch stage.
		uint32_t Address = PC + (immediate << 2);

		//Sending the signals & data to the required UNITS 
		HDU->setInputDecode(rs, rt);
		IDEXEpipe->writedata(PC, MC, ALUOp, RegDst, ALUsrc, MemReadEn, MemWriteEn, MemtoReg, RegWriteEn, rs, rt, rd);


		ConsoleLog(2, "Decoding logic done...");

	}
}

void DecodeStage::stop() {
	running = false; 
}
DecodeStage::~DecodeStage() {
	// Join the thread to ensure proper cleanup
	if (Decodethread.joinable()) {
		Decodethread.join();
	}
}