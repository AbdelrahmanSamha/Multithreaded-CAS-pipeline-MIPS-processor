#include "IDEXE.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
IDEXE::IDEXE() : s1(0) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IDEXE::writedata(bool RegWriteEnin, bool MemtoRegin, bool MemWriteEnin, bool MemReadEnin,
                      bool ALUSrcin, uint8_t ALUOpin, uint8_t RegDstin,bool JAL,
                      uint32_t PCin, uint32_t MCin,
                      uint32_t readdata1in, uint32_t readdata2in,
                      uint32_t immediatein,
                      uint8_t rsin, uint8_t rtin, uint8_t rdin) {
    // Acquire the semaphores
    s1.acquire();
   

    // Write data
    ConsoleLog(2, "Writing data..." );
  //  ConsoleLog(2, "dPC = ", std::hex, std::setw(8), std::setfill('0'),PCin ," dMC = " ,MCin );
    this->RegWriteEn = RegWriteEnin;
    this->MemWriteEn = MemWriteEnin;
    this->MemtoReg   = MemtoRegin;
    this->MemReadEn  = MemReadEnin;
    this->ALUsrc     = ALUSrcin;
    this->ALUOp      = ALUOpin;
    this->RegDst     = RegDstin;
    this->PC         = PCin;
    this->MC         = MCin;
    this->readdata1  = readdata1in;
    this->readdata2  = readdata2in;
    this->immediate  = immediatein;
    this->rs         = rsin;
    this->rt         = rtin;
    this->rd         = rdin;

    
}

void IDEXE::readdata(bool& MemWriteEnOut, bool& MemtoRegOut, bool& RegWriteEnOut, bool& MemReadEnOut,
                    bool& ALUsrcOut, uint8_t& ALUOpOut, uint8_t& RegDstOut,bool& JALOut,
                    uint32_t& PCOut, uint32_t& MCOut,
                    uint32_t& readdata1Out, uint32_t& readdata2Out,
                    uint32_t& immediateOut,
                    uint8_t& rsOut, uint8_t& rtOut, uint8_t& rdOut) {
    

    // Read data
    ConsoleLog(3, "ReadOutg data...");

     MemWriteEnOut = this->MemWriteEn;
     MemtoRegOut   = this->MemtoReg;
     RegWriteEnOut = this->RegWriteEn;
     MemReadEnOut  = this->MemReadEn;
     ALUsrcOut     = this->ALUsrc;
     ALUOpOut      = this->ALUOp;
     RegDstOut     = this->RegDst;
     JALOut        = this->JAL;
     PCOut         = this->PC;
     MCOut         = this->MC;
     readdata1Out  = this->readdata1;
     readdata2Out  = this->readdata2;
     immediateOut  = this->immediate;
     rsOut         = this->rs;
     rtOut         = this->rt;
     rdOut         = this->rd;

    // Release semaphore 
    s1.release();
}