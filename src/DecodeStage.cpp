#include "DecodeStage.h"
#include "ConsoleLogger.h"

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe)
	: clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe) {
	// Launch the decoding thread and store it in the class
	Decodethread = std::thread([this]() { Decodejob(); });
}


void DecodeStage::Decodejob() {

	//keep working 
	while (true) {
		logToConsole("\t\t\t\t\t\tDecodethread waiting for clock tick\n");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		logToConsole("\t\t\t\t\t\tDecodethread starting new clock \n");

		//read data fro,m critical section 
		IFIDpipe->readdata(PC, MC);

		//do logic with PC and MC
		std::cout << "\t\t\t\t\t\tAfterCritical sec read" << "\n" << std::endl;
		std::cout << "\t\t\t\t\t\tdPC = " << PC << " dMC = " << MC << "\n" << std::endl;
		//end of deocde logic 
		//writing to ID/EXE pipe.
		std::cout << "\t\t\t\t\t\tDecoding logic done..." << std::endl;
		IDEXEpipe->writedata(PC, MC);
	}
}
DecodeStage::~DecodeStage() {
	// Join the thread to ensure proper cleanup
	if (Decodethread.joinable()) {
		Decodethread.join();
	}
}