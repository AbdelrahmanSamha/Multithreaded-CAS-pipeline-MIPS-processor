#include <iostream>


#include "GlobalClock.h"
#include "Assembler.h"
#include "FetchStage.h"
#include "IFID.h"
#include "DecodeStage.h"
#include "IDEXE.h"
#include "ExecuteStage.h"
#include "Editor.h"


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

    // generate a clock for 2 threads, among with the stages initialization and IF/ID pipe

    GlobalClock clk(3); //remember to change this (num_threads), when you add a new stage.

    //define the pipes before making objects of each stage 
    IFID IFIDpipe;
    IDEXE IDEXEpipe;

    FetchStage Fetchthread(&clk, &IFIDpipe, assembler.getInstructions());
  
    DecodeStage Decodethread(&clk, &IFIDpipe,&IDEXEpipe);

    ExecuteStage Executethread(&clk, &IDEXEpipe);


    //generate 2 clock ticks
    for (int i = 0; i < 5; i++)
        clk.clockTick();

    return 0;
}//csmc