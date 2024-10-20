#include "ExecuteStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

ExecuteStage::ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe)
	: clk(clock), IDEXEpipe(prev_pipe), EXEMEMpipe(next_pipe) {
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
		IDEXEpipe->readdata(PC, MC);

		//do logic with PC and MC
		ConsoleLog(3, "AfterCritical sec read");
		ConsoleLog(3, "ePC = ", std::hex, std::setw(8), std::setfill('0'),  PC, " eMC = ", MC);
		//end of deocde logic 
		//writing to EXE/MEM pipe.
		EXEMEMpipe->writedata(PC, MC);
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