#include "FetchStage.h"
#include "ControlUnit.h"
#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include<iomanip>
FetchStage::FetchStage(GlobalClock* clock, const std::vector<Instruction>& instrVector, IFID* pipe,HazardDetection*HDU ,Jump* JU)
    : clk(clock), instructions(instrVector), IFIDpipe(pipe),HDU(HDU),JU(JU), PC(0x00400000) {
    Fetchthread = std::thread([this]() { Fetchjob(); });
}


void FetchStage::Fetchjob() {
    while (running) {
        ConsoleLog(1,"Fetchthread waiting for clock tick");
        clk->waitforClockTick(); // Wait for the global clock tick
        ConsoleLog(1,"Fetchthread starting new clock");
        int32_t fetchedInstruction;
        if (!hasNextInstruction()) {
            ConsoleLog(1, "No more instructions to fetch. Halt inserted");
             fetchedInstruction = 0x00000000;
             IFIDpipe->writedata(PC, fetchedInstruction);
        }
        else {
            fetchedInstruction = fetchInstruction();// Fetch the current instruction


            ConsoleLog(1, "Fetched instruction (PC = ", std::hex ,PC , "): ", std::hex, std::setw(8), fetchedInstruction);

            PC += 4;

            IFIDpipe->writedata(PC, fetchedInstruction);


            JU->JumpInputF(fetchedInstruction, PC);
            JU->JumpUnitSignalsOutput();
        }
    }
}

// Fetch the machine code of the instruction based on Jump unit selection
int32_t FetchStage::fetchInstruction() {
    int32_t address;

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
        address = JU->Baddress;
        PC = JU->Baddress;
        break;
    case 3:
        address = JU->Raddress;
        PC = JU->Raddress;
        break;
    default:
        address = 0x00400000;
        PC = 0x00400000;
        break;
    }

    // Calculate current index and fetch machine code
    int32_t currentIndex = (address - BaseAddress) / 4;
    return instructions[currentIndex].machineCode;
}

void FetchStage::stop() {
    running = false; 
}

bool FetchStage::hasNextInstruction() {


    // Calculate the maximum PC value, which corresponds to the last instruction's address
    size_t maxPC = BaseAddress + (instructions.size() * 4);

    // Check if the PC has reached or exceeded the end of the instruction vector
    return PC < maxPC;
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}