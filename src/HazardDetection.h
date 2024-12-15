#pragma once
#ifndef HAZARDDETECTION_H
#define HAZARDDETECTION_H

#include <cstdint>
#include <semaphore>
class HazardDetection {

private:
    std::binary_semaphore EXEsemaphore;
    // Inputs
    bool Is_JR;
    bool Is_Hit;
    bool Is_Branch;
    // Output
    bool NOP;

public:
    // Constructor
    HazardDetection();

    // Inputs
    void HDUinputExecute(bool Is_JR, bool Is_Hit, bool Is_Branch);
    
    
    // Output
    bool getNOP() const;

    // Perform hazard detection
    void detectHazard();


};

#endif // HAZARDDETECTION_H
