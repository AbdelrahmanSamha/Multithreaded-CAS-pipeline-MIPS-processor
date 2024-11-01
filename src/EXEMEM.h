#ifndef EXEMEM_H
#define EXEMEM_H
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include <semaphore>
class EXEMEM {
private:
	std::binary_semaphore s1;  // Binary semaphore for Raw
	
	uint32_t PC = 0x00000000;	//Programcounter
	bool RegWriteEn ,MemtoReg; //WBS
	bool MemWriteEn,MemReadEn; //mem
	uint32_t result, WriteData;
	uint8_t WriteRegister;

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