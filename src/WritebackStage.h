#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H
#include "GlobalClock.h"
#include "MEMWB.h"
#include <thread>

class GlobalClock;// Forward declaration of GlobalClock
class MEMWB; // Forward declaration of EXEMEM
class ForwardingUnit;
class RegisterFile;
class WritebackStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	MEMWB* MEMWBpipe;
	ForwardingUnit* FU;
	RegisterFile* RF;
private:
	//local stage needs: 
	std::thread Writebackthread;
	uint32_t PC = 0;				//Programcounter
	uint32_t MC = 0;				//MachineCode
	void WBjob();
	bool running = true; // temporary for debugging purposes.(so we dont use 100%CPU)
public:
	void stop(); 
	WritebackStage(GlobalClock* clock, MEMWB* prev_pipe, ForwardingUnit* FU, RegisterFile* RF);
	~WritebackStage();
};
#endif
