#pragma once 
#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <cstdint>  // For standard integer types

class ControlUnit {
public:
    // Constructor
    ControlUnit();

    // Public method to set control signals based on opcode and funct
    void setControlSignals(uint8_t opcode, uint8_t funct);

    // Accessors for control signals
    uint8_t getALUOp() const ;
    bool getRegDst() const;
    bool getBranch() const;
    bool getMemReadEn() const;
    bool getMemtoReg() const;
    bool getMemWriteEn() const;
    bool getRegWriteEn() const;
    bool getAluSrc() const;
    bool getJumpSel() const;

private:
    // Control signals
    uint8_t ALUOp;
    bool RegDst;
    bool Branch;
    bool MemReadEn;
    bool MemtoReg;
    bool MemWriteEn;
    bool RegWriteEn;
    bool ALUSrc;
    bool JumpSel;

    // Constants for opcodes and funct
    static const uint8_t Rtype = 0x00;
    static const uint8_t ADD = 0x20, ADDU = 0x21, SUB = 0x22, SUBU = 0x23;
    static const uint8_t OR = 0x25, NOR = 0x27, AND = 0x24;
    static const uint8_t SLL = 0x00, SRL = 0x02, JR = 0x08, XOR = 0x26;

    static const uint8_t ADDI = 0x08, ORI = 0x0D, ANDI = 0x0C, LW = 0x23;
    static const uint8_t SW = 0x2B, XORI = 0x0E;
    static const uint8_t BEQ = 0x04, BNE = 0x05, J = 0x02, JAL = 0x03;
};

#endif // CONTROL_UNIT_H