#include "../../include/HashAlg/CLI/TestingFileGenerator.h"

namespace TestingFileGenerator {
    void generateAllFiles() {
        removeOldTestFiles();

        static std::mt19937 rng(std::random_device{}());

        generateOneSymbolFiles();
        generateManyRandomSymbolsFiles(rng);
        generateSimilarSymbolsFiles(rng);
        generateEmptyFile();
    }

    // File names shortened to OSF - One Symbol Files
    void generateOneSymbolFiles(const std::string &symbols) {
        std::filesystem::path testDir("../data/input/test/OSF/");
        ensureTestFolders(testDir);
        for (char c: symbols) {
            std::ofstream out(testDir.string() + std::string(1, c) + ".txt");
            out << c;
        }
    }

    // File names shortened to MRS - Many Random Symbols
    void generateManyRandomSymbolsFiles(std::mt19937 &rng, const std::string &validSymbols) {
        std::filesystem::path testDir("../data/input/test/MRS/");
        ensureTestFolders(testDir);

        for (int i = 0; i < 10; i++) {
            manyRandomSymbolsFileGen(rng, testDir, validSymbols);
        }
    }

    // Similar to MRS, but it creates one MRS file and randomly modifies one symbol in it
    // File names shortened to SSM - Similar Symbols Modification
    void generateSimilarSymbolsFiles(std::mt19937 &rng, const std::string &validSymbols) {
        std::filesystem::path testDir("../data/input/test/SSM/");
        ensureTestFolders(testDir);

        std::filesystem::path baseFilePathNoTxt{manyRandomSymbolsFileGen(rng, testDir, validSymbols)};

        for (int i = 0; i < 10; i++) {
            modifyRandomSymbol(rng, baseFilePathNoTxt, validSymbols);
        }
    }

    void generateEmptyFile() {
        std::filesystem::path testDir("../data/input/test/EF/");
        ensureTestFolders(testDir);
        std::ofstream out(testDir.string() + "empty.txt");
        out << "";
        out.close();
    }

    // Helper functions

    void removeOldTestFiles() {
        std::vector<std::filesystem::path> testDirs = {
            "../data/input/test/OSF/",
            "../data/input/test/MRS/",
            "../data/input/test/SSM/",
            "../data/input/test/EF/"
        };
        for (const auto &dir: testDirs) {
            if (std::filesystem::exists(dir)) {
                for (const auto &entry: std::filesystem::directory_iterator(dir)) {
                    if (entry.is_regular_file()) {
                        std::filesystem::remove(entry.path());
                    }
                }
            }
        }
    }

    std::filesystem::path manyRandomSymbolsFileGen(std::mt19937 &rng,
                                                   const std::filesystem::path &testDir,
                                                   const std::string &validSymbols) {
        std::string outputContent{};
        int fileLenght = std::uniform_int_distribution<int>(1000, 10000)(rng);
        for (int i = 0; i < fileLenght; i++) {
            outputContent += getRandomSymbol(validSymbols, rng);
        }

        std::filesystem::path testFileNameNoTxt = testDir.string() + std::to_string(fileLenght) + "_sz";
        std::ofstream out{};
        openUniqueFile(testFileNameNoTxt, out);
        out << outputContent;
        out.close();
        return testFileNameNoTxt;
    }

    void modifyRandomSymbol(std::mt19937 &rng, const std::filesystem::path &testFileNameNoTxt,
                            const std::string &validSymbols) {
        // this one is vibe coded i was too lazy
        std::ifstream in(testFileNameNoTxt.string() + ".txt");
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        if (content.empty()) {
            std::cerr << "File is empty, cannot modify symbol." << std::endl;
            return;
        }
        std::uniform_int_distribution<size_t> dist(0, content.size() - 1);
        size_t indexToModify = dist(rng);
        content[indexToModify] = getRandomSymbol(validSymbols, rng);
        std::ofstream out{};
        openUniqueFile(testFileNameNoTxt, out);
        out << content;
        out.close();
    }

    void ensureTestFolders(const std::filesystem::path &dirPath) {
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directories(dirPath);
        }
    }


    void openUniqueFile(const std::filesystem::path &testFileNameNoTxt, std::ofstream &out) {
        if (!std::filesystem::exists(testFileNameNoTxt.string() + ".txt")) {
            out.open(testFileNameNoTxt.string() + ".txt");
        } else {
            int i{2};
            while (std::filesystem::exists(testFileNameNoTxt.string() + std::to_string(i) + ".txt")) {
                i++;
            }
            out.open(testFileNameNoTxt.string() + std::to_string(i) + ".txt");
        }
    }

    char getRandomSymbol(const std::string &validSymbols, std::mt19937 &rng) {
        std::uniform_int_distribution<size_t> dist(0, validSymbols.size() - 1);
        return validSymbols[dist(rng)];
    }
}
