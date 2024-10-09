// ConsoleLogger.cpp
#include "ConsoleLogger.h"

// Define the global mutex
std::mutex console_mutex;

void logToConsole(const std::string& message) {
    std::lock_guard<std::mutex> lock(console_mutex);  // Lock the mutex
    std::cout << message << std::endl;                // Print the message safely
}
