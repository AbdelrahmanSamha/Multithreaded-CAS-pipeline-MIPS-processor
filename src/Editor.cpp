#include "Editor.h"
#include <fstream>
#include <iostream>
#include <cstdlib> // For system()



Editor::Editor() {
    createTempFile();
}

Editor::~Editor() {
    // Optionally remove the file here if needed
}

void Editor::createTempFile() {
    fileName = "temp_instructions.asm";

    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error creating temporary file.\n";
        return;
    }
    file.close();
}

void Editor::openFileForEditing() {
    std::string command = "notepad " + fileName;
    system(command.c_str());
}

std::string Editor::getFileName() const {
    return fileName;
}
