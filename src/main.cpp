#include <iostream>
//#include <Windows.h>

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
    /*HWND console = GetConsoleWindow();
    ShowWindow(console, SW_MAXIMIZE);*/
    std::cout << "PLEASE MAKE SURE THE WINDOW OF THE TERMINAL IS MAXIMIZED..." << std::endl;
    std::cout << "Press any key to continue..." << std::endl;
    std::cin.get();
    // Define the temporary file name
    Editor editor;
    std::cout << "Enter your mips assembly code in the opened text file: " << editor.getFileName() << std::endl;
    std::cout << "Make sure the first line contains \".text\" by it self to ensure proper machine code " << std::endl;
    std::cout << "For more details on the syntax please check the appendix regarding the assembler in the documentation.. " << std::endl;
    std::cout << "Edit the file and then save it. Press Enter when done..." << std::endl;
    editor.openFileForEditing();
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
  
    DecodeStage Decodethread(&clk, &IFIDpipe, &IDEXEpipe, &CU, &RF, &HDU,&FU, &ZU ,&JU);

    ExecuteStage Executethread(&clk, &IDEXEpipe, &EXEMEMpipe, &HDU, &FU, &ZU, &JU);

    MemoryStage Memorythread(&clk, &EXEMEMpipe , &MEMWBpipe, &FU);

    WritebackStage WBthread(&clk, &MEMWBpipe,&FU,&RF);



    
    while (true) {
        if (Fetchthread.ENDPROGRAM) {
            
            break;

        }
        else {
            clk.clockTick();
        }
    }
    Fetchthread.stop();
    Decodethread.stop();
    Executethread.stop();
    Memorythread.stop();
    WBthread.stop();

    return 0;
}//csmc
