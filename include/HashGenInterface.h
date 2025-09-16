#ifndef HASHGENINTERFACE_H
#define HASHGENINTERFACE_H

#include <bitset>
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
            output += "0123456789abcdef"[hash[i] % 16];
        }
        return output;
    }
public:
    explicit HashGenInterface(int inputKey = 0) : key(std::move(inputKey)) {}
    virtual ~HashGenInterface() = default;
    virtual std::string generateHash(const std::string& input) const = 0;
};




#endif //HASHGENINTERFACE_H
