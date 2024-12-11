#include "ForwardingUnit.h"
#include <iostream>
ForwardingUnit::ForwardingUnit()
    : ForwardA(0), ForwardB(0), ForwardC(0), ForwardD(0), ForwardE(0),
    MEMsemaphore(0), EXEsemaphore(0), HelperSemaphore(0),
    IFID_Rs(0), IFID_Rt(0), IFID_JalSignal(false), IFID_AluSrc(false),
    IDEXE_Rs(0), IDEXE_Rt(0), IDEXE_Rd(0), IDEXE_RegWrite(false), IDEXE_MemRead(false),
    EXEMEM_Rd(0), EXEMEM_RegWrite(false), EXEMEM_MemRead(false),
    MEMWB_RegWrite(false),
    AluResult(0), MEMAddress(0), MEMreaddata(0) {}

void ForwardingUnit::evaluateForwarding()
{
    EXEsemaphore.acquire();
    MEMsemaphore.acquire();
    // Default forwarding: No forwarding
    ForwardA = 0b00;
    ForwardB = 0b00;
    ForwardC = 0b00;
    ForwardD = 0b00;
    ForwardE = 0b00;


    // Forwarding logic for ForwardA
    if ((IFID_Rs == IDEXE_Rd) && IDEXE_RegWrite && IDEXE_Rd != 0) {
        ForwardA = 0b01;
    }
    else if ((IFID_Rs == EXEMEM_Rd) && EXEMEM_RegWrite && !EXEMEM_MemRead && EXEMEM_Rd != 0) {
        ForwardA = 0b10;
    }
    else if ((IFID_Rs == IDEXE_Rd) && IDEXE_MemRead && IDEXE_Rd != 0) {
        ForwardA = 0b11;
    }



    // Forwarding logic for ForwardB
    if ((IFID_Rt == IDEXE_Rd) && IDEXE_RegWrite && IDEXE_Rd != 0) {
        ForwardB = 0b01;
    }
    else if ((IFID_Rt == EXEMEM_Rd) && EXEMEM_RegWrite && !EXEMEM_MemRead && EXEMEM_Rd != 0) {
        ForwardB = 0b10;
    }
    else if ((IFID_Rt == EXEMEM_Rd) && EXEMEM_MemRead && EXEMEM_Rd != 0) {
        ForwardB = 0b11;
    }


    // Forward C and D logic
    if (IFID_JalSignal) {
        ForwardC = 0b01;
    }

    if (IFID_AluSrc) {
        ForwardD = 0b01;
    }
    else {
        if (IFID_Rt == IDEXE_Rd && IDEXE_MemRead && IDEXE_Rd != 0) {
            ForwardD = 0b10;
        }
    }

    if (IFID_Rs == IDEXE_Rd && IDEXE_MemRead && IDEXE_Rd != 0) {
        ForwardC = 0b10;
    }

    // Forward E logic
    if (IDEXE_Rt == EXEMEM_Rd && EXEMEM_RegWrite && !EXEMEM_MemRead && EXEMEM_Rd != 0) {
        ForwardE = 0b01;
    }
    if (IDEXE_Rt == EXEMEM_Rd && EXEMEM_MemRead && EXEMEM_Rd != 0) {
        ForwardE = 0b10;
    }
    HelperSemaphore.release();
}

void ForwardingUnit::FUinputDecode(int32_t IFID_Rs, int32_t IFID_Rt, bool IFID_JalSignal, bool IFID_AluSrc) {
    this->IFID_Rs = IFID_Rs;
    this->IFID_Rt = IFID_Rt; 
    this->IFID_JalSignal = IFID_JalSignal; 
    this->IFID_AluSrc = IFID_AluSrc; 
}

void ForwardingUnit::FUinputEXE(int32_t AluResult, int32_t IDEXE_Rs, int32_t IDEXE_Rt, int32_t IDEXE_Rd, bool IDEXE_RegWrite, bool IDEXE_MemRead) {
    if (this == nullptr) {
        std::cerr << "Error: Attempted to call FUinputEXE on a null pointer!" << std::endl;
        return;
    }
    this->AluResult = AluResult; //this has nothing to do with Forwarding Logic, Its just to pass data to the Alu Mux.
    this->IDEXE_Rs = IDEXE_Rs;
    this->IDEXE_Rt = IDEXE_Rt;
    this->IDEXE_Rd= IDEXE_Rd; 
    this->IDEXE_RegWrite = IDEXE_RegWrite;
    this->IDEXE_MemRead = IDEXE_MemRead;
    EXEsemaphore.release();
   
}
void ForwardingUnit::FUinputMEM(int32_t MEMAddress, int32_t MEMreaddata, int32_t EXEMEM_Rd, bool EXEMEM_RegWrite, bool EXEMEM_MemRead) {
    this->MEMAddress = MEMAddress; //this has nothing to do with Forwarding Logic, Its just to pass data to the Alu Mux.
    this->MEMreaddata = MEMreaddata; 
                                   //##Can be moved to another function for better clarity,But requires further syncronization,##
    this->EXEMEM_Rd = EXEMEM_Rd;
    this->EXEMEM_RegWrite = EXEMEM_RegWrite;
    this->EXEMEM_MemRead = EXEMEM_MemRead;
    MEMsemaphore.release();
}

int32_t ForwardingUnit::ForwardEget() {
    HelperSemaphore.acquire();//so we dont get a wrong ForwardE value, as we need the one after evaluating is done.
    return ForwardE;
}
