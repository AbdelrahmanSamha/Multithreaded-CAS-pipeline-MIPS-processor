#ifndef EXEMEM_H
#define EXEMEM_H
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include <semaphore>
class EXEMEM {
private:
	std::binary_semaphore s1;  // Binary semaphore for Raw
	
	uint32_t PC = 0x00000000;	//Programcounter
	bool RegWriteEn= false;
	bool MemtoReg= false; //WBS
	bool MemWriteEn= false;
	bool MemReadEn= false; //mem
	uint32_t result= false;
	uint32_t WriteData= false;
	uint8_t WriteRegister= false;

public:
	EXEMEM();
	void writedata(uint32_t PCIn,
		bool RegWriteEnIn, bool MemtoRegIn,//WBS
		bool MemWriteEnIn, bool MemReadEnIn, //mem
		uint32_t resultIn,
		uint32_t WriteDataIn,
		uint8_t WriteRegisterIn);
	void readdata(uint32_t& PCOut,
		bool& RegWriteEnOut, bool& MemtoRegOut,//WBS
		bool& MemWriteEnOut, bool& MemReadEnOut, //mem
		uint32_t& resultOut,
		uint32_t& WriteDataOut,
		uint8_t& WriteRegisterOut);
};
#endif