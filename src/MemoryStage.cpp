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
        bool RegWriteEn=false ; //WBS
        bool MemtoReg  =false ; //WBS
        bool MemWriteEn=false ; 
        bool MemReadEn =false ;
        int32_t Address = 0;
        int32_t ReadRegister2_writedata=0;
        int32_t Memreaddata_Previous_Stage=0;
        int32_t WriteRegister_RD=0;
        int32_t ReadData= 0xFFFFFFFF; //this is the DM output
        
        //Values are sent bt refrence so whatever data is in the function EXEMEMpipe shall be extracted to them directly..
        EXEMEMpipe->readdata(PC,MC,RegWriteEn, MemtoReg,//wb signals 
            MemWriteEn, MemReadEn, Address, ReadRegister2_writedata, //address needs to be sent to WB pipe
            Memreaddata_Previous_Stage, WriteRegister_RD);
        
        if (MemReadEn) { ReadData = readFromMemory(Address); }

        //Forwarding Early in the stage because EXECUTE is waiting on its input
        FU->FUinputMEM(Address, ReadData, WriteRegister_RD, RegWriteEn, MemReadEn);

        int32_t OutMuxWriteData = WritedataMUX(ReadRegister2_writedata, Address, Memreaddata_Previous_Stage,FU->ForwardEget());
        

        if (MemWriteEn) { writeToMemory(Address, OutMuxWriteData); }//should be fine to stay unsyncronized, BUT if problems arise make a seperate FU input for it. Cant think of a case where this fails atm 12/12/24.
        


        ConsoleLog(4, "mPC = ", std::hex, std::setw(8), std::setfill('0'), PC, " mMC = ", MC);

        MEMWBpipe->writedata(PC, MC,  RegWriteEn, MemtoReg, ReadData , Address, WriteRegister_RD);
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




int32_t MemoryStage::WritedataMUX(int32_t readata2_mem, int32_t address, int32_t memreaddata, int32_t ForwardE) {
    int32_t placeholder_WriteData = 0;
    switch (ForwardE) {
    case 0:
        placeholder_WriteData = readata2_mem;
        break;
    case 1:
        placeholder_WriteData = address;
        break;
    case 2:
        placeholder_WriteData = memreaddata;
        break;
    default:
        placeholder_WriteData = 0xFFFFFFFF;
        break;
    }
    return placeholder_WriteData;
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
    dumpMemoryToFile("MEMORYYYY.txt"); // Dump memory to file on destruction
}
