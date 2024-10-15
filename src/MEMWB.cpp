#include "MEMWB.h"
#include "ConsoleLogger.h"


// Constructor to initialize binary semaphores
MEMWB::MEMWB() : s1(0), s2(1) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void MEMWB::writedata(uint32_t  PCin, uint32_t  MCin) {
    // Acquire the semaphores
    s1.acquire();
    s2.acquire();

    // Write data
    ConsoleLog(4, "Writing data...");
    ConsoleLog(4, "mPC = ", PCin, " mMC = ", MCin);
    this->PC = PCin;
    this->MC = MCin;

    // Release semaphore s2
    s2.release();
}

void MEMWB::readdata(uint32_t& PCout, uint32_t& MCout) {
    // Acquire semaphore s2
    s2.acquire();

    // Read data

    ConsoleLog(5, "Reading data...");
    PCout = this->PC;
    MCout = this->MC;

    // Release semaphore s2
    s2.release();

    // Release semaphore s1
    s1.release();
}