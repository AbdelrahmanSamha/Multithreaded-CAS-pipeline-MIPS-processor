#include <iostream>

#include "Editor.h"
#include "Assembler.h"
#include "GlobalClock.h"

#include "FetchStage.h"
#include "IFID.h"
#include "DecodeStage.h"
#include "IDEXE.h"
#include "ExecuteStage.h"
#include "EXEMEM.h"
#include "MemoryStage.h"
#include "MEMWB.h"
#include "WritebackStage.h"



int main() {
    // Define the temporary file name
    Editor editor;
    std::cout << "Opening file for editing: " << editor.getFileName() << std::endl;
    editor.openFileForEditing();
    std::cout << "Edit the file and then save it. Press Enter when done...";
    std::cin.get(); // Wait for user input before proceeding

    // Create Assembler instance
    std::string inputFileName = editor.getFileName();
    std::string outputFileName = "DataSegmeant.asm";

    Assembler assembler(inputFileName, outputFileName);
    assembler.assemble();
    std::cout << "Assembling completed. Check the file: " << outputFileName << std::endl;

    // generate a clock for 5 threads, among with the stages initialization and pipes

    GlobalClock clk(5); //determine the number of threads

    //define the pipes before making objects of each stage 
    IFID IFIDpipe;
    IDEXE IDEXEpipe;
    EXEMEM EXEMEMpipe;
    MEMWB MEMWBpipe;

    //Stages object takes: (clk, previous memory or pipe, next_pipe)
    FetchStage Fetchthread(&clk, assembler.getInstructions(), &IFIDpipe);
  
    DecodeStage Decodethread(&clk, &IFIDpipe,&IDEXEpipe);

    ExecuteStage Executethread(&clk, &IDEXEpipe, &EXEMEMpipe);

    MemoryStage Memorythread(&clk, &EXEMEMpipe , &MEMWBpipe);

    WritebackStage WBthread(&clk, &MEMWBpipe);



    //generate 2 clock ticks
    for (int i = 0; i < 5; i++)
        clk.clockTick();

    return 0;
}//csmc