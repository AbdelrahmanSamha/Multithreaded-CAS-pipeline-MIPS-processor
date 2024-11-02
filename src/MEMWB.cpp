#include "MEMWB.h"
#include "ConsoleLogger.h"
#include<iomanip>

// Constructor to initialize binary semaphores
MEMWB::MEMWB() : s1(0)  { } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void MEMWB::writedata(uint32_t PCin, uint32_t MCin,
                      bool RegWriteEnIn, bool MemtoRegIn,
                      uint32_t ReadDataIn, uint32_t AddressIn,
                      uint8_t WriteRegisterIn) {
   
    s1.acquire();
   
    // Write data
    ConsoleLog(4, "Writing data...");
    ConsoleLog(4, "mPC = ", std::hex, std::setw(8), std::setfill('0'), PCin, " mMC = ", MCin);
    this->PC = PCin;
    this->MC = MCin;
    this->RegWriteEn = RegWriteEnIn;
    this->MemtoReg = MemtoRegIn;
    this->ReadData = ReadDataIn;
    this->Address = AddressIn;
    this->WriteRegister = WriteRegisterIn;
}

void MEMWB::readdata(uint32_t& PCout, uint32_t& MCout,
                     bool& RegWriteEnOut, bool& MemtoRegOut,
                     uint32_t& ReadDataOut, uint32_t& AddressOut,
                     uint8_t& WriteRegisterOut) {

    // Read data
    ConsoleLog(5, "Reading data...");
    PCout = this->PC;
    MCout = this->MC;
    RegWriteEnOut = this->RegWriteEn;
    MemtoRegOut = this->MemtoReg;
    ReadDataOut = this->ReadData;
    AddressOut = this->Address; 
    WriteRegisterOut = this->WriteRegister;
    s1.release();
}