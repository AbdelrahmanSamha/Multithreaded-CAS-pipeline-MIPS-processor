#pragma once 
#ifndef JUMP_H
#define JUMP_H
#include <semaphore>
#include <cstdint>

class Jump {
private:
    // Input data
    uint32_t Instruction, PC4; //recieved from the fetch stage
    bool AndGate, Jr ; //recieved from the decode stage.
    

    bool JAL; //made inside the unit
    // Semaphores for synchronization
    std::binary_semaphore decodeSemaphore;


public:
    // Output data
    bool Flush;
    uint8_t JmuxSel ;
    uint32_t Jaddress;
    uint32_t Baddress;
    uint32_t Raddress;
    // Constructor
    Jump();

    // Functions to set inputs and perform logic
    void JumpInputF(uint32_t instruction, uint32_t pc4);
    void JumpInputD(uint32_t baddress,uint32_t raddress,bool zero, bool jr);
    void JumpUnitSignalsOutput(); 
    void JumpUnitAddressOutput();

};
#endif