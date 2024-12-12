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
            ConsoleLog(1, "No more instructions to fetch. Halt inserted");
             fetchedInstruction = 0x00000000;
             int32_t hult = 0; 
             IFIDpipe->writedata(hult, fetchedInstruction);
             JU->SyncWithExecute();//in case the last instruction is branch
        }
        else {
            fetchedInstruction = fetchInstruction();// Fetch the current instruction
            JU->SyncWithExecute();//allows the execute to continue execution, because it is currently stopped at JUinput.
            int32_t temptowait = (fetchedInstruction & 0xFC000000) >> 26;
            if ((temptowait== 0x4) || (temptowait == 0x5)) {//checks if we fetch a branch instruction
                counter = 2; //we need to wait to see if the branch is taken or not 
            };


            ConsoleLog(1, "Fetched instruction (PC = ", std::hex ,PC , "): ", std::hex, std::setw(8), fetchedInstruction);

            PC += 1;

            IFIDpipe->writedata(PC, fetchedInstruction);


            JU->JumpInputF(fetchedInstruction, PC);
            
            JU->JumpUnitSignalsOutput();
            ConsoleLog(1, "JMuxSel after func call=", JU->JmuxSel);
        }
    }
}

// Fetch the machine code of the instruction based on Jump unit selection
int32_t FetchStage::fetchInstruction() {
    int32_t address;
    ConsoleLog(1, "JMuxSel for current Cycle=", JU->JmuxSel);
    ConsoleLog(1, "Baddress for current Cycle=", JU->Baddress);
    // Set the address based on the jump selector
    switch (JU->JmuxSel) {
    case 0:
        address = PC;
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

    // Calculate current index and fetch machine code
    int32_t currentIndex = address;
    return instructions[currentIndex].machineCode;
}

void FetchStage::stop() {
    running = false; 
}

bool FetchStage::hasNextInstructionformain() {
    if (counter != 0 ) {
        counter -= 1;
        return true;
    }
    else {
        // Calculate the maximum PC value, which corresponds to the last instruction's address
        size_t maxPC = instructions.size();

        // Check if the PC has reached or exceeded the end of the instruction vector
        return PC < maxPC;
    }
}
bool FetchStage::hasNextInstruction() {
    // Calculate the maximum PC value, which corresponds to the last instruction's address
    size_t maxPC = instructions.size();

    // Check if the PC has reached or exceeded the end of the instruction vector
    return PC < maxPC;
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}