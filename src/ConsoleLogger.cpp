#include "ConsoleLogger.h"
#include <mutex>
// Define the global mutex
std::mutex console_mutex;

// Track the MAXIMUM Y coordinate to be used by the main thread , Start at the initial Y coordinate 
int maxY = 10; 

// Fixed for the coordinates calculations
const int width = 50; // Width for the stages segment output
int yF = 10;
int yD = 10;
int yE = 10;
int yM = 10;
int yW = 10;

// To segment the console in a way that each thread has its own column to write in
void SetCursorPosition(int x, int y) {
    // Get the handle for the console window
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(console, pos);
}
