#include "Assembler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// Defining the hash tables for opcodes, functs, and registers
std::unordered_map<std::string, uint8_t> opcodeMap = {
    {"add", 0x00},{"addu", 0x00}, {"sub", 0x00}, {"subu", 0x00}, {"and", 0x00}, {"or", 0x00}, {"nor", 0x00},
    {"sll", 0x00}, {"srl", 0x00}, {"jr", 0x00}, {"addi", 0x08}, {"lw", 0x23}, {"sw", 0x2B}, {"beq", 0x04}, {"bne", 0x05},
    {"j", 0x02}, {"jal", 0x03}
};

std::unordered_map<std::string, uint8_t> functMap = {
    {"add", 0x20}, {"addu", 0x21}, {"sub", 0x22}, {"subu", 0x23}, {"and", 0x24}, {"or", 0x25}, {"nor", 0x27},
    {"sll", 0x00}, {"srl", 0x02}, {"jr", 0x08}
};

std::unordered_map<std::string, uint8_t> registerMap = {
    {"$zero", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3}, {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31}
};

Assembler::Assembler(const std::string& inputFileName, const std::string& outputFileName)
    : inputFileName(inputFileName), outputFileName(outputFileName), currentAddress(0x00400000) {}

void Assembler::assemble() {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName, std::ios::trunc);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files.\n";
        return;
    }

    std::string line;
    while (getline(inputFile, line)) {
        uint32_t machineCode = assembleInstruction(line);
        if (machineCode == 0xDEADBEEF) {
            std::cerr << "Error assembling line: " << line << "\n";

        }

        // Create the instruction struct
        Instruction instr = {
            currentAddress,   // Address of the instruction
            machineCode,      // Machine code
            line             // Basic assembly code (source code)
        };

        instructionSet.emplace_back(instr); // Store the instruction

        // Write the instruction details to file
        writeHexToAssembledFile(instr);

        currentAddress += 4; // Increment the address (MIPS instructions are 4 bytes apart)
    }

    inputFile.close();
    outputFile.close();
}

uint32_t Assembler::assembleInstruction(const std::string& instruction) {
    std::istringstream iss(trimWhitespace(instruction));
    std::string op, rd, rs, rt;
    uint8_t shamt = 0;

    iss >> op;
    auto opcodeIt = opcodeMap.find(op);
    if (opcodeIt == opcodeMap.end()) {
        return 0xDEADBEEF;
    }

    uint8_t opcode = opcodeIt->second;

    if (opcode == 0x00) {
        if (!(iss >> rd)) {
            return 0xDEADBEEF;
        }
        rd = trimWhitespace(rd);
        if (rd.back() == ',') {
            rd.pop_back();
        }

        if (op == "sll" || op == "srl") {
            iss >> rt;
            rt = trimWhitespace(rt);
            if (rt.back() == ',') {
                rt.pop_back();
            }
            std::string shamtStr;
            iss >> shamtStr;
            shamt = std::stoi(shamtStr);
        }
        else {
            std::string temp;
            while (std::getline(iss, temp, ',')) {
                temp = trimWhitespace(temp);
                if (rs.empty()) {
                    rs = temp;
                }
                else if (rt.empty()) {
                    rt = temp;
                }
                else {
                    return 0xDEADBEEF;
                }
            }
        }

        uint32_t machineCode = (opcode << 26) |
            (registerMap[rs] << 21) |
            (registerMap[rt] << 16) |
            (registerMap[rd] << 11) |
            (shamt << 6) |
            functMap[op];

        return machineCode;
    }

    // Handle other instruction types here (I-type, J-type)
    return 0xDEADBEEF; // Default return value for unrecognized instruction
}

void Assembler::writeHexToAssembledFile(const Instruction& instr) {
    std::ofstream outputFile(outputFileName, std::ios::app); // Open in append mode
    if (outputFile.is_open()) {
        outputFile << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << instr.address << " "
            << std::setw(8) << instr.machineCode << "  " << instr.basicCode << "\n";
    }
    else {
        std::cerr << "Error writing to output file.\n";
    }
}

std::string Assembler::trimWhitespace(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Get instructions to allow Fetch stage to retrieve machine code
const std::vector<Instruction>& Assembler::getInstructions() const {
    return instructionSet;
}
