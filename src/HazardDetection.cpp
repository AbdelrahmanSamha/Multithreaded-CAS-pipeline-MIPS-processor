#include "HazardDetection.h"

// Constructor: initialize everything to default values
HazardDetection::HazardDetection()
    : IFID_Rs(0), IFID_Rt(0), IDEX_Rt(0), IDEX_MemRead(false),
    PC_Write(true), IFID_stall(false), NopSel(false) {}

// Set inputs (register values and memory read signal)
void HazardDetection::setInputExecute( uint8_t ID_EX_Rt, bool ID_EX_MemRead) {
    this->IDEX_Rt = ID_EX_Rt;
    this->IDEX_MemRead = ID_EX_MemRead;
}
void HazardDetection::setInputDecode(uint8_t IF_ID_Rs, uint8_t IF_ID_Rt) {
    this->IFID_Rs = IF_ID_Rs;
    this->IFID_Rt = IF_ID_Rt;
}
// Perform hazard detection logic, requires syncronization
void HazardDetection::detectHazard() {
    // If previous instruction is a load (ID_EX_MemRead is true) and
    // one of the source registers of the current instruction matches
    // the destination register of the previous load, stall
    if (IDEX_MemRead && ((IDEX_Rt == IFID_Rs) || (IDEX_Rt == IFID_Rt))) {
        PC_Write = false;
        IFID_stall = true;
        NopSel = true;
    }
    else {
        PC_Write = true;
        IFID_stall = false;
        NopSel = false;
    }
}

// Output getters
bool HazardDetection::getPCWrite() const {
    return PC_Write;
}

bool HazardDetection::getIFID_Stall() const {
    return IFID_stall;
}

bool HazardDetection::getNopSel() const {
    return NopSel;
}
