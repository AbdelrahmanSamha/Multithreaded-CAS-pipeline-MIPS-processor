#ifndef MEMWB_H
#define MEMWB_H
#include "MemoryStage.h"
#include "WritebackStage.h"

#include <semaphore>
class MEMWB {
private:
	std::binary_semaphore s1;  // Binary semaphore Raw
	
	int32_t PC = 0x00000000;	//Programcounter
	int32_t MC = 0x00000000; //MachineCode

	bool RegWriteEn = false;
	bool MemtoReg=false;
	int32_t ReadData = 0x0;
	int32_t Address =0x0;
	int32_t WriteRegister=0x0;
public:
	MEMWB();
	void writedata(int32_t PCin, int32_t MCin,
		bool RegWriteEnIn, bool MemtoRegIn,
		int32_t ReadDataIn, int32_t AddressIn,
		int32_t WriteRegisterIn);

	void readdata(int32_t& PCout, int32_t& MCout,
		bool &RegWriteEnOut, bool& MemtoRegOut,
		int32_t& ReadDataOut, int32_t& AddressOut,
		int32_t& WriteRegisterOut);
};
#endif