#include <iostream>
#include <queue>
#include <random>

#include "Blockchain/Blockchain.h"
#include "Blockchain/User.h"

int main() {
    Block *previousBlock = nullptr;
    unsigned int blockCount = 0;
    std::vector<User*> users;
    for (int i = 1; i <= 1000; i++) {
        users.push_back(new User(i));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, users.size() - 1);

    std::queue<Transaction*> transactionList;

    // Mine 50 blocks to introduce something to the economy lol
    while (blockCount < 50) {
        int nonce = 0;
        Block *newBlock = nullptr;
        User* miner = users.at(distrib(gen));

        // Mine a block that meets the difficulty target
        while (true) {
            newBlock = new Block(previousBlock, miner->getPublicKey(), time(nullptr), 1.0, nonce++, {});
            if (newBlock->isBlockValid()) {
                std::cout << "Block " << blockCount
                        << "\nHash " << newBlock->getBlockHash()
                        << "\nHeight " << newBlock->getHeight()
                        << "\nTimestamp " << newBlock->getTimestamp()
                        << "\nDifficulty " << newBlock->getDifficultyTarget()
                        << "\nReward " << newBlock->calculateBlockReward();
                const auto txs = newBlock->getTransactions();
                for (const auto& tx: txs) {
                    std::cout << "\nTransaction " << tx << ": "
                            << "From: " << tx->getSenderPublicKey()
                            << ", To: " << tx->getReceiverPublicKey()
                            << ", Amount: " << tx->getAmount()
                            << "\n";
                    transactionList.push(tx);
                }
                std::cout << "\n----------------------\n";
                break;
            }
            delete newBlock;
        }

        previousBlock = newBlock;
        blockCount++;
    }

    std::cout << "\nFinal block info:\n";
    std::cout << "Hash: " << previousBlock->getBlockHash() << "\n";
    std::cout << "Height: " << previousBlock->getHeight() << "\n";
    std::cout << "Timestamp: " << previousBlock->getTimestamp() << "\n";
    std::cout << "Difficulty: " << previousBlock->getDifficultyTarget() << "\n";

    return 0;
}