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
    {"add", 0x00},{"addu", 0x00}, {"sub", 0x00}, {"subu", 0x00}, {"and", 0x00}, {"or", 0x00}, {"nor", 0x00},{"andi",0x0C},
    {"sll", 0x00}, {"srl", 0x00}, {"jr", 0x00}, {"addi", 0x08}, {"lw", 0x23}, {"sw", 0x2B}, {"beq", 0x04}, {"bne", 0x05},{"sgt", 0x00},
    {"j", 0x02}, {"jal", 0x03}, {"xor", 0x00}, {"xori", 0x0E}, {"ori", 0x0D}, {"slti",0x0A},{"slt",0x00}
};

std::unordered_map<std::string, uint8_t> functMap = {
    {"add", 0x20}, {"addu", 0x21}, {"sub", 0x22}, {"subu", 0x23}, {"and", 0x24}, {"or", 0x25}, {"nor", 0x27},
    {"sll", 0x00}, {"srl", 0x02}, {"jr", 0x08}, {"xor",0x26}, {"slt",0x2A}, {"sgt", 0x2C}
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

    thirdPass();
    // First pass: record labels
    firstPass();

    // Second pass: assemble instructions
    secondPass();
}
void Assembler::thirdPass() {
    // Read the file into memory
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file: " << inputFileName << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inputFile, line)) {
        line = trimWhitespace(line);
        std::istringstream iss(line);
        std::string op, format, rs, label;

        iss >> op;
        if (op == "bltz" || op == "bgez") {
            // Extract register and label
            std::getline(iss, rs, ',');
            rs = trimWhitespace(rs);
            std::getline(iss, label);
            label = trimWhitespace(label);

            // Remove trailing commas if present
            if (rs.back() == ',') rs.pop_back();
            if (label.back() == ',') label.pop_back();

            // Generate replacement instructions
            std::string line_SLT = "slt $30, " + rs + ", $zero";
            std::string line_branch;

            if (op == "bltz") {
                line_branch = "bne $30, $zero, " + label;
            }
            else { // bgez
                line_branch = "beq $30, $zero, " + label;
            }

            // Add the replacement instructions to the lines
            lines.push_back(line_SLT);
            lines.push_back(line_branch);
        }
        else {
            // Add the original line
            lines.push_back(line);
        }
    }
    inputFile.close();

    // Write updated content back to the same file
    std::ofstream outputFile(inputFileName, std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the file for writing!" << std::endl;
        return;
    }

    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << std::endl;
    }
    outputFile.close();
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

        if (!line.empty() && line.back() == ':') { // Simple label detection
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


    if (opcode == 0x00) { // R-type
        return assembleRTypeInstruction(iss, op);
    }
    else if (opcode == 0x02 || opcode == 0x03) { // J-type
        return assembleJTypeInstruction(iss, opcode);
    }
    else if (opcode == 0x04 || opcode == 0x05) { // Branch instructions
        return assembleBranchInstruction(iss, opcode);
    }
    else if (opcode >= 0x08 && opcode <= 0x0E) { // I-type instructions
        return assembleITypeInstruction(iss, opcode);
    }
    else if (opcode == 0x23 || opcode == 0x2B) { // LW & SW
        return assembleLoadStore(iss, opcode);
    }
    return 0xDEADBEEF; // If none of the above cases match
}

