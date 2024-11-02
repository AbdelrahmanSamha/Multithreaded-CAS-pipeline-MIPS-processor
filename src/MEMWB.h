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

	bool RegWriteEn, MemtoReg;
	uint32_t ReadData, Address;
	uint8_t WriteRegister;
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