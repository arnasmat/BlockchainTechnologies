#ifndef LIBS_H
#define LIBS_H

#include <cmath>
#include <filesystem>
#include <vector>
#include <iostream>
#include <random>

enum HashAlgorithm {
    HUMAN,
    VIBE,
    MATRIX
};

constexpr unsigned short getHashAlgorithmSize(HashAlgorithm alg) {
    switch (alg) {
        case HUMAN: return 64;
        case VIBE: return 64;
        case MATRIX: return 64;
    }
    return 0;
}

static const char *hashAlgorithmToString(HashAlgorithm a) {
    switch (a) {
        case HUMAN: return "HUMAN";
        case VIBE: return "VIBE";
        case MATRIX: return "MATRIX";
    }
    throw "ERROR";
}

constexpr HashAlgorithm HASH_TYPE{MATRIX};
constexpr unsigned short HASH_LENGTH{getHashAlgorithmSize(HASH_TYPE)};
constexpr std::string SYSTEM_NAME{"SYSTEM"};
constexpr std::string SYSTEM_VERSION{"1.0"};


const std::string FILE_EXTENTION{".blkinf"};
const std::filesystem::path DEFAULT_OUTPUT_DIR{"output"};
const std::filesystem::path USERS_FILE{"users" + FILE_EXTENTION};
const std::filesystem::path TRANSACTIONS_FILE{"transactions" + FILE_EXTENTION};
const std::filesystem::path BLOCKS_DIRECTORY{"blocks"};
const std::string BLOCKS_FILE{"_block" + FILE_EXTENTION};
const std::filesystem::path METADATA_FILE{"metadata" + FILE_EXTENTION};

constexpr unsigned int DIFFICULTY_TARGET_INCREASE_INTERVAL = 10;
constexpr unsigned int DEFAULT_DIFFICULTY = 12; // bits
constexpr unsigned int TARGET_BLOCK_TIME = 5; //sec
constexpr unsigned int TARGET_TOLERANCE = 2;
constexpr unsigned int INITIAL_REWARD = 100;
constexpr unsigned int HEIGHT_FOR_HALVING_REWARD = 20;

constexpr unsigned int BLOCKS_BEFORE_TRANSACTIONS = 50;


#endif //LIBS_H
