#include "MemoryStage.h"
#include "ConsoleLogger.h"

MemoryStage::MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe)
	: clk(clock), EXEMEMpipe(prev_pipe) {
	// Launch the decoding thread and store it in the class
	Memorythread = std::thread([this]() { Memoryjob(); });
}


void MemoryStage::Memoryjob() {

	//keep working 
	while (true) {
		ConsoleLog(4,"MemoryThread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(4, "MemoryThread starting new clock");

		//read data fro,m critical section 
		EXEMEMpipe->readdata(PC, MC);

		//do logic with PC and MC
		ConsoleLog(4, "AfterCritical sec read" );
		ConsoleLog(4, "mPC = " , PC ," mMC = " , MC );
		
		MEMWBpipe->writedata(PC, MC);
	}
}
MemoryStage::~MemoryStage() {
	// Join the thread to ensure proper cleanup
	if (Memorythread.joinable()) {
		Memorythread.join();
	}
}