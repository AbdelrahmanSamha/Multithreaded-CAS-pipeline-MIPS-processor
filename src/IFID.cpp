#include "IFID.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
IFID::IFID(HazardDetection* HDU, Jump* JU):HDU(HDU),JU(JU), s1(0) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IFID::writedata(int32_t  PCin, int32_t  MCin, bool PredictionInF) {
    // Acquire the semaphores
    s1.acquire();

    // Write data
    

    
        ConsoleLog(1, "Writing data...");
        ConsoleLog(1, " fPC = ", std::hex, std::setw(8), std::setfill('0'), PCin, " fMC = ", MCin);
        this->PC = PCin;
        this->MC = MCin;
        this->Prediction = PredictionInF;
    }


void IFID::readdata(int32_t& PCout, int32_t& MCout, bool& PredictionOutD) {
    
  
    
        PCout = this->PC;
        MCout = this->MC;
        PredictionOutD = this->Prediction;
    
    ConsoleLog(2, "Reading data...");
    ConsoleLog(2, "PC", PCout);
    ConsoleLog(2, "MC", MCout);
    ConsoleLog(2, "#################");

    // Release 
    s1.release();
}