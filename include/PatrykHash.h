#ifndef PATRYKHASH_H
#define PATRYKHASH_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>
#include <chrono>

#include "HashGenInterface.h"

class PatrykHash: public HashGenInterface {
    std::string Random_String (int length)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(65, 122); // A-65, z-122

        std::string string = "";

        for (int i = 0; i<length; i++)
        {
            char c = distr(gen);
            string+=c;
        }

        return string;
    }

    std::string Random_String ()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(65, 122); // A-65, z-122
        std::uniform_int_distribution<> l_distr(500, 1500); // A-65, z-122

        std::string string = "";
        int length = l_distr(gen);

        for (int i = 0; i<length; i++)
        {
            char c = distr(gen);
            string+=c;
        }

        return string;
    }

    std::string Similar_String (std::string string)
    {
        char old_c = string.back();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(65, 122); // A-65, z-122

        char new_c = distr(gen);

        while (new_c == old_c)  {new_c = distr(gen);}

        string.pop_back();
        string.push_back(new_c);

        return string;
    }

    std::string Hash (const std::string& input) const
    {
        int output [64];
        std::fill_n(output, 64, 15);

        int sum = 0;

        for (char character : input)
        {
            std::mt19937 chargen(character);
            sum+=chargen();
        };

        std::mt19937 gen(sum);
        std::uniform_int_distribution<> distr(0, 15);

        std::stringstream stream;
        for (int num : output)
        {
            stream << std::hex << num-distr(gen);
        };

        return stream.str();
    }
public:
    std::string generateHash(const std::string &input) const override {
        return Hash(input);
    }
};
#endif //PATRYKHASH_H
