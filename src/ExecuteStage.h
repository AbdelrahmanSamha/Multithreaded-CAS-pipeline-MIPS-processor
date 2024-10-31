#pragma once
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
#include "GlobalClock.h"
#include "IDEXE.h"
#include "EXEMEM.h"
#include <thread>

struct EControlSignals{
	bool RegWriteEn;
	bool MemtoReg;
	bool MemWriteEn;
	bool MemReadEn;
	bool ALUsrc;
	uint8_t ALUOp;
	uint8_t RegDst;
	bool JAL;
	uint32_t readdata1;
	uint32_t readdata2;
	uint32_t immediate;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
};



class GlobalClock;// Forward declaration of GlobalClock
class IDEXE;// Forward declaration of IDEXE
class EXEMEM; // Forward declaration of EXEMEM

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
	bool running = true; // temporary for debugging purposes.(so we dont use 100%CPU)
	EControlSignals EXEdata; 
	void ALU();
public:
	void stop();
	ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe);
	~ExecuteStage();
};









#endif