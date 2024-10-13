#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
#include "GlobalClock.h"
#include "ExeMem.h"
#include <thread>


class MemoryStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	EXEMEM* EXEMEMpipe;
	//MemWB* MemWBpipe;

private:
	//local stage needs: 
	std::thread Executethread;
	uint32_t PC;				//Programcounter
	uint32_t MC;				//MachineCode
	void Memoryjob();
public:
	MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe);
	~MemoryStage();
};
#endif
