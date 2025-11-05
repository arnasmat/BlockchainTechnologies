//
// Created by arnas on 11/4/25.
//

#ifndef BCLIREADER_H
#define BCLIREADER_H
#include <filesystem>
#include <vector>


class User;
class Block;
class Transaction;

struct BlockData {
    std::string minerPublicKey;
    time_t timestamp;
    unsigned int height;
    std::string version;
    unsigned int nonce;
    unsigned short difficultyTarget;
    std::vector<Transaction *> transactions;
    std::string expectedBlockHash;
};

struct TransactionData {
    std::string transactionId;
    std::string senderPublicKey;
    time_t timestamp;
    std::vector<std::pair<double, std::string> > outputs; // amount, receiverPublicKey
};


TransactionData readTransactionFromFile(const std::filesystem::path &txFile);

std::vector<Transaction *> readAllTransactionsFromDir(const std::filesystem::path &txDir);

bool isCoinbaseTransaction(const TransactionData &txData);


BlockData readBlockDataFromFile(const std::filesystem::path &blockFile,
                                const std::vector<Transaction *> &allTransactions);

Block *reconstructAndVerifyBlock(const BlockData &blockData, Block *previousBlock);

std::vector<User *> readUsersFromFile(const std::filesystem::path &usersFile);


#endif //BCLIREADER_H
