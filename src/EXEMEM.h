#ifndef EXEMEM_H
#define EXEMEM_H
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include <semaphore>
class EXEMEM {
private:
	std::binary_semaphore s1;  // Binary semaphore for Raw
	
	int32_t PC = 0x00000000;	//Programcounter
	int32_t MC = 0x00000000; //mc
	bool RegWriteEn= false;
	bool MemtoReg= false; //WBS
	bool MemWriteEn= false;
	bool MemReadEn= false; //mem
	int32_t result= 0;
	int32_t WriteData= 0;
	int32_t Memreaddata = 0;
	int32_t WriteRegister= 0;

public:
	EXEMEM();
	void writedata(int32_t PCIn,int32_t MCin,
		bool RegWriteEnIn, bool MemtoRegIn,//WBS
		bool MemWriteEnIn, bool MemReadEnIn, //mem
		int32_t resultIn,
		int32_t WriteDataIn,
		int32_t MemreaddataIn,
		int32_t WriteRegisterIn);
	void readdata(int32_t& PCOut, int32_t &MCin,
		bool& RegWriteEnOut, bool& MemtoRegOut,//WBS
		bool& MemWriteEnOut, bool& MemReadEnOut, //mem
		int32_t& resultOut,
		int32_t& WriteDataOut,
		int32_t& MemreaddataOut,
		int32_t& WriteRegisterOut);
};
#endif