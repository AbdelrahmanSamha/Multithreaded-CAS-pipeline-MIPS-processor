#include "MEMWB.h"
#include "ConsoleLogger.h"
#include<iomanip>

// Constructor to initialize binary semaphores
MEMWB::MEMWB() : s1(0)  { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void MEMWB::writedata(uint32_t  PCin, uint32_t  MCin) {
   
    s1.acquire();
   
    // Write data
    ConsoleLog(4, "Writing data...");
    ConsoleLog(4, "mPC = ", std::hex, std::setw(8), std::setfill('0'), PCin, " mMC = ", MCin);
    this->PC = PCin;
    this->MC = MCin;

}

void MEMWB::readdata(uint32_t& PCout, uint32_t& MCout) {

    // Read data
    ConsoleLog(5, "Reading data...");
    PCout = this->PC;
    MCout = this->MC;

    s1.release();
}