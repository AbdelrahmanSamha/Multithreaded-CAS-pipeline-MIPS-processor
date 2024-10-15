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
    // Global Comunication needs:
    GlobalClock* clk;      // Clock object for synchronization
    IFID* IFIDpipe;        // IF/ID pipeline register
private:
    //Local Stage needs
    std::thread Fetchthread;      // Thread for the fetch stage
    const std::vector<Instruction>& instructions; // Reference to vector of instructions
    uint32_t fetchInstruction();  // Fetch instruction by PC
    bool hasNextInstruction();    // Check if there are more instructions
    uint32_t BaseAddress = 0x00400000;
    uint32_t PC = 0;              // Program Counter
    void Fetchjob();              // Fetch job method run by the thread
    
public:
    FetchStage(GlobalClock* clock, const std::vector<Instruction>& instrVector, IFID* pipe);
    ~FetchStage();                // Destructor to join the thread
};

#endif
