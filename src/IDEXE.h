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
	
	uint32_t PC = 0x00000000;	//Programcounter
	uint32_t MC = 0x00000000; //MachineCode
	uint8_t ALUOp=0x0; 
	uint8_t RegDst=0x0;
	bool JAL=false;
	bool ALUsrc= false;
	bool MemReadEn = false;
	bool MemWriteEn= false;
	bool MemtoReg  = false;
	bool RegWriteEn= false;
	uint32_t readdata1=0x0;
	uint32_t readdata2= 0x0;
	uint32_t immediate= 0x0;
	uint8_t rs= 0x0;
	uint8_t rt= 0x0;
	uint8_t rd= 0x0;
	
	
public : 
	IDEXE();
	void writedata(bool RegWriteEnin, bool MemtoRegin, bool MemWriteEnin, bool MemReadEnin,
		bool ALUsrcin, uint8_t ALUOpin, uint8_t RegDstin,bool JALin,
		uint32_t PCin, uint32_t MC,
		uint32_t readdata1in, uint32_t readdata2in,
		uint32_t immediatein,
		uint8_t rsin, uint8_t rtin, uint8_t rdin);

	void readdata(bool& MemWriteEnOut, bool& MemtoRegOut, bool& RegWriteEnOut, bool& MemReadEnOut,
		bool& ALUsrcOut, uint8_t& ALUOpOut, uint8_t& RegDstOut, bool& JALOut,
		uint32_t& PCOut, uint32_t& MCOut,
		uint32_t& readdata1Out, uint32_t& readdata2Out,
		uint32_t& immediateOut,
		uint8_t& rsOut, uint8_t& rtOut, uint8_t& rdOut);
};

#endif