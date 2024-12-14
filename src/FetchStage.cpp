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
            IFIDpipe->writedata(hult, fetchedInstruction);
            if (JU->JmuxSel != 0) {
                PC = BranchingMux();
                PC1 = PC + 1;
                FirstTime = false;
            }
            counter--;
            
        }
        else {

            fetchedInstruction = fetchInstruction(PC);// Fetch the current instruction
            int32_t branchins = (fetchedInstruction & 0xFC000000) >> 26;



            ConsoleLog(1, "Fetched instruction (PC = ", std::hex, PC, "): ", std::hex, std::setw(8), fetchedInstruction);

            PC1 = PC;
            PC1 += 1;
            IFIDpipe->writedata(PC1, fetchedInstruction);

            

            JU->JumpInputF(fetchedInstruction, PC1);
            JU->JumpUnitSignalsOutput();
            PC = BranchingMux();

            
        }
    }
}

// Fetch the machine code of the instruction based on Jump unit selection
int32_t FetchStage::fetchInstruction(int32_t address) {
    ConsoleLog(1, "JMuxSel for current Cycle=", JU->JmuxSel);
    ConsoleLog(1, "Baddress for current Cycle=", JU->Baddress);
    // Set the address based on the jump selector
    

    // Calculate current index and fetch machine code
    int32_t currentIndex = address;
    return instructions[currentIndex].machineCode;
}

int32_t FetchStage::BranchingMux() {
    int32_t address;
    switch (JU->JmuxSel) {
    case 0:
        address = PC1;
        break;
    case 1:
        address = JU->Jaddress;
        PC = JU->Jaddress;
        break;
    case 2:
        address = JU->Raddress;
        PC = JU->Raddress;
        break;
    case 3:
        address = JU->Baddress;
        PC = JU->Baddress;
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
    bool hasnext = (PC1 < maxPC);
    
    if (!hasnext && counter == 0) {
        ENDPROGRAM = true;

        
    }
    return PC1 < maxPC;
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}