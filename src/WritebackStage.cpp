#include "WritebackStage.h"
#include "ConsoleLogger.h"

WritebackStage::WritebackStage(GlobalClock* clock, MEMWB* prev_pipe)
	: clk(clock), MEMWBpipe(prev_pipe) {
	// Launch the decoding thread and store it in the class
	Writebackthread = std::thread([this]() { WBjob(); });
}


void MemoryStage::Memoryjob() {

	//keep working 
	while (true) {
		ConsoleLog(4, "MemoryThread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(4, "MemoryThread starting new clock");

		//read data fro,m critical section 
		EXEMEMpipe->readdata(PC, MC);

		//do logic with PC and MC
		ConsoleLog(4, "AfterCritical sec read");
		ConsoleLog(4, "mPC = ", PC, " mMC = ", MC);
		//end of deocde logic 
		
	}
}
WritebackStage::~WritebackStage() {
	// Join the thread to ensure proper cleanup
	if (Writebackthread.joinable()) {
		Writebackthread.join();
	}
}