//
// Created by arnas on 9/12/25.
//

#ifndef HASHGENINTERFACE_H
#define HASHGENINTERFACE_H
#include <string>

class HashGenInterface {
public:
    virtual ~HashGenInterface() = default;
    virtual std::string generateHash(const std::string& input) const = 0;
};




#endif //HASHGENINTERFACE_H
