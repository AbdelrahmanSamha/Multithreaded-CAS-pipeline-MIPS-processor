#include <iostream>
#include "GlobalClock.h"
#include "Assembler.h"
#include "DecodeStage.h"
#include "FetchStage.h"
#include "IFID.h"
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
    GlobalClock clk(2);
    IFID pipe;
    FetchStage Fetchthread(&clk, &pipe, assembler.getInstructions());
    DecodeStage Decodethread(&clk, &pipe);
    //generate 2 clock ticks
    for (int i = 0; i < 2; i++)
        clk.clockTick();

    return 0;
}