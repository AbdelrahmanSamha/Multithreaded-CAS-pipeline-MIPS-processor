#include "FetchStage.h"
#include "ConsoleLogger.h"
#include<iomanip>
FetchStage::FetchStage(GlobalClock* clock, const std::vector<Instruction>& instrVector, IFID* pipe)
    : clk(clock), instructions(instrVector), IFIDpipe(pipe),  PC(0x00400000) {
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

        ConsoleLog(1,"Fetched instruction (PC = " , std::hex , PC , "): " , std::hex , std::setw(8) ,fetchedInstruction );

       

        // Here we would write the fetched instruction to the IF/ID pipeline register
        IFIDpipe->writedata(PC, fetchedInstruction); // Placeholder for now

        PC += 4; // Increment PC to the next instruction
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


    // Calculate the current index 
    uint32_t currentIndex = (PC - BaseAddress) / 4;

    ConsoleLog(1, "Current Index: " , currentIndex);

    // Return the machine code of the instruction at the current index
    return instructions[currentIndex].machineCode;
}
void FetchStage::stop() {
    running = false; 
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}
