#include "ControlUnit.h"

ControlUnit::ControlUnit()
    : aluop(0), RegDst(0), ALUSrc(0),
    Branch(false), MemReadEn(false), MemtoReg(false),
    MemWriteEn(false), RegWriteEn(false), JR_Signal(false),
    ZERO(false), JAL_signal(false) {}

// Set control signals based on opcode and funct fields
void ControlUnit::setControlSignals(int32_t opcode, int32_t funct) {
    // Reset control signals to default values
    aluop = 0;
    RegDst = 0;
    ALUSrc = 0;
    Branch = false;
    MemReadEn = false;
    MemtoReg = false;
    MemWriteEn = false;
    RegWriteEn = false;
    JR_Signal = false;
    ZERO = false;
    JAL_signal = false;

    // Set control signals based on opcode
    switch (opcode) {
    case Rtype:
        aluop = 2;
        RegDst = 1;
        RegWriteEn = true;
        if (funct == JR) {
            JR_Signal = true;
            RegWriteEn = false;
        }
        break;
    case ADDI:
    case ORI:
    case ANDI:
    case XORi:
        aluop = 2;
        ALUSrc = 1;
        RegWriteEn = true;
        break;
    case LW:
        aluop = 0;
        ALUSrc = 1;
        MemReadEn = true;
        MemtoReg = true;
        RegWriteEn = true;
        break;
    case SW:
        aluop = 0;
        ALUSrc = 1;
        MemWriteEn = true;
        break;
    case BEQ:
        aluop = 1;
        Branch = true;
        ZERO = true;
        break;
    case BNE:
        aluop = 1;
        Branch = true;
        ZERO = false;
        break;
    case J:
        // No aluop, direct jump
        break;
    case JAL:
        aluop = 2;
        RegDst = 2;
        RegWriteEn = false;
        JAL_signal = true;
        ALUSrc = 2;
        break;
    default:
        // Unsupported opcode, leave default signals
        break;
    }
}

// Accessor methods for control signals
int32_t ControlUnit::getALUOp() const { return aluop; }
int32_t ControlUnit::getRegDst() const { return RegDst; }
int32_t ControlUnit::getALUSrc() const { return ALUSrc; }
bool ControlUnit::getBranch() const { return Branch; }
bool ControlUnit::getMemReadEn() const { return MemReadEn; }
bool ControlUnit::getMemtoReg() const { return MemtoReg; }
bool ControlUnit::getMemWriteEn() const { return MemWriteEn; }
bool ControlUnit::getRegWriteEn() const { return RegWriteEn; }
bool ControlUnit::getJR_Signal() const { return JR_Signal; }
bool ControlUnit::getZERO() const { return ZERO; }
bool ControlUnit::getJAL_Signal() const { return JAL_signal; }
