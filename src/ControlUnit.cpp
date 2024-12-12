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
        regDst = 1;
        regWriteEn = true;
        aluSrc = 0;
        switch (funct) {
        case ADD: aluOp = 0; break;
        case SUB: aluOp = 1; break;
        case OR: aluOp = 3; break;
        case NOR: aluOp = 4; break;
        case AND: aluOp = 2; break;
        case SLL: aluOp = 7; aluSrc = 1; break;
        case SRL: aluOp = 8; aluSrc = 1; break;
        case JR: jrSignal = true; regWriteEn = false; break;
        case XOR: aluOp = 5; break;
        case SLT: aluOp = 6; break;
        case SGT: aluOp = 9; aluSrc = 0; break;
        default: break;
        }
        break;

    case ADDI:
        aluOp = 0;
        regDst = 0;
        regWriteEn = true;
        aluSrc = 1;
        break;

    case ORI:
        aluOp = 3;
        regDst = 0;
        regWriteEn = true;
        aluSrc = 1;
        break;

    case ANDI:
        aluOp = 2;
        regDst = 0;
        regWriteEn = true;
        aluSrc = 1;
        break;

    case LW:
        aluOp = 0;
        regDst = 0;
        memReadEn = true;
        memToReg = true;
        regWriteEn = true;
        aluSrc = 1;
        break;

    case SW:
        aluOp = 0;
        regDst = 0;
        memWriteEn = true;
        aluSrc = 1;
        break;

    case XORI:
        aluOp = 5;
        regDst = 0;
        regWriteEn = true;
        aluSrc = 1;
        break;

    case BEQ:
        branch = true;
        zero = true;
        aluSrc = 0;
        break;

    case BNE:
        branch = true;
        zero = false;
        aluSrc = 0;
        break;

    case SLTI:
        aluOp = 6;
        regDst = 0;
        regWriteEn = true;
        aluSrc = 1;
        break;

    case J:
        // No specific control signals
        regDst = 0;
        break;

    case JAL:
        aluOp = 0;
        regDst = 2;
        regWriteEn = true;
        jalSignal = true;
        aluSrc = 0;
        break;

    default:
        // Default case: reset all signals
        aluOp = 0; regDst = 0; branch = false; memReadEn = false;
        memToReg = false; memWriteEn = false; regWriteEn = false;
        aluSrc = 0; jrSignal = false; zero = false; jalSignal = false;
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
