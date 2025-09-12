//
// Created by arnas on 9/12/25.
//

#ifndef HASHTESTS_H
#define HASHTESTS_H

#include <string>
#include <algorithm>
#include <random>
#include "HashGenInterface.h"

namespace HashTests {
    void determinismTest(const HashGenInterface* hashGen, const std::string& input);
    void collisionSearch(const HashGenInterface* hashGen);
}

#endif // HASHTESTS_H
