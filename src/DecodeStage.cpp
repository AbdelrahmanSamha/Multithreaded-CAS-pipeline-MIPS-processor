#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe, FetchStage* Fetch, ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU)
    : clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe),Fetch(Fetch), CU(Cu), RF(rf), HDU(HDU) {
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

        // RF read will always happen
        uint32_t readdata1, readdata2;
        RF->readRegisters(instrFields.rs, instrFields.rt, readdata1, readdata2);

        

        // Adding PC + (Imm << 2). Sending the value to the Fetch stage.
        uint32_t Address = PC + (instrFields.immediate << 2);

        // Sending the signals & data to the required units
        HDU->setInputDecode(instrFields.rs, instrFields.rt);  // Input to the hazard detection unit

        IDEXEpipe->writedata(
            controlSignals.RegWriteEn,
            controlSignals.MemtoReg,
            controlSignals.MemWriteEn,
            controlSignals.MemReadEn,
            controlSignals.ALUsrc,
            controlSignals.ALUOp,
            controlSignals.RegDst,
            PC,
            readdata1,
            readdata2,
            instrFields.immediate,
            instrFields.rs,
            instrFields.rt,
            instrFields.rd
        );

        // Hazard detection... needs further adjustment to flush IDEXE, need data from EXEC.
        HDU->detectHazard();

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

    // Get control signals and store them in the struct
    controlSignals.ALUOp = CU->getALUOp();
    controlSignals.ALUsrc = CU->getAluSrc();
    controlSignals.MemReadEn = CU->getMemReadEn();
    controlSignals.MemtoReg = CU->getMemtoReg();
    controlSignals.MemWriteEn = CU->getMemWriteEn();
    controlSignals.RegDst = CU->getRegDst();
    controlSignals.RegWriteEn = CU->getRegWriteEn();
    controlSignals.JumpSel = CU->getJumpSel();
    controlSignals.Branch = CU->getBranch();
}

DecodeStage::~DecodeStage() {
    // Join the thread to ensure proper cleanup
    if (Decodethread.joinable()) {
        Decodethread.join();
    }
}
