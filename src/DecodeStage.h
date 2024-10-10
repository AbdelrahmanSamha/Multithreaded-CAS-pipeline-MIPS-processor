#pragma once
#ifndef DECODESTAGE_H
#define DECODESTAGE_h
#include "GlobalClock.h"
#include "IFID.h"
#include"IDEXE.h"
#include <thread>

class GlobalClock; // Forward declaration of GlobalClock
class IFID;        // Forward declaration of IFID 
class IDEXE; // Forward declaration of IDEXE 

class DecodeStage {
private:
    // Global Comunication needs:
    GlobalClock* clk;
    IFID* IFIDpipe;
    IDEXE* IDEXEpipe;
private: 
    //local stage needs:
    std::thread Decodethread;
    uint32_t PC=0;	//Programcounter
    uint32_t MC=0; //MachineCode
    void Decodejob();
public:

    DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe);
    ~DecodeStage();
};



#endif