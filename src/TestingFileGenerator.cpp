//
// Created by arnas on 9/12/25.
//

#include "../include/TestingFileGenerator.h"

void TestingFileGenerator::oneSymbolFiles(const std::string& symbols) {
    std::filesystem::path testDir("../data/input/test/OSF");
    ensureTestFolders(testDir);
    for (char c : symbols) {
        std::ofstream out("../data/input/test/OSF/OSF_" + std::string(1, c));
        out << c;
    }
}

void TestingFileGenerator::ensureTestFolders(const std::filesystem::path& dirPath) {
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
}
