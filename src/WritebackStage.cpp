#include "WritebackStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

WritebackStage::WritebackStage(GlobalClock* clock, MEMWB* prev_pipe)
	: clk(clock), MEMWBpipe(prev_pipe) {
	// Launch the decoding thread and store it in the class
	Writebackthread = std::thread([this]() { WBjob(); });
}


void WritebackStage::WBjob() {

	//keep working 
	while (running) {
		ConsoleLog(5, "WBthread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(5, "WBthread starting new clock");

		//read data fro,m critical section 
		MEMWBpipe->readdata(PC, MC);

		//do logic with PC and MC
		ConsoleLog(5, "AfterCritical sec read");
		ConsoleLog(5, "wPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " wMC = ", MC);
		//end of deocde logic 
		
	}
}
void WritebackStage::stop() {
	running = false; 

}
WritebackStage::~WritebackStage() {
	// Join the thread to ensure proper cleanup
	if (Writebackthread.joinable()) {
		Writebackthread.join();
	}
}