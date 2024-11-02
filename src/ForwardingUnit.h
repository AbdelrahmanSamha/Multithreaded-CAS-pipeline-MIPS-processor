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
    uint8_t EXEMEM_Rd;

    bool MEMWB_RegWrite;
    uint8_t MEMWB_Rd;

    uint8_t IDEXE_Rs= 0;
    uint8_t IDEXE_Rt=0;
public:
    uint32_t MEMdata, WBdata;
    uint8_t ForwardA, ForwardB;
public:
    // Constructor
    ForwardingUnit();

    // Function to evaluate forwarding logic
    void FUinputEXE(uint8_t IDEXE_Rs, uint8_t IDEXE_Rt);
    void FUinputMEM(bool EXEMEM_RegWrite, uint8_t EXEMEM_Rd, uint32_t MEMdata);
    void FUinputWB(bool MEMWB_RegWrite, uint8_t MEMWB_Rd, uint32_t WBdata);

    void evaluateForwarding();
};

#endif // FORWARDING_UNIT_H
