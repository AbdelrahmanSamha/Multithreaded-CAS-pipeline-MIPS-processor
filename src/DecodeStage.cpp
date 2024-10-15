#include "DecodeStage.h"
#include "ConsoleLogger.h"

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe)
	: clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe) {
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

		//do logic with PC and MC

		ConsoleLog(2, "AfterCritical sec read" );
		ConsoleLog(2, "dPC = " , PC , "dMC =" , MC );
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