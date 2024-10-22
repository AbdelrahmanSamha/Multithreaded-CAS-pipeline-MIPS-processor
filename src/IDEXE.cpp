#include "IDEXE.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
IDEXE::IDEXE() : s1(0), s2(1) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IDEXE::writedata(uint32_t PCin, uint32_t MCin, uint8_t ALUOpin, bool RegDstin, bool ALUSrcin,
                        bool MemReadEnin, bool MemWriteEnin, bool MemtoRegin, bool RegWriteEnin,
                            uint8_t rsin, uint8_t rdin, uint8_t rtin) {
    // Acquire the semaphores
    s1.acquire();
    s2.acquire();

    // Write data
    ConsoleLog(2, "Writing data..." );
    ConsoleLog(2, "dPC = ", std::hex, std::setw(8), std::setfill('0'),PCin ," dMC = " ,MCin );
    this->PC = PCin;
    this->MC = MCin;
    this->ALUOp = ALUOpin;
    this->RegDst = RegDstin;
    this->ALUsrc = ALUSrcin;
    this->MemReadEn = MemReadEnin;
    this->MemWriteEn = MemWriteEnin;
    this->MemtoReg = MemtoRegin;
    this->RegWriteEn = RegWriteEnin;
    this->rs = rsin;
    this->rd = rdin;
    this->rt = rtin;

    // Release semaphore s2
    s2.release();
}

void IDEXE::readdata(uint32_t& PCout, uint32_t& MCout) {
    // Acquire semaphore s2
    s2.acquire();

    // Read data
    ConsoleLog(3, "Reading data..." );
    PCout = this->PC;
    MCout = this->MC;

    // Release semaphore s2
    s2.release();

    // Release semaphore s1
    s1.release();
}