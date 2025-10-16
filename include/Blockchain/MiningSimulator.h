//
// Created by arnas on 10/16/25.
//

#ifndef MININGSIMULATOR_H
#define MININGSIMULATOR_H
#include <queue>
#include <random>
#include <vector>

#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "User.h"


class MiningSimulator: SystemAlgorithm {
    std::vector<User*> users;
    bool isMining{};
public:

    MiningSimulator(const std::vector<User*> &_users): users(_users) {
        isMining = true;
    };

    Block* mineBlock(
        std::queue<Transaction*>& transactionList,
        const Block* previousBlock
    ) {
        const User* miner{pickMiner()};
        int nonce{0};

        Block* newBlock{nullptr};

        while (isMining) {
            newBlock = new Block(previousBlock, miner->getPublicKey(), time(nullptr), SYSTEM_VERSION, nonce++, {});
            if (newBlock->isBlockValid()) {
                // transactionList.push(tx);
                // TODO
                announceNewBlock(newBlock);
                break;
            }
            delete newBlock;
        }

        return newBlock;
    }

    User* pickMiner() {
        static std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, users.size() - 1);
        return users.at(distrib(gen));
    }

    void announceNewBlock(const Block* newBlock) {
        std::stringstream ss;
        ss << "A new Block was just mined by User " << newBlock->getMinerPublicKey() << "\n"
                << "Hash " << newBlock->getBlockHash() << "\n"
                << "Height " << newBlock->getHeight() << "\n"
                << "Timestamp " << newBlock->getTimestamp() << "\n"
                << "Difficulty " << newBlock->getDifficultyTarget() << "\n"
                << "Reward " << newBlock->calculateBlockReward() << "\n";
        const auto txs = newBlock->getTransactions();
        ss << "This block contains the following transactions: \n";
        for (const auto& tx: txs) {
            ss << "\nTransaction " << tx->getTransactionId() << ": \n"
                    << "From: " << tx->getSenderPublicKey()
                    << ", To: " << tx->getReceiverPublicKey()
                    << ", Amount: " << tx->getAmount()
                    << "\n";
        }
        ss << "\n----------------------\n";

        std::cout<<ss.str();
    }

};



#endif //MININGSIMULATOR_H
