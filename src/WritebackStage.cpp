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

		bool RegWriteEn= false ; 
		bool MemtoReg= false;
		int32_t ReadData=0;
		int32_t Address=0;
		int32_t WriteRegister = 0 ;
		int32_t OutWbMux=0;

		//read data fro,m critical section 
		MEMWBpipe->readdata(PC, MC,
		RegWriteEn, MemtoReg,
		ReadData, Address,
		WriteRegister);
		
		
		ConsoleLog(5, "wPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " wMC = ", MC);
		if (MemtoReg) {
			OutWbMux = ReadData;
		}
		else {
			OutWbMux = Address;
		}
		
		//Forwarding...
		
		RF->writeRegister(WriteRegister,OutWbMux,RegWriteEn);
		 
		
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