void Assembler::writeHexToAssembledFile(const Instruction& instr) {
    std::ofstream outputFile(outputFileName, std::ios::app);
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

uint32_t Assembler::assembleRTypeInstruction(std::istringstream& iss, const std::string& op) {
    std::string rd, rs, rt;
    uint8_t shamt = 0;

    if (op == "jr") {

        if (!(iss >> rs)) {
            return 0xDEADBEEF;
        }
        rs = trimWhitespace(rs);
        if (rs.back() == ',') {
            rs.pop_back();
        }
        return (0 << 26) |
            (registerMap[rs] << 21) |
            (0 << 16) |
            (0 << 11) |
            (0 << 6) |
            functMap[op];

    }
    //if (!(iss>>rd))
    if (!(std::getline(iss, rd, ','))) {
        return 0xDEADBEEF;
    }
    rd = trimWhitespace(rd);
    if (rd.back() == ',') {
        rd.pop_back();
    }

    if (op == "sll" || op == "srl") {
        //iss >> rs
        std::getline(iss, rs, ',');
        rs = trimWhitespace(rs);
        if (rs.back() == ',') rs.pop_back();
        std::string shamtStr;
        iss >> shamtStr;
        shamt = std::stoi(shamtStr);
        return (0 << 26) |
            (registerMap[rs] << 21) |
            (registerMap[rt] << 16) |
            (registerMap[rd] << 11) |
            (shamt << 6) |
            functMap[op];
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
        return  (00 << 26) |
            (registerMap[rs] << 21) |
            (registerMap[rt] << 16) |
            (registerMap[rd] << 11) |
            (shamt << 6) |
            functMap[op];

    }


}


uint32_t Assembler::assembleJTypeInstruction(std::istringstream& iss, uint8_t opcode) {
    std::string label;
    if (!(iss >> label) || labelMap.find(label) == labelMap.end()) return 0xDEADBEEF;
    uint32_t labelAddress = labelMap[label] >> 2;
    return (opcode << 26) | (labelAddress & 0x03FFFFFF);
}


uint32_t Assembler::assembleBranchInstruction(std::istringstream& iss, uint8_t opcode) {
    std::string rs, rt, label;
    //if (!(iss >> rs) || !(iss >> rt) || !(iss >> label)) return 0xDEADBEEF;
    std::getline(iss, rs, ',');
    rs = trimWhitespace(rs);
    if (rs.back() == ',') rs.pop_back();
    std::getline(iss, rt, ',');
    rt = trimWhitespace(rt);
    if (rt.back() == ',') rt.pop_back();
    std::getline(iss, label, ',');
    label = trimWhitespace(label);
    if (label.back() == ',') label.pop_back();

    auto labelIt = labelMap.find(label);
    if (labelIt == labelMap.end()) return 0xDEADBEEF;

    int16_t offset = (labelMap[label] - currentAddress - 4) >> 2;
    return (opcode << 26) |
        (registerMap[rs] << 21) |
        (registerMap[rt] << 16) |
        (offset & 0xFFFF);
}



uint32_t Assembler::assembleITypeInstruction(std::istringstream& iss, uint8_t opcode) {
    std::string rt, rs, immediateStr;
    //if (!(iss >> rt) || !(iss >> rs) || !(iss >> immediateStr)) return 0xDEADBEEF;
    std::getline(iss, rt, ',');
    rt = trimWhitespace(rt);
    if (rt.back() == ',') rt.pop_back();
    std::getline(iss, rs, ',');
    rs = trimWhitespace(rs);
    if (rs.back() == ',') rs.pop_back();
    std::getline(iss, immediateStr, ',');
    int16_t immediate = std::stoi(trimWhitespace(immediateStr));
    if (immediate < -32768 || immediate > 32767) return 0xDEADBEEF;

    return (opcode << 26) |
        (registerMap[rs] << 21) |
        (registerMap[rt] << 16) |
        (immediate & 0xFFFF);
}

uint32_t Assembler::assembleLoadStore(std::istringstream& iss, uint8_t opcode) {
    std::string rt, offsetAndRs;
    //if (!(iss >> rt) || !(iss >> offsetAndRs)) return 0xDEADBEEF;

    std::getline(iss, rt, ',');
    rt = trimWhitespace(rt);
    if (rt.back() == ',') rt.pop_back();
    std::getline(iss, offsetAndRs, ',');
    // Extract offset and rs from the format offset($rs)
    size_t openParen = offsetAndRs.find('(');
    size_t closeParen = offsetAndRs.find(')');
    if (openParen == std::string::npos || closeParen == std::string::npos) return 0xDEADBEEF;

    std::string offsetStr = offsetAndRs.substr(0, openParen);
    std::string rs = offsetAndRs.substr(openParen + 1, closeParen - openParen - 1);


    int16_t offset = std::stoi(trimWhitespace(offsetStr));
    if (offset < -32768 || offset > 32767) return 0xDEADBEEF;

    // Assemble the instruction
    return (opcode << 26) |
        (registerMap[rs] << 21) |
        (registerMap[rt] << 16) |
        (offset & 0xFFFF);
}


/*
add $s0, $s1, $s2
op   rd  rs   rt
 010E602C          sgt $t4, $t0, $t6

 0000 0001 0000 1110 0110 0000 0010 1100
 sgt done
*/