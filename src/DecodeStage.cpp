#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe,  ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU, ZERO* Zu,Jump* JU)
    : clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe), CU(Cu), RF(rf), HDU(HDU), ZU(Zu), JU(JU) {
    // Launch the decoding thread and store it in the class
    Decodethread = std::thread([this]() { Decodejob(); });
}

void DecodeStage::Decodejob() {
    // Keep working
    while (running) {
        ConsoleLog(2, "Decodethread waiting for clock tick");
        clk->waitforClockTick();  // Called at the beginning of all the stages.
        ConsoleLog(2, "Decodethread starting new clock");

        IFIDpipe->readdata(PC, MC);
             
        // Instruction decoding...
        InstructionDecode();

        // Generate control signals
        GenerateControlSignals();


        //Sign extend the immediate value 
        int32_t signExtendedImmediate = (instrFields.immediate & 0x0000FFFF); // Mask to get the lower 16 bits
        if (signExtendedImmediate & 0x00008000) {                 // Check if the 16th bit is set
            signExtendedImmediate |= 0xFFFF0000;                  // Extend the sign to upper 16 bits
        }
        
        //branch address calculation...
        uint32_t BranchAddress = PC + (signExtendedImmediate << 2);

        // RF read will always happen
        uint32_t readdata1, readdata2;
        RF->readRegisters(instrFields.rs, instrFields.rt, readdata1, readdata2);

        //ZERO unit input
        ZU->ZeroInput(readdata1, readdata2, controlSignals.ZERO);

        //this is the and gate result entering the JUMP unit
        bool AndGate = (controlSignals.Branch && ZU->ZeroOutput());
       
        JU->JumpInputD(BranchAddress,readdata1 ,AndGate, controlSignals.JR_Signal);

        HDU->setInputDecode(instrFields.rs, instrFields.rt);  // Input to the hazard detection unit


       // Hazard detection... needs further adjustment to flush IDEXE, need data from EXEC.
        HDU->detectHazard();

       // Mux for the LW dependency, inserting a bubble if there is one.
        if (HDU->getNOP()) {
            IDEXEpipe->writedata(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
        }
        else {
            IDEXEpipe->writedata(
                controlSignals.RegWriteEn,
                controlSignals.MemtoReg,
                controlSignals.MemWriteEn,
                controlSignals.MemReadEn,
                controlSignals.ALUSrc,
                controlSignals.ALUop,
                controlSignals.RegDst,
                controlSignals.JAL_signal,
                PC,
                MC,
                readdata1,
                readdata2,
                instrFields.immediate,
                instrFields.rs,
                instrFields.rt,
                instrFields.rd);
        }
        


        ConsoleLog(2, "Decoding logic done...");
    }
}

void DecodeStage::stop() {
    running = false;
}

void DecodeStage::InstructionDecode() {
    // Decode the instruction and store fields in the struct
    instrFields.opcode = (MC >> 26) & 0x3F;   // 6-bit opcode
    instrFields.rs = (MC >> 21) & 0x1F;       // 5-bit source register
    instrFields.rt = (MC >> 16) & 0x1F;       // 5-bit target register
    instrFields.rd = (MC >> 11) & 0x1F;       // 5-bit destination register
    instrFields.shamt = (MC >> 6) & 0x1F;     // 5-bit shift amount
    instrFields.funct = MC & 0x3F;            // 6-bit function code
    instrFields.immediate = MC & 0xFFFF;      // 16-bit immediate value
    instrFields.address = MC & 0x03FFFFFF;    // 26-bit address (for jump)
}

void DecodeStage::GenerateControlSignals() {
    // Set control signals based on opcode and function
    CU->setControlSignals(instrFields.opcode, instrFields.funct);
    // Populate the controlSignals struct with values from ControlUnit
    controlSignals.ALUop = CU->getALUOp();
    controlSignals.RegDst = CU->getRegDst();
    controlSignals.ALUSrc = CU->getALUSrc();
    controlSignals.Branch = CU->getBranch();
    controlSignals.MemReadEn = CU->getMemReadEn();
    controlSignals.MemtoReg = CU->getMemtoReg();
    controlSignals.MemWriteEn = CU->getMemWriteEn();
    controlSignals.RegWriteEn = CU->getRegWriteEn();
    controlSignals.JR_Signal = CU->getJR_Signal();
    controlSignals.ZERO = CU->getZERO();
    controlSignals.JAL_signal = CU->getJAL_Signal();
}

DecodeStage::~DecodeStage() {
    // Join the thread to ensure proper cleanup
    if (Decodethread.joinable()) {
        Decodethread.join();
    }
}
