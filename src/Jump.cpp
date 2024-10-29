#include "Jump.h"

// Constructor
Jump::Jump() : fetchSemaphore(0), decodeSemaphore(0) {}

// Function to receive fetch stage input
void Jump::JumpSignalF(uint32_t instruction, uint32_t pc4) {
    Instruction = instruction;
    PC4 = pc4;
    fetchSemaphore.release(); // Signal fetch input is ready
}

// Function to receive decode stage input
void Jump::JumpSignalD(bool zANDb, bool jr) {
    AndGate = zANDb;
    Jr = jr;
    decodeSemaphore.release(); // Signal decode input is ready
}

// Core jump logic function
void Jump::JumpUnit() {

    // Wait until both inputs are set
    fetchSemaphore.acquire();
    decodeSemaphore.acquire();
    


}
