#ifndef EXEMEM_H
#define EXEMEM_H
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include <semaphore>
class EXEMEM {
private:
	std::binary_semaphore s1;  // Binary semaphore for write
	std::binary_semaphore s2;  // Binary semaphore for read
	uint32_t PC = 0x00000000;	//Programcounter
	uint32_t MC = 0x00000000; //MachineCode

public:
	EXEMEM();
	void writedata(uint32_t PCin, uint32_t MCin);
	void readdata(uint32_t& PCout, uint32_t& MCout);
};
#endif