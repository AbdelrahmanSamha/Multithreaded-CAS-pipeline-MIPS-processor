#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include <string>

class Editor {
private:
    std::string fileName;
    void createTempFile();

public:
    Editor();
    ~Editor();
    void openFileForEditing();
    std::string getFileName() const;


};

#endif // EDITOR_H
