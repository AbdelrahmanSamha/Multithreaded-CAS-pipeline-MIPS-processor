#include <iostream>
#include <Windows.h>


#include "Editor.h"
#include "Assembler.h"
#include "GlobalClock.h"
//stages and pipes
#include "FetchStage.h"
#include "IFID.h"
#include "DecodeStage.h"
#include "IDEXE.h"
#include "ExecuteStage.h"
#include "EXEMEM.h"
#include "MemoryStage.h"
#include "MEMWB.h"
#include "WritebackStage.h"
//units 
#include "RegisterFile.h"
#include "ControlUnit.h"
#include "HazardDetection.h"
#include "ZERO.h"
#include "Jump.h"
#include "ForwardingUnit.h"


int main() {
    //force the console window to be fullscreen since the ConsoleLogger deals with Y coordinates that need to be available before hand.
    HWND console = GetConsoleWindow(); 
    ShowWindow(console, SW_MAXIMIZE);

    // Define the temporary file name
    Editor editor;
    std::cout << "Opening file for editing: " << editor.getFileName() << std::endl;
    editor.openFileForEditing();
    std::cout << "Edit the file and then save it. Press Enter when done...";
    std::cin.get(); // Wait for user input before proceeding

    // Create Assembler instance
    std::string inputFileName = editor.getFileName();
    std::string outputFileName = "TextSegmeant.asm";

    Assembler assembler(inputFileName, outputFileName);
    assembler.assemble();
    std::cout << "Assembling completed. Check the file: " << outputFileName << std::endl;

    //Initializing of the units in the data path 
    ControlUnit CU;
    RegisterFile RF;
    HazardDetection HDU;
    Jump JU;
    ZERO ZU;
    ForwardingUnit FU;
    // generate a clock for 5 threads, among with the stages initialization and pipes
    GlobalClock clk(5); //determine the number of threads

    //define the pipes before making objects of each stage 
    IFID IFIDpipe(&HDU, &JU);
    IDEXE IDEXEpipe;
    EXEMEM EXEMEMpipe;
    MEMWB MEMWBpipe;

    //Stages object takes: (clk, previous memory or pipe, next_pipe)
    FetchStage Fetchthread(&clk, assembler.getInstructions(), &IFIDpipe, &HDU , &JU );
  
    DecodeStage Decodethread(&clk, &IFIDpipe, &IDEXEpipe, &CU, &RF, &HDU, &ZU ,&JU);

    ExecuteStage Executethread(&clk, &IDEXEpipe, &EXEMEMpipe, &HDU, &FU);

    MemoryStage Memorythread(&clk, &EXEMEMpipe , &MEMWBpipe, &FU);

    WritebackStage WBthread(&clk, &MEMWBpipe);



    //generate 2 clock ticks
    for (int i = 0; i < 5; i++) {
        clk.clockTick();
    }
    Fetchthread.stop();
    Decodethread.stop();
    Executethread.stop();
    Memorythread.stop();
    WBthread.stop();

    return 0;
}//csmc