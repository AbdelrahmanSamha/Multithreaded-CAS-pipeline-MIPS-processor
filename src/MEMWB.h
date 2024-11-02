#ifndef MEMWB_H
#define MEMWB_H
#include "MemoryStage.h"
#include "WritebackStage.h"

#include <semaphore>
class MEMWB {
private:
	std::binary_semaphore s1;  // Binary semaphore Raw
	
	uint32_t PC = 0x00000000;	//Programcounter
	uint32_t MC = 0x00000000; //MachineCode

	bool RegWriteEn = false;
	bool MemtoReg=false;
	uint32_t ReadData = 0x0;
	uint32_t Address =0x0;
	uint8_t WriteRegister=0x0;
public:
	MEMWB();
	void writedata(uint32_t PCin, uint32_t MCin,
		bool RegWriteEnIn, bool MemtoRegIn,
		uint32_t ReadDataIn, uint32_t AddressIn,
		uint8_t WriteRegisterIn);

	void readdata(uint32_t& PCout, uint32_t& MCout,
		bool &RegWriteEnOut, bool& MemtoRegOut,
		uint32_t& ReadDataOut, uint32_t& AddressOut,
		uint8_t& WriteRegisterOut);
};
#endif