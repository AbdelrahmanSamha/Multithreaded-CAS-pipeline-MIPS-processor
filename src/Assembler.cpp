#include "Assembler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Hash tables for opcodes, functs, and registers (same as your original code)
std::unordered_map<std::string, uint8_t> opcodeMap = {
    {"add", 0x00},{"addu", 0x00}, {"sub", 0x00}, {"subu", 0x00}, {"and", 0x00}, {"or", 0x00}, {"nor", 0x00},
    {"sll", 0x00}, {"srl", 0x00}, {"jr", 0x00}, {"addi", 0x08}, {"lw", 0x23}, {"sw", 0x2B}, {"beq", 0x04}, {"bne", 0x05},
    {"j", 0x02}, {"jal", 0x03}, {"bltz",0x01},{"bgez",0x01},{"xor", 0x00}, {"xori", 0x0E}, {"ori", 0x0D}, {"slti",0x0A},{"slt",0x00}
};

std::unordered_map<std::string, uint8_t> functMap = {
    {"add", 0x20}, {"addu", 0x21}, {"sub", 0x22}, {"subu", 0x23}, {"and", 0x24}, {"or", 0x25}, {"nor", 0x27},
    {"sll", 0x00}, {"srl", 0x02}, {"jr", 0x08}, {"xor",0x26}, {"slt",0x2A}
};

std::unordered_map<std::string, uint8_t> registerMap = {
    {"$zero", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3}, {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31}
};

std::unordered_map<std::string, uint8_t> rtMap = {
    {"bltz", 0x00},
    {"bgez", 0x01},
};

std::unordered_map<std::string, uint32_t> labelMap;
uint32_t currentAddress = 0x00400000;

Assembler::Assembler(const std::string& inputFileName, const std::string& outputFileName)
    : inputFileName(inputFileName), outputFileName(outputFileName), currentAddress(0x00400000) {}

void Assembler::assemble() {
    // First pass: record labels
    firstPass();

    // Second pass: assemble instructions
    secondPass();
}

void Assembler::firstPass() {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file: " << inputFileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        line = trimWhitespace(line);
        if (line.empty()) { // Skip empty lines and comments
            continue;
        }

        // Here you would parse the line to identify labels and add them to the labelTable
        // For example:
        if (line.back() == ':') { // Simple label detection
            std::string label = line.substr(0, line.size() - 1);
            labelMap[label] = currentAddress;
        }
        else {
            currentAddress += 4; // Increment address for the next instruction (assuming 4 bytes per instruction)
        }
    }

    inputFile.close();
}


void Assembler::secondPass() {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName, std::ios::trunc);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files.\n";
        return;
    }

    currentAddress = 0x00400000; // Reset current address for the second pass
    std::string line;
    while (getline(inputFile, line)) {
        uint32_t machineCode = assembleInstruction(line);
        if (machineCode == 0xDEADBEEF) {
            std::cerr << "Error assembling line: " << line << "\n";

        }

        else {
            // Create and store the instruction
            Instruction instr = { currentAddress, machineCode, line };
            instructionSet.emplace_back(instr);
            writeHexToAssembledFile(instr);
            currentAddress += 4;
        }
    }

    inputFile.close();
    outputFile.close();
}

