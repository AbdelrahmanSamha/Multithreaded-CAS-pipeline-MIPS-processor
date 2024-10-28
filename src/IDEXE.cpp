#include "IDEXE.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
IDEXE::IDEXE() : s1(0), s2(1) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IDEXE::writedata(bool RegWriteEnin, bool MemtoRegin, bool MemWriteEnin, bool MemReadEnin,
                      bool ALUSrcin, uint8_t ALUOpin, bool RegDstin,
                      uint32_t PCin,
                      uint32_t readdata1in, uint32_t readdata2in,
                      uint32_t immediatein,
                      uint8_t rsin, uint8_t rtin, uint8_t rdin) {
    // Acquire the semaphores
    s1.acquire();
    s2.acquire();

    // Write data
    ConsoleLog(2, "Writing data..." );
  //  ConsoleLog(2, "dPC = ", std::hex, std::setw(8), std::setfill('0'),PCin ," dMC = " ,MCin );
    this->MemWriteEn = MemWriteEnin;
    this->MemtoReg = MemtoRegin;
    this->RegWriteEn = RegWriteEnin;
    this->MemReadEn = MemReadEnin;
    this->ALUsrc = ALUSrcin;
    this->ALUOp = ALUOpin;
    this->RegDst = RegDstin;
    this->PC = PCin;
    this->readdata1 = readdata1in;
    this->readdata2 = readdata2in;
    this->immediate = immediatein;
    this->rs = rsin;
    this->rt = rtin;
    this->rd = rdin;

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