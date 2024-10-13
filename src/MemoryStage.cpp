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
		logToConsole("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tMemoryThread waiting for clock tick\n");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		logToConsole("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tMemoryThread starting new clock \n");

		//read data fro,m critical section 
		EXEMEMpipe->readdata(PC, MC);

		//do logic with PC and MC
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tAfterCritical sec read" << "\n" << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tmPC = " << PC << " mMC = " << MC << "\n" << std::endl;
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