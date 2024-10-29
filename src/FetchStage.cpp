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

        if (!hasNextInstruction()) {
            std::cout << "No more instructions to fetch. Fetchthread terminated." << std::endl;
            break; // Exit the loop when there are no more instructions
        }

  
        uint32_t fetchedInstruction = fetchInstruction(); // Fetch the current instruction

        IFIDpipe->writedata(PC, fetchedInstruction);

        ConsoleLog(1, "Fetched instruction (PC = ", std::hex, PC, "): ", std::hex, std::setw(8), fetchedInstruction);

        JU->JumpSignalF(fetchedInstruction, PC + 4);
        JU->JumpUnit();

        // Here we would write the fetched instruction to the IF/ID pipeline register
       
    }
}

bool FetchStage::hasNextInstruction() {


    // Calculate the maximum PC value, which corresponds to the last instruction's address
    size_t maxPC = BaseAddress + (instructions.size() * 4);

    // Check if the PC has reached or exceeded the end of the instruction vector
    return PC < maxPC;
}

// Return the machine code of the instruction at the current index
uint32_t FetchStage::fetchInstruction() {

    switch (JU->JmuxSel) {//implemnt as a mux...
    case 0:    // Calculate the current index 
        uint32_t currentIndex = (PC - BaseAddress) / 4;

        // Return the machine code of the instruction at the current index
        return instructions[currentIndex].machineCode;
    case 1:
    case 2: 
    case 3: 
    }
}
void FetchStage::stop() {
    running = false; 
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}
