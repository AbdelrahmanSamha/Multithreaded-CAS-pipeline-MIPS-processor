#include "WritebackStage.h"
#include "ConsoleLogger.h"

WritebackStage::WritebackStage(GlobalClock* clock, MEMWB* prev_pipe)
	: clk(clock), MEMWBpipe(prev_pipe) {
	// Launch the decoding thread and store it in the class
	Writebackthread = std::thread([this]() { WBjob(); });
}


void WritebackStage::WBjob() {

	//keep working 
	while (true) {
		ConsoleLog(5, "WBthread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(5, "WBthread starting new clock");

		//read data fro,m critical section 
		MEMWBpipe->readdata(PC, MC);

		//do logic with PC and MC
		ConsoleLog(5, "AfterCritical sec read");
		ConsoleLog(5, "wPC = ", PC, " wMC = ", MC);
		//end of deocde logic 
		
	}
}
WritebackStage::~WritebackStage() {
	// Join the thread to ensure proper cleanup
	if (Writebackthread.joinable()) {
		Writebackthread.join();
	}
}