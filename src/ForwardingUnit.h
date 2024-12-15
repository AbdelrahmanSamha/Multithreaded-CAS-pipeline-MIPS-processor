#pragma once
#ifndef FORWARDING_UNIT_H
#define FORWARDING_UNIT_H

#include <cstdint>
#include <semaphore>
class ForwardingUnit {
private:
    std::binary_semaphore MEMsemaphore,HelperSemaphore, EXEsemaphore,wait1Sem ;
    // Forwarding signals initialized to 0 by default
    //DECODE STAGE INPUTS
    int32_t IFID_Rs;
    int32_t IFID_Rt;
    bool IFID_JalSignal;
    bool IFID_AluSrc ;
    //EXECUTE STAGE INPUTS
    int32_t IDEXE_Rs ;
    int32_t IDEXE_Rt ;
    int32_t IDEXE_Rd ;
    bool IDEXE_RegWrite ;
    bool IDEXE_MemRead;

    //MEMORY STAGE INPUTS
    int32_t EXEMEM_Rd ;

    bool EXEMEM_RegWrite;
    bool MEMWB_RegWrite ;
    bool EXEMEM_MemRead ;

public:
    //We need to pass the values of the ALU, the address from the EXEMEMpipe, and the readdata from the memory through the FORWARDING UNIT.
    int32_t AluResult ;
    int32_t MEMAddress ;
    int32_t MEMreaddata ;
    int32_t ForwardA, ForwardB, ForwardC, ForwardD, ForwardE;
public:
    // Constructor
    ForwardingUnit();

    // Function to evaluate forwarding logic
    void FUinputDecode(int32_t IFID_Rs, int32_t IFID_Rt, bool IFID_JalSignal,bool IFID_AluSrc);
    void FUinputEXE(int32_t AluResult,int32_t IDEXE_Rs, int32_t IDEXE_Rt, int32_t IDEXE_Rd, bool IDEXE_RegWrite, bool IDEXE_MemRead);
    void FUinputMEM(int32_t MEMAddress, int32_t MEMreaddata, int32_t EXEMEM_Rd,  bool EXEMEM_RegWrite, bool EXEMEM_MemRead);
    int32_t ForwardEget();
    void WaitForMemoryInput();
    void evaluateForwarding();
};

#endif // FORWARDING_UNIT_H
