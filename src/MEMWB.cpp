#include "MEMWB.h"
#include "ConsoleLogger.h"
#include<iomanip>

// Constructor to initialize binary semaphores
MEMWB::MEMWB() : s1(0){ } // Initial state of s1 = 0 (not available), s2 = 1 (available)

void MEMWB::writedata(int32_t PCin, int32_t MCin,
                      bool RegWriteEnIn, bool MemtoRegIn,
                      int32_t ReadDataIn, int32_t AddressIn,
                      int32_t WriteRegisterIn) {
   
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
    ConsoleLog(4, "RegWriteEnIn", RegWriteEnIn);
    ConsoleLog(4, "MemtoRegIn", MemtoRegIn);
    ConsoleLog(4, "ReadDataIn", ReadDataIn);
    ConsoleLog(4, "AddressIn", AddressIn);
    ConsoleLog(4, "WriteRegisterIn", WriteRegisterIn);
    
}

void MEMWB::readdata(int32_t& PCout, int32_t& MCout,
                     bool& RegWriteEnOut, bool& MemtoRegOut,
                     int32_t& ReadDataOut, int32_t& AddressOut,
                     int32_t& WriteRegisterOut) {

    // Read data
    ConsoleLog(5, "ReadOutg data...");
    PCout = this->PC;
    MCout = this->MC;
    RegWriteEnOut = this->RegWriteEn;
    MemtoRegOut = this->MemtoReg;
    ReadDataOut = this->ReadData;
    AddressOut = this->Address; 
    WriteRegisterOut = this->WriteRegister;

    ConsoleLog(5, "RegWriteEnOut", RegWriteEnOut);
    ConsoleLog(5, "MemtoRegOut", MemtoRegOut);
    ConsoleLog(5, "ReadDataOut", ReadDataOut);
    ConsoleLog(5, "AddressOut", AddressOut);
    ConsoleLog(5, "WriteRegisterOut", WriteRegisterOut);
    s1.release();
}