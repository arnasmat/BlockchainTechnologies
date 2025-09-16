#ifndef HASHGENINTERFACE_H
#define HASHGENINTERFACE_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

// TODO: rule of 5
// TODO: add input to change shit
class HashGenInterface {
protected:
    int key;
    static std::string castToHex(const int hash[], const size_t size) {
        std::string output;
        for (int i=0; i<size; i++) {
            output += "0123456789abcdef"[abs(hash[i]) % 16];
        }
        return output;
    }
public:
    explicit HashGenInterface(int inputKey = 0) : key(inputKey) {}
    virtual ~HashGenInterface() = default;
    virtual std::string generateHash(const std::string& input) const = 0;
    std::string hashFromFile(const std::filesystem::path& path) const {
        std::ifstream in(path);
        if (!in) {
            std::cerr << "Error: Could not open file " << path << std::endl;
            return "ERROR";
        }

        std::ostringstream inputBuffer;
        inputBuffer << in.rdbuf();
        std::string input = inputBuffer.str();
        return generateHash(input);
    }
};




#endif //HASHGENINTERFACE_H
