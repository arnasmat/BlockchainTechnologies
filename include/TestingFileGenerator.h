//
// Created by arnas on 9/12/25.
//

#ifndef TESTINGFILEGENERATOR_H
#define TESTINGFILEGENERATOR_H

#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

class TestingFileGenerator {
private:
    static void ensureTestFolders(const std::filesystem::path &dirPath);
public:
    static void oneSymbolFiles(const std::string& symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
};




#endif //TESTINGFILEGENERATOR_H
