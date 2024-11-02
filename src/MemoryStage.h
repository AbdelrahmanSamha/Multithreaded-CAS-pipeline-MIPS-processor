#pragma once
#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H

#include "GlobalClock.h"
#include "EXEMEM.h"
#include "MEMWB.h"
#include <thread>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>

class GlobalClock; // Forward declaration of GlobalClock
class EXEMEM; // Forward declaration of EXEMEM
class MEMWB;  // Forward declaration of MEMWB
class ForwardingUnit;

struct MemoryEntry {
    uint32_t address; // Memory address
    uint32_t data;    // Content of the memory address
};

class MemoryStage {
private:
    GlobalClock* clk;
    EXEMEM* EXEMEMpipe;
    MEMWB* MEMWBpipe;
    ForwardingUnit* FU; 

    std::thread Memorythread;
    uint32_t PC = 0;
    uint32_t MC = 0;
    bool running = true;

    std::vector<MemoryEntry> dataMemory; // Vector to represent data memory

    void Memoryjob();
    void dumpMemoryToFile(const std::string& filename);

public:
    MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe, MEMWB* next_pipe, ForwardingUnit* FU);
    ~MemoryStage();

    void stop();
    void writeToMemory(uint32_t address, uint32_t data);
    uint32_t readFromMemory(uint32_t address);
};

#endif
