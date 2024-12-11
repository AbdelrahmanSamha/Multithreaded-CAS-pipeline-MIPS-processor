#pragma once
#ifndef IDEXE_H
#define IDEXE_H
#include "DecodeStage.h" 
#include "ExecuteStage.h"
#include <thread>
#include <semaphore>

//achieve fixing the idexe pipe and after that work on the structure of the Execute stage while synchrounizing it with the 
//decode stage for a RAW protocl.
class IDEXE {

private:
	std::binary_semaphore s1;  // Binary semaphore for raw
	
	int32_t PC = 0x00000000;	//Programcounter
	int32_t MC = 0x00000000; //MachineCode
	int32_t ALUOp=0x0;
	int32_t RegDst=0x0;
	int32_t FC=0;
	int32_t FD= 0;
	bool JrSignal= false;
	bool Branch = false; 
	bool ZeroS = false; 
	bool MemReadEn = false;
	bool MemWriteEn= false;
	bool MemtoReg  = false;
	bool RegWriteEn= false;
	int32_t readdata1=0x0;
	int32_t readdata2= 0x0;
	int32_t immediate= 0x0;
	int32_t rs= 0x0;
	int32_t rt= 0x0;
	int32_t rd= 0x0;
	
	
public : 
	IDEXE();
	void writedata(bool RegWriteEnin, bool MemtoRegin, bool MemWriteEnin, bool MemReadEnin,
	    int32_t ALUOpin, int32_t RegDstin, int32_t FCin, int32_t FDin,bool JrSingalin,bool Branchin, bool ZeroSignalin, 
		int32_t PCin, int32_t MC,
		int32_t readdata1in, int32_t readdata2in,
		int32_t immediatein,
		int32_t rsin, int32_t rtin, int32_t rdin);

	void readdata(bool& MemWriteEnOut, bool& MemtoRegOut, bool& RegWriteEnOut, bool& MemReadEnOut,
	    int32_t& ALUOpOut, int32_t& RegDstOut, int32_t& FCOut, int32_t& FDOut, bool &JrSingalOut, bool& BranchOut, bool& ZeroSignalOut,
		int32_t& PCOut, int32_t& MCOut,
		int32_t& readdata1Out, int32_t& readdata2Out,
		int32_t& immediateOut,
		int32_t& rsOut, int32_t& rtOut, int32_t& rdOut);
};

#endif