uint32_t Assembler::assembleInstruction(const std::string& instruction) {
    std::istringstream iss(trimWhitespace(instruction));
    std::string op, rd, rs, rt, label, immediateStr, address;
    uint8_t shamt = 0;
    int16_t immediate = 0;


    iss >> op;
    auto opcodeIt = opcodeMap.find(op);
    if (opcodeIt == opcodeMap.end()) return 0xDEADBEEF;

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

    else if (opcode == 0x01) { // Special I-type for bltz, bgez
        if (!(iss >> rs)) {
            return 0xDEADBEEF;
        }
        rs = trimWhitespace(rs);
        if (rs.back() == ',') {
            rs.pop_back();
        }

        if (!(iss >> label) || labelMap.find(label) == labelMap.end()) return 0xDEADBEEF;

        int16_t offset = (labelMap[label] - currentAddress - 4) >> 2;
        uint8_t rt = rtMap[op];

        return (opcode << 26) | (registerMap[rs] << 21) | (rt << 16) | (offset & 0xFFFF);
    }

    else if (opcode == 0x02 || opcode == 0x03) { // J-type instructions
        if (!(iss >> label) || labelMap.find(label) == labelMap.end()) return 0xDEADBEEF;
        uint32_t labelAddress = labelMap[label] >> 2;

        return (opcode << 26) | (labelAddress & 0x03FFFFFF);
    }

    else if (opcode == 0x04 || opcode == 0x05) { // Branch instructions
        if (!(iss >> rs)) {
            return 0xDEADBEEF; // Return error if rs is not found
        }

        rs = trimWhitespace(rs);
        if (rs.back() == ',') {
            rs.pop_back(); // Remove the trailing comma if present
        }

        // Read the second register
        if (!(iss >> rt)) {
            return 0xDEADBEEF; // Return error if rt is not found
        }

        rt = trimWhitespace(rt);
        if (rt.back() == ',') {
            rt.pop_back(); // Remove the trailing comma if present
        }

        // Read the label
        if (!(iss >> label)) {
            return 0xDEADBEEF; // Return error if label is not found
        }

        label = trimWhitespace(label);
        if (label.back() == ',') {
            label.pop_back(); // Remove the trailing comma if present
        }

        auto labelIt = labelMap.find(label);
        if (labelIt == labelMap.end()) {
            return 0xDEADBEEF; // Return error if label is not found
        }
        int16_t offset = (labelMap[label] - currentAddress - 4) >> 2;

        return (opcode << 26) | (registerMap[rs] << 21) | (registerMap[rt] << 16) | (offset & 0xFFFF);
    }

    else if (opcode >= 0x08 && opcode <= 0x0E) { // I-type instructions with immediate values
        if (!(iss >> rt))  return 0xDEADBEEF;

        rt = trimWhitespace(rt);
        if (rt.back() == ',') {
            rt.pop_back();
        }

        if (!(iss >> rs))  return 0xDEADBEEF;


        rs = trimWhitespace(rs);
        if (rs.back() == ',') {
            rs.pop_back();
        }

        if (!(iss >> immediateStr))  return 0xDEADBEEF;


        immediateStr = trimWhitespace(immediateStr);
        if (immediateStr.back() == ',') {
            immediateStr.pop_back();
        }

        immediate = std::stoi(immediateStr);
        // Ensure immediate is within valid 16-bit signed integer range
        if (immediate < -32768 || immediate > 32767) {
            return 0xDEADBEEF;
        }


        uint32_t machineCode = (opcode << 26) |
            (registerMap[rs] << 21) |
            (registerMap[rt] << 16) |
            (immediate & 0xFFFF);

        return machineCode;
    }
    else if (opcode == 0x23 || opcode == 0x2B) { // sw or lw
        if (!(iss >> rt)) {
            return 0xDEADBEEF; // Error if rt is not found
        }
        rt = trimWhitespace(rt);
        if (rt.back() == ',') {
            rt.pop_back(); // Remove the trailing comma if present
        }

        // Read the offset and base register (in offset(base) format)
        if (!(iss >> address)) {
            return 0xDEADBEEF; // Error if address (offset + base) is not found
        }
        address = trimWhitespace(address);

        // Find the offset and base register by parsing the address
        size_t start = address.find('(');
        size_t end = address.find(')');
        if (start == std::string::npos || end == std::string::npos || end <= start + 1) {
            return 0xDEADBEEF; // Error if format is incorrect
        }

        // Parse offset and base register
        std::string offsetStr = address.substr(0, start);
        std::string baseRegister = address.substr(start + 1, end - start - 1);

        // Trim and convert the offset
        offsetStr = trimWhitespace(offsetStr);
        baseRegister = trimWhitespace(baseRegister);
        int16_t offset = static_cast<int16_t>(std::stoi(offsetStr));
        int baseRegisterNumber = std::stoi(baseRegister.substr(1)); // Assuming "baseRegister" is in "$X" format, like "$4"

        // Encode the machine code for lw/sw
        uint32_t machineCode = (opcode << 26) |
            (baseRegisterNumber << 21) | // Directly using baseRegisterNumber as rs
            (std::stoi(rt.substr(1)) << 16) | // Assuming "rt" is in "$X" format, like "$5"
            (offset & 0xFFFF); // Offset value               // Offset value

        return machineCode;
    }



    return 0xDEADBEEF; // Error
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


const std::vector<Instruction>& Assembler::getInstructions() const {
    return instructionSet;

}
/*andi    jr   lw     sw*/
/*sw/ lw -> instruction destination offset(source)
            opcode  source   destination  offset extended
            kkkkk
*/
