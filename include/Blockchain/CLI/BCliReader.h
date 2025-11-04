//
// Created by arnas on 11/4/25.
//

#ifndef BCLIREADER_H
#define BCLIREADER_H
#include <filesystem>
#include <vector>


class Block;
class Transaction;

struct BlockData {
    std::string previousBlockHash;
    std::string minerPublicKey;
    time_t timestamp;
    unsigned int height;
    std::string version;
    unsigned int nonce;
    unsigned short difficultyTarget;
    std::vector<Transaction *> transactions;
    std::string expectedBlockHash;
};

BlockData readBlockDataFromFile(const std::filesystem::path &blockFile,
                                const std::vector<Transaction *> &allTransactions);

Block *reconstructAndVerifyBlock(const BlockData &blockData, Block *previousBlock);


#endif //BCLIREADER_H
