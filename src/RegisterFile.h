#pragma once
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <array>
#include <semaphore>  // Use for controlling access with semaphores
#include <cstdint>
#include <iostream>

class RegisterFile {
private:


    std::array<int32_t, 32> registers;  // 32 general-purpose registers
    std::binary_semaphore reg_semaphore; // Semaphore for later sync

    


public:
    // Constructor
    RegisterFile();

    // Read two registers (source operands)
    void readRegisters(int32_t readReg1, int32_t readReg2, int32_t& data1, int32_t& data2);

    // Write to a register (destination register)
    void writeRegister(int32_t writeReg, int32_t writeData, bool regWrite);

    // Reset all registers to zero and initialize SP and PC
    void reset();

    // Debug method to print the contents of the register file
    void printRegisterFile() const ;

};

#endif // REGISTERFILE_H
