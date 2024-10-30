#pragma once
#ifndef HAZARDDETECTION_H
#define HAZARDDETECTION_H

#include <cstdint>

class HazardDetection {
public:
    // Constructor
    HazardDetection();

    // Inputs
    void setInputExecute(uint8_t ID_EX_Rt, bool ID_EX_MemRead);
    void setInputDecode(uint8_t IF_ID_Rs, uint8_t IF_ID_Rt);
    
    // Outputs
    bool getPCWrite() const;
    bool getIFID_Stall() const;
    bool getNOP() const;

    // Perform hazard detection
    void detectHazard();

private:
    // Inputs
    uint8_t IFID_Rs;
    uint8_t IFID_Rt;
    uint8_t IDEX_Rt;
    bool IDEX_MemRead;

    // Outputs
    bool PC_Write;
    bool IFID_stall;
    bool NOP;
};

#endif // HAZARDDETECTION_H
