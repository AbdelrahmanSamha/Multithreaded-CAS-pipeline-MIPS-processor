#include "MemoryStage.h"
#include "ConsoleLogger.h"

MemoryStage::MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe)
	: clk(clock), EXEMEMpipe(prev_pipe) {
	// Launch the decoding thread and store it in the class
	Executethread = std::thread([this]() { Memoryjob(); });
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
		//end of deocde logic 
		//writing to ID/EXE pipe.
		//EXEMEMpipe->writedata(PC, MC);
	}
}
MemoryStage::~MemoryStage() {
	// Join the thread to ensure proper cleanup
	if (Executethread.joinable()) {
		Executethread.join();
	}
}