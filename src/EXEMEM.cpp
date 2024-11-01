#include "EXEMEM.h"
#include "ConsoleLogger.h"
#include <iomanip>

// Constructor to initialize binary semaphores
EXEMEM::EXEMEM() : s1(0)  { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void EXEMEM::writedata(uint32_t PCIn,
                        bool RegWriteEnIn, bool MemtoRegIn,//WBS
                        bool MemWriteEnIn, bool MemReadEnIn, //mem
                        uint32_t resultIn,
                        uint32_t WriteDataIn,
                        uint8_t WriteRegisterIn) {

    s1.acquire();
   
    // Write data
    ConsoleLog(3,"Writing data...");
    this->PC = PCIn;
    this->RegWriteEn = RegWriteEnIn;
    this->MemtoReg = MemtoRegIn;
    this->MemWriteEn = MemWriteEnIn;
    this->MemReadEn = MemReadEnIn;
    this->result = resultIn;
    this->WriteData =WriteDataIn ;
    this->WriteRegister = WriteRegisterIn;

    
}

void EXEMEM::readdata(uint32_t& PCOut,
                        bool& RegWriteEnOut, bool& MemtoRegOut,//WBS
                        bool& MemWriteEnOut, bool& MemReadEnOut, //mem
                        uint32_t& resultOut,
                        uint32_t& WriteDataOut,
                        uint8_t& WriteRegisterOut) {
  
    // Read data
    ConsoleLog(4, "Reading data...");
    PCOut = this->PC;
    RegWriteEnOut = this->RegWriteEn;
    MemtoRegOut = this->MemtoReg;
    MemWriteEnOut = this->MemWriteEn;
    MemReadEnOut = this->MemReadEn;
    resultOut = this->result;
    WriteDataOut = this->WriteData;
    WriteRegisterOut = this->WriteRegister;

    s1.release();
}