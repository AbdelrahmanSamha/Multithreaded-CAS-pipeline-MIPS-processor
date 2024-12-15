#include "Jump.h"
#include "ConsoleLogger.h"
// Constructor
Jump::Jump() : ExecuteSemaphore(0), JmuxSel(0), Is_Hit(false), Flush(false), Instruction(0), is_JumpIns(false), Jaddress(0), PC1(0), Jr(false) {}

// Function to receive fetch stage input
void Jump::JumpInputF(int32_t instruction, int32_t PC1) {
    this->Instruction = instruction;
    this->PC1 = PC1;
    JumpUnitAddressOutput();
}

// Function to receive decode stage input
void Jump::JumpInputEXE(int32_t bAddress, int32_t rAddress, bool Is_Hit, bool jr, bool Branch) {
    
    this->MissTargetAddress = bAddress; 
    this->Raddress = rAddress;
    this->Is_Hit = Is_Hit;
    this->Jr = jr;
    this->Branch = Branch;
    this->ExecuteSemaphore.release(); // Signal  input is ready
}

// Core jump logic function
void Jump::JumpUnitSignalsOutput() {
    // Wait until both inputs are set
    ExecuteSemaphore.acquire();
    
    Prediction = 0;
    

    int32_t instrOpcode = (Instruction & 0xFC000000) >> 26;

    // Check if the instruction is a jump instruction
    is_JumpIns = (instrOpcode == 0x2 || instrOpcode == 0x3);

    // Check if the instruction is a branch instruction
    is_BranchIns = (instrOpcode == 0x4 || instrOpcode == 0x5);

    // Check if the next instruction is either a branch or a jump
    is_nextBorJ = (is_BranchIns || is_JumpIns);

    bool is_backward = (BranchAddress < PC1);



    if (is_JumpIns) {
        UnitAddressOutput = Jaddress;
    }

    // Check if the instruction is a branch instruction
    if (is_BranchIns) {
        if (is_backward) {
            UnitAddressOutput = BranchAddress;
            Prediction = true;
        }
        else {
            UnitAddressOutput = PC1;
        }
    }


    bool S1 = Jr || (!Is_Hit && Branch);
    bool S0 = (!Jr && is_nextBorJ ) || (!Is_Hit && Branch);
    Flush = S1; 
    JmuxSel = (S1 << 1) | S0;
    ConsoleLog(1, "JMuxSel after exe inp=", JmuxSel);
    
}


//called automatically when the Fetch instruction calls JumpSignalF
void Jump::JumpUnitAddressOutput() {
    int32_t instrAddress = (Instruction & 0x03FFFFFF);
    int32_t upperPC = (PC1 & 0xF0000000);
    Jaddress = upperPC | instrAddress;

    int32_t Offset = (Instruction & 0x0000ffff);// Mask to get the lower 16 bits
 
    if (Offset & 0x00008000) {      // Check if the 16th bit is set
        Offset |= 0xFFFF0000;       // Extend the sign to upper 16 bits
    }

    BranchAddress = Offset + PC1;

}

