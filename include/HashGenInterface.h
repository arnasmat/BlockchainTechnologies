#ifndef HASHGENINTERFACE_H
#define HASHGENINTERFACE_H

#include <bitset>
#include <iostream>
#include <sstream>
#include <string>

// TODO: rule of 5
// TODO: add input to change shit
class HashGenInterface {
public:
    virtual ~HashGenInterface() = default;
    virtual std::string generateHash(const std::string& input) const = 0;
};




#endif //HASHGENINTERFACE_H
