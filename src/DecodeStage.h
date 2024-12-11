#pragma once
#ifndef DECODESTAGE_H
#define DECODESTAGE_H

#include <thread>
#include "GlobalClock.h"
#include "IFID.h"
#include "IDEXE.h"
#include "RegisterFile.h"
#include "ControlUnit.h"
#include "HazardDetection.h"
#include "FetchStage.h"
#include "ZERO.h"
#include "Jump.h"
#include "ForwardingUnit.h"

struct ControlSignals {
    int32_t ALUop =0;
    int32_t RegDst = 0;
    int32_t ALUSrc = 0;
    bool Branch = 0;
    bool MemReadEn = 0;
    bool MemtoReg = 0;
    bool MemWriteEn = 0;
    bool RegWriteEn = 0;
    bool JR_Signal = 0;
    bool ZERO = 0;
    bool JAL_signal = 0;
};

struct InstructionFields {
    int32_t opcode = 0;
    int32_t rs = 0;
    int32_t rt = 0;
    int32_t rd = 0;
    int32_t shamt = 0;
    int32_t funct=0;
    int32_t immediate = 0;
    int32_t address = 0;
};

class GlobalClock; // Forward declaration of GlobalClock
class IFID;        // Forward declaration of IFID 
class IDEXE; // Forward declaration of IDEXE

class DecodeStage {
private:
    // Global Communication needs:
    GlobalClock* clk;
    IFID* IFIDpipe;
    IDEXE* IDEXEpipe;
    ControlUnit* CU;
    RegisterFile* RF;
    HazardDetection* HDU;
    ForwardingUnit* FU;
   
    ZERO* ZU;
    Jump* JU;

    // Local stage needs:
    std::thread Decodethread;
    bool running = true;  // Temporary for debugging purposes (so we don't use 100% CPU).
    int32_t PC = 0;      // Program counter
    int32_t MC = 0;      // Machine code

    // Decoding variables
    InstructionFields instrFields;
    ControlSignals controlSignals;

    void Decodejob();
    void InstructionDecode();
    void GenerateControlSignals();
    int32_t FAMUX(int32_t FA, int32_t readdata1, int32_t AluResult, int32_t MEMAddress, int32_t MEMreaddata);
    int32_t FBMUX(int32_t FB, int32_t readdata2, int32_t AluResult, int32_t MEMAddress, int32_t MEMreaddata);

public:
    void stop();  // Temporary for debugging purposes (so we don't use 100% CPU)
    DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe, ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU, ForwardingUnit* FU, ZERO* Zu, Jump* JU);
    ~DecodeStage();
};

#endif
