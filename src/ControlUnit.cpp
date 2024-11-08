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
    Branch = false;
    MemReadEn = false;
    MemtoReg = false;
    MemWriteEn = false;
    RegWriteEn = false;
    ALUSrc = 0;
    JR_Signal = false;
    ZERO = false;
    JAL_signal = false;

    // Set control signals based on opcode
    switch (opcode) {
    case Rtype:
        RegDst = 1;
        RegWriteEn = true;
        ALUSrc = 0;

        switch (funct) {
        case ADD:
        case ADDU:
            aluop = 0;
            break;
        case SUB:
        case SUBU:
            aluop = 1;
            break;
        case OR:
            aluop = 3;
            break;
        case NOR:
            aluop = 4;
            break;
        case AND:
            aluop = 2;
            break;
        case SLL:
            aluop = 7;
            ALUSrc = 1;
            break;
        case SRL:
            aluop = 8;
            ALUSrc = 1;
            break;
        case JR:
            JR_Signal = true;
            RegWriteEn = false;
            break;
        case XOR:
            aluop = 5;
            break;
        case SLT:
            aluop = 6;
            break;
        case SGT:
            aluop = 9;
            ALUSrc = 1;
            break;
        default:
            break;
        }
        break;

    case ADDI:
        aluop = 0;
        RegDst = 0;
        ALUSrc = 1;
        RegWriteEn = true;
        break;

    case ORI:
        aluop = 3;
        RegDst = 0;
        ALUSrc = 1;
        RegWriteEn = true;
        break;

    case ANDI:
        aluop = 2;
        RegDst = 0;
        ALUSrc = 1;
        RegWriteEn = true;
        break;

    case LW:
        aluop = 0;
        RegDst = 0;
        MemReadEn = true;
        MemtoReg = true;
        ALUSrc = 1;
        RegWriteEn = true;
        break;

    case SW:
        aluop = 0;
        MemWriteEn = true;
        ALUSrc = 1;
        break;

    case XORi:
        aluop = 5;
        RegDst = 0;
        ALUSrc = 1;
        RegWriteEn = true;
        break;

    case BEQ:
        Branch = true;
        ZERO = true;
        ALUSrc = 0;
        break;

    case BNE:
        Branch = true;
        ZERO = false;
        ALUSrc = 0;
        break;

    case SLTI:
        aluop = 6;
        RegDst = 0;
        ALUSrc = 1;
        RegWriteEn = true;
        break;

    case J:
        // No aluop needed for J
        RegDst = 0;
        break;

    case JAL:
        aluop = 2;
        RegDst = 2;
        JAL_signal = true;
        ALUSrc = 2;
        break;

    default:
        // Unsupported opcode, leave default values
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
