#pragma once
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
#include "GlobalClock.h"
#include "IDEXE.h"
#include "EXEMEM.h"
#include "ForwardingUnit.h"
#include "ZERO.h"
#include "Jump.h"
#include <thread>
class ForwardingUnit;

struct EControlSignals{
	bool MemWriteEn=0;
	bool MemtoReg=0;
	bool RegWriteEn=0;
	bool MemReadEn = 0;
	bool JrSignal = false; 
	bool Branch = false; 
	bool ZeroSignal = false; 
	int32_t FC = 0;
	int32_t FD = 0; 
	int32_t ALUOp=0;
	int32_t RegDst=0;
	int32_t readdata1=0;
	int32_t readdata2=0;
	int32_t immediate=0;
	int32_t rs=0;
	int32_t rt=0;
	int32_t rd=0;
};



class GlobalClock;// Forward declaration of GlobalClock
class IDEXE;// Forward declaration of IDEXE
class EXEMEM; // Forward declaration of EXEMEM
class HazardDetection;
class ForwardingUnit;


class ExecuteStage {
private:
	// Global Comunication needs:
	GlobalClock* clk;
	IDEXE* IDEXEpipe;
	EXEMEM* EXEMEMpipe;
	HazardDetection* HDU;
	ForwardingUnit* FU;
	ZERO* ZU;
	Jump* JU;
private:
	//local stage needs: 
	std::thread Executethread;
	int32_t PC;				//Programcounter
	int32_t MC;				//MachineCode
	bool running = true; // temporary for debugging purposes.(so we dont use 100%CPU)
	EControlSignals EXEdata;
	int32_t Operand1 = 0;
	
	int32_t Operand2 =0 ;
	int32_t Out_JALM, Out_BOP2M;
	int32_t Out_RegDstM;

private: //functions 
	void Executejob();
	int32_t ALU(int32_t operand1, int32_t operand2, int32_t opSel);
	
	int32_t Op1Mux(int32_t readata1, int32_t PC, int32_t readmemdata, int32_t ForwardC);
	
	int32_t Op2Mux(int32_t readata2, int32_t Imm, int32_t readmemdata, int32_t ForwardD);
	int32_t RegDstMux(int32_t rt, int32_t rd, int32_t RegDstS);

private:
	static constexpr int32_t _ADD = 0b0000;
	static constexpr int32_t _SUB = 0b0001;
	static constexpr int32_t _AND = 0b0010;
	static constexpr int32_t _OR = 0b0011;
	static constexpr int32_t _NOR = 0b0100;
	static constexpr int32_t _XOR = 0b0101;
	static constexpr int32_t _SLT = 0b0110;
	static constexpr int32_t _SLL = 0b0111;
	static constexpr int32_t _SRL = 0b1000;
	static constexpr int32_t _SGT = 0b1001;

public:

	void stop();
	ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe, HazardDetection* HDU, ForwardingUnit* FU, ZERO* ZU, Jump* JU);
	~ExecuteStage();
};


#endif