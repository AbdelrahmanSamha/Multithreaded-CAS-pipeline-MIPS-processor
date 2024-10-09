#pragma once
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <unordered_map>

// Instruction struct to hold address, machine code, and assembly code details
struct Instruction {
    uint32_t address;
    uint32_t machineCode;
    std::string basicCode;
};

class Assembler {
private:
    std::string inputFileName;
    std::string outputFileName;
    std::vector<Instruction> instructionSet; // Stores all the instructions with details
    uint32_t currentAddress;                 // Keeps track of instruction addresses

    // Helper functions
    std::string trimWhitespace(const std::string& str);
    void writeHexToAssembledFile(const Instruction& instr);

public:
    Assembler(const std::string& inputFileName, const std::string& outputFileName);

    void assemble();                         // Main function to assemble the file
    uint32_t assembleInstruction(const std::string& instruction);

    const std::vector<Instruction>& getInstructions() const; // Fetch stage will need this
};

#endif // ASSEMBLER_H
