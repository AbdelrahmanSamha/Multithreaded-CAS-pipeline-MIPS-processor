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

struct ControlSignals {
    uint8_t ALUop;
    uint8_t RegDst;
    uint8_t ALUSrc;
    bool Branch;
    bool MemReadEn;
    bool MemtoReg;
    bool MemWriteEn;
    bool RegWriteEn;
    bool JR_Signal;
    bool ZERO;
    bool JAL_signal;
};

struct InstructionFields {
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
    uint32_t immediate;
    uint32_t address;
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
   
    ZERO* ZU;
    Jump* JU;

    // Local stage needs:
    std::thread Decodethread;
    bool running = true;  // Temporary for debugging purposes (so we don't use 100% CPU).
    uint32_t PC = 0;      // Program counter
    uint32_t MC = 0;      // Machine code

    // Decoding variables
    InstructionFields instrFields;
    ControlSignals controlSignals;

    void Decodejob();
    void InstructionDecode();
    void GenerateControlSignals();

public:
    void stop();  // Temporary for debugging purposes (so we don't use 100% CPU)
    DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe, ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU, ZERO* Zu, Jump* JU);
    ~DecodeStage();
};

#endif
