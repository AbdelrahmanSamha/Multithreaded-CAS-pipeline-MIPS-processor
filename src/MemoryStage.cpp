#include "MemoryStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

MemoryStage::MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe,MEMWB* next_pipe)
	: clk(clock), EXEMEMpipe(prev_pipe), MEMWBpipe(next_pipe) {
	// Launch the decoding thread and store it in the class
	Memorythread = std::thread([this]() { Memoryjob(); });
}


void MemoryStage::Memoryjob() {

	//keep working 
	while (running) {
		ConsoleLog(4,"MemoryThread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(4, "MemoryThread starting new clock");

		//read data fro,m critical section 
		EXEMEMpipe->readdata(PC, MC);

		//do logic with PC and MC
		ConsoleLog(4, "AfterCritical sec read" );
		ConsoleLog(4, "mPC = ", std::hex, std::setw(8), std::setfill('0'), PC ," mMC = " , MC );
		
		MEMWBpipe->writedata(PC, MC);
	}
}
void MemoryStage::stop() {
	running = false; 
}
MemoryStage::~MemoryStage() {
	// Join the thread to ensure proper cleanup
	if (Memorythread.joinable()) {
		Memorythread.join();
	}
}