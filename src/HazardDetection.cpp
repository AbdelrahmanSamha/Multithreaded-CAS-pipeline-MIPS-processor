#include "HazardDetection.h"

// Constructor: initialize everything to default values
HazardDetection::HazardDetection()
    : Is_JR(false), Is_Hit(false), Is_Branch(false),
     NOP(false), EXEsemaphore(0){}


// Set inputs (register values and memory read signal)
void HazardDetection::HDUinputExecute(bool Is_JR, bool Is_Hit, bool Is_Branch) {

    this->Is_JR = Is_JR;
    this->Is_Hit = Is_Hit;
    this->Is_Branch = Is_Branch;
    EXEsemaphore.release();
}


// Perform hazard detection logic, requires syncronization
void HazardDetection::detectHazard() {

    EXEsemaphore.acquire();
    NOP = (Is_Branch && !Is_Hit) || Is_JR;
}
//NopSel = (BranchSignalin & !Hit) | JRSignalin;
// Output getter

bool HazardDetection::getNOP() const {
    return NOP;
}
