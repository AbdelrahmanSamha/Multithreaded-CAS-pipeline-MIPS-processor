#include "RegisterFile.h"

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
void RegisterFile::readRegisters(uint8_t readReg1, uint8_t readReg2, uint32_t& data1, uint32_t& data2) {
   
    data1 = registers[readReg1];
    data2 = registers[readReg2];
   
}

// Write to a register if regWrite is true
void RegisterFile::writeRegister(uint8_t writeReg, uint32_t writeData, bool regWrite) {
    
    if (regWrite && writeReg != 0) {  // Don't write to register 0 ($zero)
        registers[writeReg] = writeData;
    }
   
}


// Debug method to print the register file (for inspection during simulation)
void RegisterFile::printRegisterFile() const {

        std::cout << "Register File Contents:\n";
    for (size_t i = 0; i < registers.size(); ++i) {
        std::cout << "R" << i << ": " << std::hex << registers[i] << "\n";

    }

    }
