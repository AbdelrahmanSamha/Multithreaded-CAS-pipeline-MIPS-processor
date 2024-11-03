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
#include "Jump.h"
#include "HazardDetection.h"


class GlobalClock; // Forward declaration of GlobalClock
class IFID;        // Forward declaration of IFID

class FetchStage {
private:
    // Global Comunication needs:
    GlobalClock* clk;      // Clock object for synchronization
    IFID* IFIDpipe;// IF/ID pipeline register
    HazardDetection* HDU;
    Jump* JU;

private:
    //Local Stage needs
    std::thread Fetchthread;      // Thread for the fetch stage
    const std::vector<Instruction>& instructions; // Reference to vector of instructions
    int32_t fetchInstruction();  // Fetch instruction by PC
    bool hasNextInstruction();    // Check if there are more instructions
    int32_t BaseAddress = 0x00400000;
    int32_t PC = 0;              // Program Counter
    void Fetchjob();              // Fetch job method run by the thread
    bool running = true;          // temporary for debugging purposes.
    
public:
    void stop();                    // temporary for debugging purposes.(so we dont use 100%CPU)
    FetchStage(GlobalClock* clock, const std::vector<Instruction>& instrVector, IFID* pipe,HazardDetection*HDU ,Jump*JU);
    ~FetchStage();                // Destructor to join the thread
};

#endif
