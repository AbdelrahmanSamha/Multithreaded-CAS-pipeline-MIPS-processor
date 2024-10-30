#include "EXEMEM.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
EXEMEM::EXEMEM() : s1(0)  { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void EXEMEM::writedata(uint32_t  PCin, uint32_t  MCin) {

    s1.acquire();
   
    // Write data
    ConsoleLog(3,"Writing data...");
    ConsoleLog(3, "ePC = " ,std::hex, std::setw(8),std::setfill('0'), PCin, " eMC = ", MCin);
    this->PC = PCin;
    this->MC = MCin;

    
}

void EXEMEM::readdata(uint32_t& PCout, uint32_t& MCout) {
  
    // Read data
    ConsoleLog(4, "Reading data...");
    PCout = this->PC;
    MCout = this->MC;

    s1.release();
}