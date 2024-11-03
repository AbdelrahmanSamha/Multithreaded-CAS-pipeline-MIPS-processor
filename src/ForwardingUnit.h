#pragma once
#ifndef FORWARDING_UNIT_H
#define FORWARDING_UNIT_H

#include <cstdint>
#include <semaphore>
class ForwardingUnit {
private:
    std::binary_semaphore MEMsemaphore, WBsemaphore;
    // Forwarding signals initialized to 0 by default
    

    bool EXEMEM_RegWrite;
    int32_t EXEMEM_Rd;

    bool MEMWB_RegWrite;
    int32_t MEMWB_Rd;

    int32_t IDEXE_Rs= 0;
    int32_t IDEXE_Rt=0;
public:
    int32_t MEMdata, WBdata;
    int32_t ForwardA, ForwardB;
public:
    // Constructor
    ForwardingUnit();

    // Function to evaluate forwarding logic
    void FUinputEXE(int32_t IDEXE_Rs, int32_t IDEXE_Rt);
    void FUinputMEM(bool EXEMEM_RegWrite, int32_t EXEMEM_Rd, int32_t MEMdata);
    void FUinputWB(bool MEMWB_RegWrite, int32_t MEMWB_Rd, int32_t WBdata);

    void evaluateForwarding();
};

#endif // FORWARDING_UNIT_H
