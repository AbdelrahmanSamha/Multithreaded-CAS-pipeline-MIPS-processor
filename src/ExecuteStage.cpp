#include "ExecuteStage.h"
#include "ConsoleLogger.h"

ExecuteStage::ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe)
	: clk(clock), IDEXEpipe(prev_pipe) {
	// Launch the decoding thread and store it in the class
	Executethread = std::thread([this]() { Executejob(); });
}


void ExecuteStage::Executejob() {

	//keep working 
	while (true) {
		logToConsole("\t\t\t\t\t\t\t\t\t\t\t\tExecutethread waiting for clock tick\n");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		logToConsole("\t\t\t\t\t\t\t\t\t\t\t\tExecutethread starting new clock \n");

		//read data fro,m critical section 
		IDEXEpipe->readdata(PC, MC);

		//do logic with PC and MC
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tAfterCritical sec read" << "\n" << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\tdPC = " << PC << " dMC = " << MC << "\n" << std::endl;
		//end of deocde logic 
		//writing to ID/EXE pipe.
		//EXEMEMpipe->writedata(PC, MC);
	}
}
ExecuteStage::~ExecuteStage() {
	// Join the thread to ensure proper cleanup
	if (Executethread.joinable()) {
		Executethread.join();
	}
}