#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H
#include "GlobalClock.h"
#include "MEMWB.h"
#include <thread>

class GlobalClock;// Forward declaration of GlobalClock
class EXEMEM; // Forward declaration of EXEMEM

class WritebackStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	EXEMEM* EXEMEMpipe;
	MEMWB* MEMWBpipe;

private:
	//local stage needs: 
	std::thread Writebackthread;
	uint32_t PC = 0;				//Programcounter
	uint32_t MC = 0;				//MachineCode
	void WBjob();
public:
	WritebackStage(GlobalClock* clock, MEMWB* prev_pipe);
	~WritebackStage();
};
#endif
