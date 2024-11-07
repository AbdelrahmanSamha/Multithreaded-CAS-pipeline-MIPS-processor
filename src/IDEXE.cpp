#include "IDEXE.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
IDEXE::IDEXE() : s1(0) { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void IDEXE::writedata(bool RegWriteEnin, bool MemtoRegin, bool MemWriteEnin, bool MemReadEnin,
                      bool ALUSrcin, int32_t ALUOpin, int32_t RegDstin,bool JAL,
                      int32_t PCin, int32_t MCin,
                      int32_t readdata1in, int32_t readdata2in,
                      int32_t immediatein,
                      int32_t rsin, int32_t rtin, int32_t rdin) {
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
    ConsoleLog(2,"RegWriteEnin", std::hex, RegWriteEnin);
    ConsoleLog(2,"MemWriteEnin", std::hex, MemWriteEnin);
    ConsoleLog(2,"MemtoRegin", std::hex, MemtoRegin);
    ConsoleLog(2,"MemReadEnin", std::hex, MemReadEnin);
    ConsoleLog(2,"ALUSrcin", std::hex, ALUSrcin);
    ConsoleLog(2,"ALUOpin",std::hex, ALUOpin);
    ConsoleLog(2,"RegDstin", std::hex, RegDstin);
    ConsoleLog(2,"PCin", std::hex, PCin);
    ConsoleLog(2,"readdata1in", std::hex, readdata1in);
    ConsoleLog(2,"readdata2in", std::hex, readdata2in);
    ConsoleLog(2,"immediatein", std::hex, immediatein);
    ConsoleLog(2, "rsin", std::hex, rsin);
    ConsoleLog(2, "rtin", std::hex, rtin);
    ConsoleLog(2, "rdin", std::hex, rdin);

    
}

void IDEXE::readdata(bool& MemWriteEnOut, bool& MemtoRegOut, bool& RegWriteEnOut, bool& MemReadEnOut,
                    bool& ALUsrcOut, int32_t& ALUOpOut, int32_t& RegDstOut,bool& JALOut,
                    int32_t& PCOut, int32_t& MCOut,
                    int32_t& readdata1Out, int32_t& readdata2Out,
                    int32_t& immediateOut,
                    int32_t& rsOut, int32_t& rtOut, int32_t& rdOut) {
    

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

     ConsoleLog(3, "RegWriteEnOut", RegWriteEnOut);
     ConsoleLog(3, "MemWriteEnOut", MemWriteEnOut);
     ConsoleLog(3, "MemtoRegOut", MemtoRegOut);
     ConsoleLog(3, "MemReadEnOut", MemReadEnOut);
     ConsoleLog(3, "ALUSrcOut", ALUsrcOut);
     ConsoleLog(3, "ALUOpOut", ALUOpOut);
     ConsoleLog(3, "RegDstOut", RegDstOut);
     ConsoleLog(3, "PCOut", PCOut);
     ConsoleLog(3, "readdata1Out", readdata1Out);
     ConsoleLog(3, "readdata2Out", readdata2Out);
     ConsoleLog(3, "immediateOut", immediateOut);
     ConsoleLog(3, "rsOut", rsOut);
     ConsoleLog(3, "rtOut", rtOut);
     ConsoleLog(3, "rdOut", rdOut);
     ConsoleLog(3, "#################");

    // Release semaphore 
    s1.release();
}