#pragma once 
#ifndef JUMP_H
#define JUMP_H
#include <semaphore>
#include <cstdint>

class Jump {
private:
    // Input data
    int32_t Instruction, PC4; //recieved from the fetch stage
    bool AndGate, Jr ; //recieved from the decode stage.
    

    bool JAL; //made inside the unit
    // Semaphores for synchronization
    std::binary_semaphore decodeSemaphore;


public:
    // Output data
    bool Flush;
    int32_t JmuxSel ;
    int32_t Jaddress;
    int32_t Baddress;
    int32_t Raddress;
    // Constructor
    Jump();

    // Functions to set inputs and perform logic
    void JumpInputF(int32_t instruction, int32_t pc4);
    void JumpInputD(int32_t baddress,int32_t raddress,bool zero, bool jr);
    void JumpUnitSignalsOutput(); 
    void JumpUnitAddressOutput();

};
#endif