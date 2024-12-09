#pragma once
#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <cstdint>  // For standard integer types

class ControlUnit {
public:
    // Constructor
    ControlUnit();

    // Method to set control signals based on opcode and funct
    void setControlSignals(int32_t opcode, int32_t funct);

    // Accessors for control signals
    int32_t getAluOp() const;
    int32_t getRegDst() const;
    int32_t getAluSrc() const;
    bool getBranch() const;
    bool getMemReadEn() const;
    bool getMemToReg() const;
    bool getMemWriteEn() const;
    bool getRegWriteEn() const;
    bool getJrSignal() const;
    bool getZero() const;
    bool getJalSignal() const;

private:
    // Control signals
    int32_t aluOp;
    int32_t regDst;
    int32_t aluSrc;
    bool branch;
    bool memReadEn;
    bool memToReg;
    bool memWriteEn;
    bool regWriteEn;
    bool jrSignal;
    bool zero;
    bool jalSignal;

    // Constants for opcodes and funct values
    static const int32_t R_TYPE = 0x00;
    static const int32_t ADD = 0x20, ADDU = 0x21, SUB = 0x22, SUBU = 0x23;
    static const int32_t OR = 0x25, NOR = 0x27, AND = 0x24;
    static const int32_t SLL = 0x00, SRL = 0x02, JR = 0x08, XOR = 0x26;
    static const int32_t SLT = 0x2A, SGT = 0x2C;

    static const int32_t ADDI = 0x08, ORI = 0x0D, ANDI = 0x0C, LW = 0x23, SLTI = 0x0A;
    static const int32_t SW = 0x2B, XORI = 0x0E;
    static const int32_t BEQ = 0x04, BNE = 0x05, J = 0x02, JAL = 0x03;
};

#endif // CONTROL_UNIT_H
