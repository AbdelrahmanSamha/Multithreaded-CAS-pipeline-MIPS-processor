#include "FetchStage.h"
#include "ControlUnit.h"
#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include<iomanip>
FetchStage::FetchStage(GlobalClock* clock, const std::vector<Instruction>& instrVector, IFID* pipe,HazardDetection*HDU ,Jump* JU)
    : clk(clock), instructions(instrVector), IFIDpipe(pipe),HDU(HDU),JU(JU), PC(0) {
    Fetchthread = std::thread([this]() { Fetchjob(); });
}


void FetchStage::Fetchjob() {
    while (running) {
        ConsoleLog(1,"Fetchthread waiting for clock tick");
        clk->waitforClockTick(); // Wait for the global clock tick
        ConsoleLog(1,"Fetchthread starting new clock");
        int32_t fetchedInstruction;
        
        if (!hasNextInstruction()) { //if there is no more instructions then insert NOPs
            if (!FirstTime) {
                counter = 5;
                FirstTime = true;
            }
            ConsoleLog(1, "No more instructions to fetch. Halt inserted");
            fetchedInstruction = 0x00000000;
            int32_t hult = 0; 
            JU->JumpInputF(fetchedInstruction, PC1);
            JU->JumpUnitSignalsOutput();
            IFIDpipe->writedata(hult, fetchedInstruction, 0);
            if (JU->JmuxSel != 0) {
                PC = BranchingMux();
                PC1 = PC + 1;
                FirstTime = false;
            }
            counter--;
            
        }
        else {

            fetchedInstruction = fetchInstruction(PC);// Fetch the current instruction
            //int32_t branchins = (fetchedInstruction & 0xFC000000) >> 26;



            ConsoleLog(1, "Fetched instruction (PC = ", std::hex, PC, "): ", std::hex, std::setw(8), fetchedInstruction);

            PC1 = PC+1;
            

            JU->JumpInputF(fetchedInstruction, PC1);
            JU->JumpUnitSignalsOutput();
            ConsoleLog(1, "Is Flush = ", JU->Flush);
            if (JU->Flush) {
                IFIDpipe->writedata(0, 0, 0);
            }
            else { IFIDpipe->writedata(PC1, fetchedInstruction, JU->Prediction); }

            PC = BranchingMux();
            //if (JU->JmuxSel != 0) { PC1 = PC + 1; }
        }
    }
}

// Fetch the machine code of the instruction based on Jump unit selection
int32_t FetchStage::fetchInstruction(int32_t address) {
    ConsoleLog(1, "JMuxSel for current Cycle=", JU->JmuxSel);
    ConsoleLog(1, "BaddressE for current Cycle=", JU->MissTargetAddress);
    ConsoleLog(1, "UnitAddressOutput for current Cycle=", JU->UnitAddressOutput);
    int32_t currentIndex = address;

    // Check if the index is out of bounds
    if (currentIndex < 0 || currentIndex >= static_cast<int32_t>(instructions.size())) {
        return 0; // Return 0 if the index is invalid
    }

    // Return the machine code if the index is valid
    return instructions[currentIndex].machineCode;
}

int32_t FetchStage::BranchingMux() {
    int32_t address;
    switch (JU->JmuxSel) {
    case 0:
        address = PC1;
        break;
    case 1:
        address = JU->UnitAddressOutput;
        PC = JU->UnitAddressOutput;
        break;
    case 2:
        address = JU->Raddress;
        PC = JU->Raddress;
        break;
    case 3:
        address = JU->MissTargetAddress;
        PC = JU->MissTargetAddress;
        break;
    default:
        address = 0;
        PC = 0;
        break;
    }
    return address;
}

void FetchStage::stop() {
    running = false; 
}


bool FetchStage::hasNextInstruction() {
    // Calculate the maximum PC value, which corresponds to the last instruction's address
    int32_t maxPC = instructions.size();

    
    // Check if the PC has reached or exceeded the end of the instruction vector
    bool hasnext = (PC < maxPC);
    
    if (!hasnext && counter == 0) {
        ENDPROGRAM = true;

        
    }
    return PC < maxPC;
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}