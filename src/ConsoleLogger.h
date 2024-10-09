#pragma once
// ConsoleLogger.h
#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <mutex>
#include <iostream>
#include <string>

// Global mutex for console output
extern std::mutex console_mutex;

// Thread-safe function to print to the console
void logToConsole(const std::string& message);

#endif // CONSOLE_LOGGER_H
