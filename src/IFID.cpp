#include "IFID.h"


// Constructor to initialize binary semaphores
IFID::IFID() : s1(0), s2(1) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IFID::writedata(uint32_t  PCin, uint32_t  MCin) {
    // Acquire the semaphores
    s1.acquire();
    s2.acquire();

    // Write data
    std::cout << "Writing data..." << std::endl;
    std::cout << " fPC = " << PCin << " fMC = " << MCin << std::endl;
    this->PC = PCin;
    this->MC = MCin;

    // Release semaphore s2
    s2.release();
}

void IFID::readdata(uint32_t& PCout, uint32_t& MCout) {
    // Acquire semaphore s2
    s2.acquire();

    // Read data

    std::cout << "\t\t\t\t\t\tReading data..." << std::endl;
    PCout = this->PC;
    MCout = this->MC;

    // Release 
    s2.release();
    s1.release();
}