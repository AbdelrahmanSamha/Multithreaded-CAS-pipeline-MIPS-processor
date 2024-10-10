#pragma once
#ifndef DECODESTAGE_H
#define DECODESTAGE_h
#include "GlobalClock.h"
#include "IFID.h"
#include <thread>

class GlobalClock; // Forward declaration of GlobalClock
class IFID;        // Forward declaration of IFID
class IDEXE;       // Forward declaration of IFID 
class DecodeStage {
private:

    GlobalClock* clk;
    IFID* IFIDpipe;
    IDEXE* IDEXEpipe;
    std::thread Decodethread;
    void Decodejob();
    uint32_t PC;	//Programcounter
    uint32_t MC; //MachineCode
public:
    DecodeStage(GlobalClock* clock, IFID* pipe);
    ~DecodeStage();
};



#endif