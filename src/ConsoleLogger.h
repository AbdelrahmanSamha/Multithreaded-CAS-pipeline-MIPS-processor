#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <iostream>

#include <mutex>
#include <Windows.h>
#include <sstream>

// Declare the global mutex
extern std::mutex console_mutex;

// Function declarations
extern void SetCursorPosition(int x, int y);

// Global variables to keep track of cursor positions
extern int maxY;
extern int yF; // Fetch stage
extern int yD; // Decode stage
extern int yE; // Execute stage
extern int yM; // Memory stage
extern int yW; // Writeback stage
extern const int width; // Column width for each stage

// Function to log messages from different threads
template<typename... Args>
 inline void ConsoleLog(int threadIdx, Args&&... args){
    {
        std::lock_guard<std::mutex> lock(console_mutex);  // Lock the mutex

        // Create a stringstream to handle concatenating different types
        std::stringstream ss;

        // Using a fold expression to unpack the parameter pack and append to the stringstream
        (ss << ... << args);

        int startX = 1;
        int startY = 1;
        if (threadIdx == 0) { // For the main thread to output to the console
            startY = maxY + 1;
            SetCursorPosition(startX, startY);
            std::cout << ss.str() << std::endl;
            maxY+=1;
            yF = maxY+ 1;
            yD = maxY+ 1;
            yE = maxY+ 1;
            yM = maxY+ 1;
            yW = maxY+ 1;
            return; // Correctly return from the function
        }

        
        // Determine the starting Y position based on the thread index
        switch (threadIdx) {
        case 1: startY = yF++; break;  // Fetch stage
        case 2: startY = yD++; break;  // Decode stage
        case 3: startY = yE++; break;  // Execute stage
        case 4: startY = yM++; break;  // Memory stage
        case 5: startY = yW++; break;  // Writeback stage
        default: break;
        }

        startX = (threadIdx - 1) * width; // Use threadIdx-1 to match the correct column starting from 0.

        SetCursorPosition(startX, startY);



        // Output the result to the console
        std::cout << ss.str() << std::endl;
        maxY = (maxY > startY) ? maxY : startY;
    }
}
   


#endif // CONSOLE_LOGGER_H
