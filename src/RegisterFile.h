#pragma once
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <array>
#include <semaphore>  // Use for controlling access with semaphores
#include <cstdint>
#include <iostream>

class RegisterFile {
private:


    std::array<uint32_t, 32> registers;  // 32 general-purpose registers
    std::binary_semaphore reg_semaphore; // Semaphore for later sync

    


public:
    // Constructor
    RegisterFile();

    // Read two registers (source operands)
    void readRegisters(uint8_t readReg1, uint8_t readReg2, uint32_t& data1, uint32_t& data2);

    // Write to a register (destination register)
    void writeRegister(uint8_t writeReg, uint32_t writeData, bool regWrite);

    // Reset all registers to zero and initialize SP and PC
    void reset();

    // Debug method to print the contents of the register file
    void printRegisterFile() const ;

};

#endif // REGISTERFILE_H
