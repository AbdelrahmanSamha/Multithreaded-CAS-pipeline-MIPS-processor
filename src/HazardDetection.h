#pragma once
#ifndef HAZARDDETECTION_H
#define HAZARDDETECTION_H

#include <cstdint>
#include <semaphore>
class HazardDetection {
public:
    // Constructor
    HazardDetection();

    // Inputs
    void HDUinputExecute(int32_t ID_EX_Rt, bool ID_EX_MemRead);
    void HDUinputDecode(int32_t IF_ID_Rs, int32_t IF_ID_Rt);
    
    // Outputs
    bool getPCWrite() const;
    bool getIFID_Stall() const;
    bool getNOP() const;

    // Perform hazard detection
    void detectHazard();

private:
    std::binary_semaphore EXEsemaphore;
    // Inputs
    int32_t IFID_Rs;
    int32_t IFID_Rt;
    int32_t IDEX_Rt;
    bool IDEX_MemRead;

    // Outputs
    bool PC_Write;
    bool IFID_stall;
    bool NOP;
};

#endif // HAZARDDETECTION_H
