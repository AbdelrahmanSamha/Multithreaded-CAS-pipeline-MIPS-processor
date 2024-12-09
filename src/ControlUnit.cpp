#include "ControlUnit.h"

// Constructor
ControlUnit::ControlUnit()
    : aluOp(0), regDst(0), aluSrc(0), branch(false), memReadEn(false),
    memToReg(false), memWriteEn(false), regWriteEn(false),
    jrSignal(false), zero(false), jalSignal(false) {}

// Set control signals based on opcode and funct
void ControlUnit::setControlSignals(int32_t opcode, int32_t funct) {
    // Reset signals
    aluOp = 0; regDst = 0; aluSrc = 0; branch = false; memReadEn = false;
    memToReg = false; memWriteEn = false; regWriteEn = false;
    jrSignal = false; zero = false; jalSignal = false;

    switch (opcode) {
    case R_TYPE:
        regDst = 1; regWriteEn = true; aluSrc = 0;
        switch (funct) {
        case ADD: aluOp = 2; break;
        case SUB: aluOp = 6; break;
        case AND: aluOp = 0; break;
        case OR: aluOp = 1; break;
        case XOR: aluOp = 7; break;
        case SLT: aluOp = 10; break;
        case JR: jrSignal = true; break;
        }
        break;
    case LW:
        aluSrc = 1; memReadEn = true; memToReg = true; regWriteEn = true;
        aluOp = 2;
        break;
    case SW:
        aluSrc = 1; memWriteEn = true;
        aluOp = 2;
        break;
    case BEQ:
        branch = true; aluOp = 6;
        break;
    case JAL:
        jalSignal = true; regWriteEn = true;
        break;
    case J:
        // Jump handled separately in the fetch stage
        break;
    case ADDI:
        aluSrc = 1; regWriteEn = true;
        aluOp = 2;
        break;
    case ORI:
        aluSrc = 1; regWriteEn = true;
        aluOp = 1;
        break;
    default:
        // Handle invalid opcode
        break;
    }
}

// Accessor methods
int32_t ControlUnit::getAluOp() const { return aluOp; }
int32_t ControlUnit::getRegDst() const { return regDst; }
int32_t ControlUnit::getAluSrc() const { return aluSrc; }
bool ControlUnit::getBranch() const { return branch; }
bool ControlUnit::getMemReadEn() const { return memReadEn; }
bool ControlUnit::getMemToReg() const { return memToReg; }
bool ControlUnit::getMemWriteEn() const { return memWriteEn; }
bool ControlUnit::getRegWriteEn() const { return regWriteEn; }
bool ControlUnit::getJrSignal() const { return jrSignal; }
bool ControlUnit::getZero() const { return zero; }
bool ControlUnit::getJalSignal() const { return jalSignal; }
