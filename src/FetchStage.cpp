#include "FetchStage.h"
#include "ConsoleLogger.h"
FetchStage::FetchStage(GlobalClock* clock, IFID* pipe, const std::vector<Instruction>& instrVector)
    : clk(clock), IFIDpipe(pipe), instructions(instrVector), PC(0x00400000) {
    Fetchthread = std::thread([this]() { Fetchjob(); });
}

FetchStage::~FetchStage() {
    if (Fetchthread.joinable()) {
        Fetchthread.join();
    }
}

void FetchStage::Fetchjob() {
    while (true) {
        logToConsole("Fetchthread waiting for clock tick\n");
        clk->waitforClockTick(); // Wait for the global clock tick
        logToConsole("Fetchthread starting new clock");
        if (!hasNextInstruction()) {
            std::cout << "No more instructions to fetch. Fetchthread terminated." << std::endl;
            break; // Exit the loop when there are no more instructions
        }

        uint32_t fetchedInstruction = fetchInstruction(); // Fetch the current instruction

        std::cout << "Fetched instruction (PC = " << std::hex << PC << "): " << std::hex << fetchedInstruction << std::endl;

        PC += 4; // Increment PC to the next instruction

        // Here we would write the fetched instruction to the IF/ID pipeline register
        IFIDpipe->writedata(PC, fetchedInstruction); // Placeholder for now


    }
}

bool FetchStage::hasNextInstruction() {


    // Calculate the maximum PC value, which corresponds to the last instruction's address
    uint32_t maxPC = BaseAddress + (instructions.size() * 4);

    // Check if the PC has reached or exceeded the end of the instruction vector
    return PC < maxPC;
}

// Return the machine code of the instruction at the current index
uint32_t FetchStage::fetchInstruction() {


    // Calculate the current index 
    uint32_t currentIndex = (PC - BaseAddress) / 4;

    std::cout << "Current Index: " << currentIndex << "\n";

    // Return the machine code of the instruction at the current index
    return instructions[currentIndex].machineCode;
}
