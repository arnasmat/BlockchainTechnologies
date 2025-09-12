//
// Created by arnas on 9/12/25.
//

#ifndef HUMANHASH_H
#define HUMANHASH_H

#include <bitset>
#include <sstream>
#include <string>

// TODO: this is a random stupid hash idea that hasn't been tested lmso
class HumanHash {
    std::string humanHash(const std::string input) {
        int hash[64] = {0};
        for (const char c : input) {
            std::string bin = std::bitset<8>(c).to_string();
            for (int i=0; i<8; i++) {
                for (int j=0; j<8; j++) {
                    if (bin[i]) {
                        hash[i*j] += 1;
                    } else {
                        hash[i*j] += hash[i*j-1];
                    }
                }
            }
        }
        std::string output;

        for (int i : hash) {
            std::stringstream ss;
            ss << std::hex << i;
            output += ss.str();
        }

        return output;
    }

};



#endif //HUMANHASH_H
