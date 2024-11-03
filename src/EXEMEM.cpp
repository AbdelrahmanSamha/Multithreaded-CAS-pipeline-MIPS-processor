#include "EXEMEM.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
EXEMEM::EXEMEM() : s1(0)  { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void EXEMEM::writedata(int32_t PCIn, int32_t MCin,
                        bool RegWriteEnIn, bool MemtoRegIn,//WBS
                        bool MemWriteEnIn, bool MemReadEnIn, //mem
                        int32_t resultIn,
                        int32_t WriteDataIn,
                        int32_t WriteRegisterIn) {

    s1.acquire();
   
    // Write data
    ConsoleLog(3,"Writing data...");
    this->PC = PCIn;
    this->MC = MCin;
    this->RegWriteEn = RegWriteEnIn;
    this->MemtoReg = MemtoRegIn;
    this->MemWriteEn = MemWriteEnIn;
    this->MemReadEn = MemReadEnIn;
    this->result = resultIn;
    this->WriteData =WriteDataIn ;
    this->WriteRegister = WriteRegisterIn;
    ConsoleLog(3, "RegWriteEnIn", RegWriteEnIn);
    ConsoleLog(3, "MemtoRegIn", MemtoRegIn);
    ConsoleLog(3, "MemWriteEnIn", MemWriteEnIn);
    ConsoleLog(3, "MemReadEnIn", MemReadEnIn);
    ConsoleLog(3, "resultIn", resultIn);
    ConsoleLog(3, "WriteDataIn", WriteDataIn);
    ConsoleLog(3, "WriteRegisterIn", WriteRegisterIn);
   
}

void EXEMEM::readdata(int32_t& PCOut, int32_t& MCOut,
                        bool& RegWriteEnOut, bool& MemtoRegOut,//WBS
                        bool& MemWriteEnOut, bool& MemReadEnOut, //mem
                        int32_t& resultOut,
                        int32_t& WriteDataOut,
                        int32_t& WriteRegisterOut) {
  
    // Read data
    ConsoleLog(4, "ReadOutg data...");
    PCOut = this->PC;
    MCOut = this->MC;
    RegWriteEnOut = this->RegWriteEn;
    MemtoRegOut = this->MemtoReg;
    MemWriteEnOut = this->MemWriteEn;
    MemReadEnOut = this->MemReadEn;
    resultOut = this->result;
    WriteDataOut = this->WriteData;
    WriteRegisterOut = this->WriteRegister;

    ConsoleLog(4, "RegWriteEnOut", RegWriteEnOut);
    ConsoleLog(4, "MemtoRegOut", MemtoRegOut);
    ConsoleLog(4, "MemWriteEnOut", MemWriteEnOut);
    ConsoleLog(4, "MemReadEnOut", MemReadEnOut);
    ConsoleLog(4, "resultOut", resultOut);
    ConsoleLog(4, "WriteDataOut", WriteDataOut);
    ConsoleLog(4, "WriteRegisterOut", WriteRegisterOut);

    s1.release();
}