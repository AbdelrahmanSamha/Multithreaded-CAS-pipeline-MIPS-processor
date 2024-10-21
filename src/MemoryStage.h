#pragma once
#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H
#include "GlobalClock.h"
#include "EXEMEM.h"
#include "MEMWB.h"
#include <thread>

class GlobalClock;// Forward declaration of GlobalClock
class EXEMEM; // Forward declaration of EXEMEM
class MEMWB;  //forward declaration of MEMWB

class MemoryStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	EXEMEM* EXEMEMpipe;
	MEMWB* MEMWBpipe;

private:
	//local stage needs: 
	std::thread Memorythread;
	uint32_t PC= 0;				//Programcounter
	uint32_t MC= 0;				//MachineCode
	void Memoryjob();
	bool running = true; // temporary for debugging purposes.(so we dont use 100%CPU)
public:
	void stop();
	MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe,MEMWB* next_pipe);
	~MemoryStage();
};
#endif
