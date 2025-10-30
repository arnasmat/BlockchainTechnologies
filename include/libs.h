#ifndef LIBS_H
#define LIBS_H

#include <cmath>
#include <vector>
#include <iostream>
#include <random>

constexpr std::string SYSTEM_NAME{"SYSTEM"};

constexpr unsigned int DIFFICULTY_TARGET_INCREASE_INTERVAL = 10;
constexpr unsigned int DEFAULT_DIFFICULTY = 12; // bits
constexpr unsigned int TARGET_BLOCK_TIME = 5; //sec
constexpr unsigned int TARGET_TOLERANCE = 2;
constexpr unsigned int INITIAL_REWARD = 100;
constexpr unsigned int HEIGHT_FOR_HALVING_REWARD = 20;

constexpr unsigned int BLOCKS_BEFORE_TRANSACTIONS = 50;

#endif //LIBS_H
