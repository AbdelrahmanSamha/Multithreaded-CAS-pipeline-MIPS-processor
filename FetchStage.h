#pragma once
#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H

#include "GlobalClock.h"
#include "IFID.h"
#include "Assembler.h" // To access the Instruction struct
#include <vector>
#include <thread>
#include <cstdint>
#include <iostream>

class GlobalClock; // Forward declaration of GlobalClock
class IFID;        // Forward declaration of IFID

class FetchStage {
private:
    GlobalClock* clk;      // Clock object for synchronization
    IFID* IFIDpipe;        // IF/ID pipeline register

    uint32_t PC = 0;       // Program Counter
    const std::vector<Instruction>& instructions; // Reference to vector of instructions

    uint32_t fetchInstruction();  // Fetch instruction by PC
    void Fetchjob();              // Fetch job method run by the thread
    bool hasNextInstruction();    // Check if there are more instructions
    uint32_t BaseAddress = 0x00400000;
    std::thread Fetchthread;      // Thread for the fetch stage

public:
    FetchStage(GlobalClock* clock, IFID* pipe, const std::vector<Instruction>& instrVector);
    ~FetchStage();                // Destructor to join the thread
};

#endif
