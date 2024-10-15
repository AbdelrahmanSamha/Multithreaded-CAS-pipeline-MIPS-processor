#include "IFID.h"
#include "ConsoleLogger.h"


// Constructor to initialize binary semaphores
IFID::IFID() : s1(0), s2(1) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IFID::writedata(uint32_t  PCin, uint32_t  MCin) {
    // Acquire the semaphores
    s1.acquire();
    s2.acquire();

    // Write data
    ConsoleLog(1, "Writing data..." );
    ConsoleLog(1, " fPC = " , PCin , " fMC = " , MCin );
    this->PC = PCin;
    this->MC = MCin;

    // Release semaphore s2
    s2.release();
}

void IFID::readdata(uint32_t& PCout, uint32_t& MCout) {
    // Acquire semaphore s2
    s2.acquire();

    // Read data

    ConsoleLog(2, "Reading data...");
    PCout = this->PC;
    MCout = this->MC;

    // Release 
    s2.release();
    s1.release();
}