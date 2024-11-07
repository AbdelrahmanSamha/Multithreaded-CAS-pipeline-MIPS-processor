#include "RegisterFile.h"
#include "ConsoleLogger.h"
// Constructor: Initialize all registers to 0 and set SP and PC
RegisterFile::RegisterFile(): reg_semaphore(0) {
    reset();  // Reset the registers and initialize special registers (SP, PC)
}

// Reset all registers to zero and initialize SP and PC
void RegisterFile::reset() {
    
    registers.fill(0);  // Fill all registers with 0
    registers[29] = 0x7FFFFFFC;  // $sp (stack pointer)
    registers[31] = 0x00400000;  // PC (if using $31 to store the program counter)
    
}

// Read two registers: data1 = registers[readReg1], data2 = registers[readReg2]
void RegisterFile::readRegisters(int32_t readReg1, int32_t readReg2, int32_t& data1, int32_t& data2) {
    reg_semaphore.acquire();
    data1 = registers[readReg1];
    data2 = registers[readReg2];
   
}

// Write to a register if regWrite is true
void RegisterFile::writeRegister(int32_t writeReg, int32_t writeData, bool regWrite) {
    
    if (regWrite && writeReg != 0) {  // Don't write to register 0 ($zero)
        registers[writeReg] = writeData;
    }
    reg_semaphore.release();
}


// Debug method to print the register file (for inspection during simulation)
void RegisterFile::printRegisterFile() const {

        ConsoleLog(1, "Register File Contents:\n");
    for (size_t i = 0; i < registers.size(); ++i) {
        ConsoleLog(1, "R" ,std::dec ,i , ": " , std::hex , registers[i] , "\n");

    }

    }
