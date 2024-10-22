#include "ControlUnit.h"
#include <iostream>

// Constructor
ControlUnit::ControlUnit() : ALUOp(0), RegDst(false), Branch(false), MemReadEn(false),
MemtoReg(false), MemWriteEn(false), RegWriteEn(false),
ALUSrc(false), JumpSel(false) {}

// Set control signals based on opcode and funct
void ControlUnit::setControlSignals(uint8_t opcode, uint8_t funct) {
    // Default values for control signals
    ALUOp = 0;
    RegDst = Branch = MemReadEn = MemtoReg = MemWriteEn = RegWriteEn = ALUSrc = JumpSel = false;

    switch (opcode) {
    case Rtype:
        ALUOp = 0x2;  // ALU control comes from funct
        RegDst = true;
        RegWriteEn = true;
        switch (funct) {
        //case ADD: case ADDU: case SUB: case SUBU: case OR:
        //case NOR: case AND: case SLL: case SRL: case XOR:
            // No specific action needed, these are regular R-type operations
            //break;
        case JR:
            JumpSel = true;
            RegWriteEn = false;
            break;
        }
        break;

    case ADDI: case ORI: case ANDI:
        ALUOp = 0x2;  // ALU uses immediate operations
        ALUSrc = true;
        RegWriteEn = true;
        break;

    case LW:
        ALUOp = 0x0;  // ALU adds base + offset
        ALUSrc = true;
        MemReadEn = true;
        MemtoReg = true;
        RegWriteEn = true;
        break;

    case SW:
        ALUOp = 0x0;  // ALU adds base + offset
        ALUSrc = true;
        MemWriteEn = true;
        break;

    case XORI:
        ALUOp = 0x2;  // ALU uses XOR immediate
        ALUSrc = true;
        RegWriteEn = true;
        break;

    case BEQ: case BNE:
        ALUOp = 0x1;  // ALU subtracts
        Branch = true;
        break;

    case J:
        JumpSel = true;
        break;

    case JAL:
        JumpSel = true;
        break;
    }
}

// Accessor methods
uint8_t ControlUnit::getALUOp() const { return ALUOp; }
bool ControlUnit::getRegDst() const { return RegDst; }
bool ControlUnit::getBranch() const { return Branch; }
bool ControlUnit::getMemReadEn() const { return MemReadEn; }
bool ControlUnit::getMemtoReg() const { return MemtoReg; }
bool ControlUnit::getMemWriteEn() const { return MemWriteEn; }
bool ControlUnit::getRegWriteEn() const { return RegWriteEn; }
bool ControlUnit::getAluSrc() const { return ALUSrc; }
bool ControlUnit::getJumpSel() const { return JumpSel; }
