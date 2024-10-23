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
	std::binary_semaphore s1;  // Binary semaphore for write
	std::binary_semaphore s2;  // Binary semaphore for read
	uint32_t PC = 0x00000000;	//Programcounter
	uint32_t MC = 0x00000000; //MachineCode
	uint8_t ALUOp; 
	bool RegDst	   ;
	bool ALUsrc	   ;
	bool MemReadEn ;
	bool MemWriteEn;
	bool MemtoReg  ;
	bool RegWriteEn;
	uint32_t readdata1;
	uint32_t readdata2;
	uint32_t immediate;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	
	
public : 
	IDEXE();
	void writedata(uint32_t PCin, uint32_t MCin, uint8_t ALUOp, bool RegDst, bool ALUsrc,
					bool MemReadEn, bool MemWriteEn, bool MemtoReg, bool RegWriteEn,
		uint32_t readdata1, uint32_t readdata2, uint32_t immediate,
						uint8_t rs , uint8_t rt , uint8_t rd);
	void readdata(uint32_t& PCout, uint32_t& MCout);
};







#endif