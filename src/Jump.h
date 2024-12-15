#pragma once 
#ifndef JUMP_H
#define JUMP_H
#include <semaphore>
#include <cstdint>

class Jump {
private:
    // Input data
    int32_t Instruction = 0;
    int32_t PC1 = 0; //recieved from the fetch stage
    bool Is_Hit = 0;
    bool Jr = 0; //recieved from the decode stage.
    bool Branch = 0;

    bool is_JumpIns=0; //made inside the unit
    bool is_BranchIns=0;
    bool is_nextBorJ=0; 
    // Semaphores for synchronization
    std::binary_semaphore ExecuteSemaphore;
    

public:
    // Output data
    bool Flush = 0;
    int32_t JmuxSel = 0;
    int32_t Jaddress = 0;
    int32_t MissTargetAddress = 0;
    int32_t Raddress=0;
    int32_t BranchAddress = 0;
    int32_t UnitAddressOutput = 0;
    bool Prediction = 0;
    // Constructor
    Jump();

    // Functions to set inputs and perform logic
    void JumpInputF(int32_t instruction, int32_t pc4);
    void JumpInputEXE(int32_t baddress,int32_t raddress,bool Is_Hit, bool jr, bool Branch);
    void JumpUnitSignalsOutput(); 
    void JumpUnitAddressOutput();
    
    
};
#endif