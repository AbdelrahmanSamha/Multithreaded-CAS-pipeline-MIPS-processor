#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
#include "GlobalClock.h"
#include "ExeMem.h"
#include <thread>

class GlobalClock;// Forward declaration of GlobalClock
class EXEMEM; // Forward declaration of EXEMEM

class MemoryStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	EXEMEM* EXEMEMpipe;
	//MemWB* MemWBpipe;

private:
	//local stage needs: 
	std::thread Executethread;
	uint32_t PC= 0;				//Programcounter
	uint32_t MC= 0;				//MachineCode
	void Memoryjob();
public:
	MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe);
	~MemoryStage();
};
#endif
