#include "Jump.h"

// Constructor
Jump::Jump() : decodeSemaphore(0),JmuxSel(0), AndGate(false), Flush(false), Instruction(0), JAL(false), Jaddress(0),PC4(0), Jr(false) {}

// Function to receive fetch stage input
void Jump::JumpInputF(int32_t instruction, int32_t pc4) {
    Instruction = instruction;
    PC4 = pc4;
    JumpUnitAddressOutput();
}

// Function to receive decode stage input
void Jump::JumpInputD(int32_t bAddress, int32_t rAddress, bool zANDb, bool jr) {
    Baddress = bAddress; 
    Raddress = rAddress;
    AndGate = zANDb;
    Jr = jr;
    decodeSemaphore.release(); // Signal decode input is ready
}

// Core jump logic function
void Jump::JumpUnitSignalsOutput() {
    // Wait until both inputs are set
    decodeSemaphore.acquire();

    int32_t instrOpcode = (Instruction & 0xFC000000) >> 26;
    if ((instrOpcode == 0x2) || (instrOpcode == 0x3)) {
        JAL = true;
    }
    else {
        JAL = false;
    }

    bool S0 = AndGate || ((!Jr) && JAL);
    bool S1 = Jr || AndGate;
    Flush = S1; 
    JmuxSel = (S1 << 1) | S0;

    // Release semaphore
    decodeSemaphore.release();
}


//called automatically when the Fetch instruction calls JumpSignalF
void Jump::JumpUnitAddressOutput() {
    int32_t instrAddress = (Instruction & 0x03FFFFFF);
    instrAddress <<= 2;
    int32_t upperPC = (PC4 & 0xF0000000);
    Jaddress = upperPC | instrAddress;

}