#include "MemoryStage.h"
#include "ConsoleLogger.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

MemoryStage::MemoryStage(GlobalClock* clock, EXEMEM* prev_pipe, MEMWB* next_pipe, ForwardingUnit* FU)
    : clk(clock), EXEMEMpipe(prev_pipe), MEMWBpipe(next_pipe),FU(FU) {
    Memorythread = std::thread([this]() { Memoryjob(); });
}

void MemoryStage::Memoryjob() {
    while (running) {
        ConsoleLog(4, "MemoryThread waiting for clock tick");
        clk->waitforClockTick();
        ConsoleLog(4, "MemoryThread starting new clock");
        bool RegWriteEn, MemtoReg; //WBS
        bool MemWriteEn, MemReadEn; 
        int32_t Address, WriteData;
        int32_t WriteRegister;
        int32_t ReadData= 0; //this is the DM output
        
        EXEMEMpipe->readdata(PC,MC,RegWriteEn, MemtoReg, MemWriteEn, MemReadEn, Address, WriteData, WriteRegister);

        //Forwarding Early in the stage because EXECUTE is waiting on its input
        FU->FUinputMEM(RegWriteEn,WriteRegister,Address);

        if (MemReadEn) { ReadData = readFromMemory(Address); }
        if (MemWriteEn) { writeToMemory(Address, WriteData); }

        ConsoleLog(4, "mPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " mMC = ", MC);

        MEMWBpipe->writedata(PC, MC,  RegWriteEn, MemtoReg, ReadData , Address, WriteRegister);
    }
}

void MemoryStage::writeToMemory(int32_t address, int32_t data) {
    auto it = std::find_if(dataMemory.begin(), dataMemory.end(),[address](const MemoryEntry& entry) { return entry.address == address; });
    if (it != dataMemory.end()) {
        it->data = data; // Update existing entry
    }
    else {
        dataMemory.emplace_back(address, data);// Add new entry
    }
}

int32_t MemoryStage::readFromMemory(int32_t address) {
    auto it = std::find_if(dataMemory.begin(), dataMemory.end(),
        [address](const MemoryEntry& entry) { return entry.address == address; });
    if (it != dataMemory.end()) {
        return it->data;
    }
    return 0; // Default value if address not found
}



void MemoryStage::dumpMemoryToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        ConsoleLog(1, "Error opening file for memory dump");
        return;
    }

    // Sort the dataMemory vector by address for ordered output
    std::sort(dataMemory.begin(), dataMemory.end(), [](const MemoryEntry& a, const MemoryEntry& b) {
        return a.address < b.address;
        });

    size_t entriesPerRow = 8; // 8 entries per row (each entry is 4 bytes, total 32 bytes per row)

    for (size_t i = 0; i < dataMemory.size(); i += entriesPerRow) {
        // Print the base address for this row
        file << "0x" << std::hex << std::setw(8) << std::setfill('0') << dataMemory[i].address << ":\t";

        // Print up to 8 memory entries in this row with offsets +0, +4, ..., +1C
        for (size_t j = 0; j < entriesPerRow && (i + j) < dataMemory.size(); ++j) {
            file << "+0x" << std::hex << j * 4 << ": 0x"
                << std::setw(8) << std::setfill('0') << dataMemory[i + j].data << "\t";
        }

        file << "\n"; // Move to the next row
    }

    file.close();
}


void MemoryStage::stop() {
    running = false;
}


MemoryStage::~MemoryStage() {
    if (Memorythread.joinable()) {
        Memorythread.join();
    }
    dumpMemoryToFile("DataSegment.txt"); // Dump memory to file on destruction
}
