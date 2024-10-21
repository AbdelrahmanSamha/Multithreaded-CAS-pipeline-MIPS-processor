#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe,ControlUnit* Cu, RegisterFile* rf)
	: clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe), CU(Cu), RF(rf) {
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

		ConsoleLog(2, "AfterCritical sec read");
		ConsoleLog(2, "dPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " dMC =", MC);

		//**********************************************************************do logic with PC and MC**********************************************************************//
		//Instruction decoding...
		uint8_t opcode = (MC >> 26) & 0x3F;  // 6-bit opcode
		uint8_t rs = (MC >> 21) & 0x1F;      // 5-bit source register
		uint8_t rt = (MC >> 16) & 0x1F;      // 5-bit target register
		uint8_t rd = (MC >> 11) & 0x1F;      // 5-bit destination register
		uint8_t shamt = (MC >> 6) & 0x1F;    // 5-bit shift amount
		uint8_t funct = MC & 0x3F;           // 6-bit function code
		uint32_t immediate = MC & 0xFFFF;     // 16-bit immediate value
		uint32_t address = MC & 0x03FFFFFF;   // 26-bit address (for jump)
		

		//ControlUnit signals generation
		CU->setControlSignals(opcode, funct);


		//RF write 
		uint32_t readdata1;
		uint32_t readdata2;
		RF->readRegisters(rs, rt, readdata1, readdata2);
		
		bool Zero; //zero signal, 1 if readdata1 and readdata2 are equal, 0 otherwise.
		Zero = (readdata1 == readdata2);


		//end of deocde logic 
		// 
		//writing to ID/EXE pipe.
		ConsoleLog(2, "Decoding logic done...");
		IDEXEpipe->writedata(PC, MC);
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