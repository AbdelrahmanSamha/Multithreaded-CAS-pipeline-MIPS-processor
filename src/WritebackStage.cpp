#include "WritebackStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

WritebackStage::WritebackStage(GlobalClock* clock, MEMWB* prev_pipe, ForwardingUnit* FU, RegisterFile* RF)
	: clk(clock), MEMWBpipe(prev_pipe),FU(FU),RF(RF){
	// Launch the decoding thread and store it in the class
	Writebackthread = std::thread([this]() { WBjob(); });
}


void WritebackStage::WBjob() {

	//keep working 
	while (running) {
		ConsoleLog(5, "WBthread waiting for clock tick");
		clk->waitforClockTick(); //called at the beggining of all the stages. 
		ConsoleLog(5, "WBthread starting new clock");

		bool RegWriteEn, MemtoReg;
		uint32_t ReadData, Address;
		uint8_t WriteRegister = 0 ;
		uint32_t OutWbMux;

		//read data fro,m critical section 
		MEMWBpipe->readdata(PC, MC,
		RegWriteEn, MemtoReg,
		ReadData, Address,
		WriteRegister);
		
		if (MemtoReg) {
			OutWbMux = ReadData;
		}
		else {
			OutWbMux = Address;
		}
		
		//Forwarding...
		FU->FUinputWB(RegWriteEn, WriteRegister, OutWbMux);
		RF->writeRegister(WriteRegister,OutWbMux,RegWriteEn);
		ConsoleLog(5, "AfterCritical sec read");
		ConsoleLog(5, "wPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " wMC = ", MC);
		 
		
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