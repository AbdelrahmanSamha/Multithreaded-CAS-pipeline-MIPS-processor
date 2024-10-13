#pragma once
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
#include "GlobalClock.h"
#include "IDEXE.h"
#include"EXEMEM.h"
#include <thread>


class GlobalClock;// Forward declaration of GlobalClock
class IDEXE;// Forward declaration of IDEXE
class EXEMEM; //// Forward declaration of EXEMEM
class ExecuteStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	IDEXE* IDEXEpipe;
	EXEMEM* EXEMEMpipe;

private: 
	//local stage needs: 
	std::thread Executethread;
	uint32_t PC;				//Programcounter
	uint32_t MC;				//MachineCode
	void Executejob(); 

public:
	ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe);
	~ExecuteStage();
};









#endif