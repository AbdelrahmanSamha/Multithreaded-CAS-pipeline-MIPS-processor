#pragma once
#ifndef HAZARDDETECTION_H
#define HAZARDDETECTION_H

#include <cstdint>
#include <semaphore>
class HazardDetection {

private:
    std::binary_semaphore EXEsemaphore;
    // Inputs
    bool JRSIGNAL;
    bool ANDGATE;

    // Output
    bool NOP;

public:
    // Constructor
    HazardDetection();

    // Inputs
    void HDUinputExecute(bool JRSIGNAL, bool ANDGATE);
    
    
    // Output
    bool getNOP() const;

    // Perform hazard detection
    void detectHazard();


};

#endif // HAZARDDETECTION_H
