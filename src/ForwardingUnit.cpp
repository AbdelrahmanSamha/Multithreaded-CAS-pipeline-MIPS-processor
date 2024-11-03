#include "ForwardingUnit.h"
#include <iostream>
ForwardingUnit::ForwardingUnit() : ForwardA(0), ForwardB(0), MEMsemaphore(0),WBsemaphore(0) {}

void ForwardingUnit::evaluateForwarding()
{
    MEMsemaphore.acquire();
    WBsemaphore.acquire();
    // Default forwarding: No forwarding
    ForwardA = 0b00;
    ForwardB = 0b00;

    // Forwarding logic for ForwardA
    if (EXEMEM_RegWrite && EXEMEM_Rd != 0 && EXEMEM_Rd == IDEXE_Rs) {
        ForwardA = 0b10;
    }

    if (MEMWB_RegWrite && MEMWB_Rd != 0 && MEMWB_Rd == IDEXE_Rs && EXEMEM_Rd != IDEXE_Rs) {
        ForwardA = 0b01;
    }

    // Forwarding logic for ForwardB
    if (EXEMEM_RegWrite && EXEMEM_Rd != 0 && EXEMEM_Rd == IDEXE_Rt) {
        ForwardB = 0b10;
    }

    if (MEMWB_RegWrite && MEMWB_Rd != 0 && MEMWB_Rd == IDEXE_Rt && EXEMEM_Rd != IDEXE_Rt) {
        ForwardB = 0b01;
    }
}

void ForwardingUnit::FUinputEXE(int32_t EXErs, int32_t EXErt) {
    if (this == nullptr) {
        std::cerr << "Error: Attempted to call FUinputEXE on a null pointer!" << std::endl;
        return;
    }
    this->IDEXE_Rs = EXErs;
    this->IDEXE_Rt = EXErt;
}
void ForwardingUnit::FUinputMEM(bool EXEMEM_RegWrite, int32_t EXEMEM_Rd, int32_t MEMdata) {
    this->MEMdata = MEMdata; //this has nothing to do with Forwarding Logic, Its just to pass data to the Alu Mux.
                             //##Can be moved to another function for better clarity,But requires further syncronization,##
    this->EXEMEM_RegWrite = EXEMEM_RegWrite;
    this->EXEMEM_Rd = EXEMEM_Rd;
    MEMsemaphore.release();
}
void ForwardingUnit::FUinputWB(bool MEMWB_RegWrite, int32_t MEMWB_Rd, int32_t WBdata) {
    this->WBdata = WBdata;//this has nothing to do with Forwarding Logic, Its just to pass data to the Alu Mux.
                          //##Can be moved to another function for better clarity,But requires further syncronization,##
    this->MEMWB_RegWrite = MEMWB_RegWrite;
    this->MEMWB_Rd = MEMWB_Rd;
    WBsemaphore.release();
}
