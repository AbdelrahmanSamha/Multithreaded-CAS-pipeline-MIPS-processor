#pragma once
#ifndef DECODESTAGE_H
#define DECODESTAGE_h

#include <thread>


#include "GlobalClock.h"
#include "IFID.h"
#include"IDEXE.h"
#include "RegisterFile.h"
#include "ControlUnit.h"
#include "HazardDetection.h"



class GlobalClock; // Forward declaration of GlobalClock
class IFID;        // Forward declaration of IFID 
class IDEXE; // Forward declaration of IDEXE 

class DecodeStage {
private:
    // Global Comunication needs:
    GlobalClock* clk;
    IFID* IFIDpipe;
    IDEXE* IDEXEpipe;
    ControlUnit* CU;
    RegisterFile* RF;
    HazardDetection* HDU;
private: 
    //local stage needs:
    std::thread Decodethread;
    uint32_t PC=0;	//Programcounter
    uint32_t MC=0; //MachineCode
    void Decodejob();
    bool running = true ;// temporary for debugging purposes.(so we dont use 100%CPU)
public:
    void stop();// temporary for debugging purposes.(so we dont use 100%CPU)
    DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe, ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU);
    ~DecodeStage();
};



#endif