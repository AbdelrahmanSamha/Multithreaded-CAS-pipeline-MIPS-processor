#include "DecodeStage.h"
#include "ConsoleLogger.h"
#include <iomanip>

DecodeStage::DecodeStage(GlobalClock* clock, IFID* prev_pipe, IDEXE* next_pipe,  ControlUnit* Cu, RegisterFile* rf, HazardDetection* HDU, ForwardingUnit* FU, ZERO* Zu,Jump* JU)
    : clk(clock), IFIDpipe(prev_pipe), IDEXEpipe(next_pipe), CU(Cu), RF(rf), HDU(HDU),FU(FU), ZU(Zu), JU(JU) {
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
        FU->FUinputDecode(instrFields.rs , instrFields.rt, controlSignals.JAL_signal , controlSignals.ALUSrc);//send JAL and ALU src signals early 
        FU->evaluateForwarding(); //will make the rest of the stages wait till they all input to the Forwarding unit


        //Sign extend the immediate value 
        int32_t signExtendedImmediate = (instrFields.immediate & 0x0000FFFF); // Mask to get the lower 16 bits
        if (signExtendedImmediate & 0x00008000) {                             // Check if the 16th bit is set
            signExtendedImmediate |= 0xFFFF0000;                              // Extend the sign to upper 16 bits
        }
        
        
        

        //RF read will always happen
        //RF Write happens from the WB stage. Read is syncronized with write in a way that read cant happen before a write.   
        int32_t readdata1, readdata2;
        RF->readRegisters(instrFields.rs, instrFields.rt, readdata1, readdata2);
        
        int32_t OutMuxFA= FAMUX(FU->ForwardA, readdata1, FU->AluResult, FU->MEMAddress, FU->MEMreaddata);
        int32_t OutMuxFB= FBMUX(FU->ForwardB, readdata2, FU->AluResult, FU->MEMAddress, FU->MEMreaddata);


        HDU->detectHazard();

        // Mux for the LW dependency, inserting a bubble if there is one.
        if (HDU->getNOP()) {
            IDEXEpipe->writedata(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
        }
        else {
            IDEXEpipe->writedata(
                controlSignals.RegWriteEn,
                controlSignals.MemtoReg,
                controlSignals.MemWriteEn,
                controlSignals.MemReadEn,
                controlSignals.ALUop,
                controlSignals.RegDst,
                FU->ForwardC,//
                FU->ForwardD,//
                controlSignals.JR_Signal,
                controlSignals.Branch, 
                controlSignals.ZERO,
                PC,
                MC,
                OutMuxFA,
                OutMuxFB,
                signExtendedImmediate,
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
    controlSignals.RegDst = CU->getRegDst();
    controlSignals.ALUop = CU->getAluOp();

    controlSignals.MemReadEn = CU->getMemReadEn();
    controlSignals.MemWriteEn = CU->getMemWriteEn();

    controlSignals.MemtoReg = CU->getMemToReg();
    controlSignals.RegWriteEn = CU->getRegWriteEn();
    controlSignals.ZERO = CU->getZero();
    controlSignals.Branch = CU->getBranch();
    controlSignals.JR_Signal = CU->getJrSignal();

    controlSignals.JAL_signal = CU->getJalSignal();
    controlSignals.ALUSrc = CU->getAluSrc();
}

int32_t DecodeStage::FAMUX(int32_t FA, int32_t readdata1, int32_t AluResult, int32_t MEMAddress, int32_t MEMreaddata) {
    int32_t MuxOutPut = 0;
    switch (FA) {
    case 0: 
        MuxOutPut = readdata1;
        break; 
    case 1: 
        MuxOutPut = AluResult;
        break;
    case 2: 
        MuxOutPut = MEMAddress;
        break;
    case 3: 
        MuxOutPut = MEMreaddata;
        break;
    default:
        MuxOutPut = 0xFFFFFFFF;
        break;
    }
    return MuxOutPut;
}
int32_t DecodeStage::FBMUX(int32_t FB, int32_t readdata2, int32_t AluResult, int32_t MEMAddress, int32_t MEMreaddata) {
    int32_t MuxOutPut = 0;
    switch (FB) {
    case 0:
        MuxOutPut = readdata2;
        break;
    case 1:
        MuxOutPut = AluResult;
        break;
    case 2:
        MuxOutPut = MEMAddress;
        break;
    case 3:
        MuxOutPut = MEMreaddata;
        break;
    default:
        MuxOutPut = 0xFFFFFFFF;
        break;
    }
    return MuxOutPut;
}

DecodeStage::~DecodeStage() {
    // Join the thread to ensure proper cleanup
    RF->printRegisterFile();
    if (Decodethread.joinable()) {
        Decodethread.join();
    }
}


