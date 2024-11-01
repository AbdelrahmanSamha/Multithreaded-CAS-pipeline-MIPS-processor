#pragma once
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
#include "GlobalClock.h"
#include "IDEXE.h"
#include "EXEMEM.h"
#include "ForwardingUnit.h"
#include <thread>

class ForwardingUnit;

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
private:
	//local stage needs: 
	std::thread Executethread;
	uint32_t PC;				//Programcounter
	uint32_t MC;				//MachineCode
	bool running = true; // temporary for debugging purposes.(so we dont use 100%CPU)
	EControlSignals EXEdata;
	uint32_t Operand1, Operand2;
	uint32_t Out_JALM, Out_BOP2M;
	uint8_t Out_RegDstM;

private: //functions 
	void Executejob();
	uint32_t ALU(uint32_t operand1, uint32_t operand2, uint8_t opSel);
	void JalMux(uint32_t PC, uint32_t Rs, bool JalSignal);
	void Op1Mux(uint32_t JalMux, uint32_t WB32, uint32_t MEM32, uint8_t ForwardA);
	void BeforeOp2Mux(uint32_t Rt, uint32_t WB32, uint32_t MEM32, uint8_t ForwardB);
	void Op2Mux(uint32_t BOP2Mux, uint32_t Imm, uint8_t AluSrc);
	void RegDstMux(uint8_t rt, uint8_t rd, uint8_t RegDstS);

private:
	static constexpr uint8_t _ADD = 0b0000;
	static constexpr uint8_t _SUB = 0b0001;
	static constexpr uint8_t _AND = 0b0010;
	static constexpr uint8_t _OR = 0b0011;
	static constexpr uint8_t _NOR = 0b0100;
	static constexpr uint8_t _XOR = 0b0101;
	static constexpr uint8_t _SLT = 0b0110;
	static constexpr uint8_t _SLL = 0b0111;
	static constexpr uint8_t _SRL = 0b1000;
	static constexpr uint8_t _SGT = 0b1001;

public:

	void stop();
	ExecuteStage(GlobalClock* clock, IDEXE* prev_pipe, EXEMEM* next_pipe, HazardDetection* HDU, ForwardingUnit* FU);
	~ExecuteStage();
};


#endif