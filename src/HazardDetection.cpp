#include "HazardDetection.h"

// Constructor: initialize everything to default values
HazardDetection::HazardDetection()
    : JRSIGNAL(false), ANDGATE(false),
     NOP(false), EXEsemaphore(0){}


// Set inputs (register values and memory read signal)
void HazardDetection::HDUinputExecute( bool JRSIGNAL, bool ANDGATE) {

    this->JRSIGNAL = JRSIGNAL;
    this->ANDGATE = ANDGATE;
    EXEsemaphore.release();
}


// Perform hazard detection logic, requires syncronization
void HazardDetection::detectHazard() {

    EXEsemaphore.acquire();
    NOP = ANDGATE || JRSIGNAL;
}

// Output getter

bool HazardDetection::getNOP() const {
    return NOP;
}
