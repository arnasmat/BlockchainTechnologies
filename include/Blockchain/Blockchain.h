#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Helper.h"
#include "MerkleTree.h"
#include "libs.h"
#include "Transaction.h"

class Block : public SystemAlgorithm {
    // Header
    MerkleTree merkleTree;
    const Block *previousBlock;
    std::string previousBlockHash;
    std::string minerPublicKey;
    time_t timestamp;
    const unsigned int height;
    const std::string version;
    unsigned int nonce;
    std::string merkleRootHash;
    const unsigned short int difficultyTarget;
    std::atomic<bool> isFinal= false;

    // Body
    std::vector<Transaction *> transactions;

public:
    Block(const Block *previousBlock, const std::string &minerPk, const std::string version,
          const unsigned int nonce,
          const std::vector<Transaction *> &transactions)
        : previousBlock(previousBlock),
          previousBlockHash(previousBlock
                                ? previousBlock->getBlockHash()
                                : "0000000000000000000000000000000000000000000000000000000000000000"),
          minerPublicKey(minerPk),
          height(previousBlock ? previousBlock->getHeight() + 1 : 0),
          version(version),
          nonce(nonce),
          difficultyTarget(calculateDifficulty()),
          transactions(transactions) {
        timestamp = time(nullptr);
        // TODO: terrible to insert it at the start but whatever, max n is 100
        this->transactions.insert(this->transactions.begin(),
                                  new Transaction(SYSTEM_NAME, minerPk, calculateBlockReward()));
        merkleRootHash = merkleTree.calculateMerkleTreeHash(this->transactions);
    }
    //since block is created and deleted many times, we need the transactions which are being passed and cannot use std::move in constructor

    std::string getBlockAsString() const {
        return previousBlockHash + std::to_string(timestamp) + version +
               std::to_string(nonce) + merkleRootHash + std::to_string(difficultyTarget);
    }

    std::string getBlockHash() const {
        return hash->generateHash(getBlockAsString());
    }

    unsigned int getDifficultyTarget() const {
        return difficultyTarget;
    }

    unsigned int getHeight() const {
        return height;
    }

    std::string getMinerPublicKey() const {
        return minerPublicKey;
    }

    Block *getPreviousBlock() const {
        // const_cast is the stupidest thing I have ever seen but ok
        return const_cast<Block *>(previousBlock);
    }

    unsigned int verifyHeight() const {
        // idk if we need this, bet for security or whatever lol
        if (!previousBlock) {
            return 0;
        }
        return previousBlock->verifyHeight() + 1;
    }

    time_t getTimestamp() const {
        return timestamp;
    }

    bool isBlockValid() const {
        std::bitset<256> hashBits = hashToBits(getBlockHash());
        for (unsigned int i = 0; i < difficultyTarget; i++) {
            if (hashBits[i]) {
                return false;
            }
        }

        if (verifyHeight() == height) {
            return true;
        }
        std::cerr << "Transaction height couldn't be verified";
        return false;
    }

    double calculateBlockReward() const {
        return static_cast<double>(INITIAL_REWARD / std::pow(
                                       2, static_cast<int>(getHeight() / HEIGHT_FOR_HALVING_REWARD)));
    }

    std::vector<Transaction *> getTransactions() const {
        return transactions;
    }

    void updateNonce(int updateBy = 1) {
        nonce += updateBy;
    }

    bool finaliseBlock() {
        bool expected = false;
        return isFinal.compare_exchange_strong(expected, true);
    }

private:
    unsigned short int calculateDifficulty() const {
        // If no previous block or not enough history, start with initial difficulty
        if (!previousBlock || getHeight() < DIFFICULTY_TARGET_INCREASE_INTERVAL ||
            getHeight() % DIFFICULTY_TARGET_INCREASE_INTERVAL != 0) {
            return previousBlock ? previousBlock->getDifficultyTarget() : DEFAULT_DIFFICULTY;
        }

        const Block *blockAtInterval = previousBlock;
        for (int i = 0; i < DIFFICULTY_TARGET_INCREASE_INTERVAL - 1; i++) {
            if (!blockAtInterval->previousBlock) {
                return previousBlock->getDifficultyTarget();
            }
            blockAtInterval = blockAtInterval->previousBlock;
        }

        time_t timeDiff = previousBlock->getTimestamp() - blockAtInterval->getTimestamp();
        double averageBlockTime = static_cast<double>(timeDiff) / DIFFICULTY_TARGET_INCREASE_INTERVAL;
        unsigned short int currentDifficulty = previousBlock->getDifficultyTarget();

        if (averageBlockTime > TARGET_BLOCK_TIME + TARGET_TOLERANCE) {
            return std::max(1, static_cast<int>(currentDifficulty) - 1);
        }
        if (averageBlockTime < TARGET_BLOCK_TIME - TARGET_TOLERANCE) {
            return std::min(static_cast<int>(currentDifficulty) + 1, static_cast<int>(MAX_HASH_LENGTH) * 4);
        }

        return currentDifficulty;
    }
};

// TODO: class block


#endif //BLOCKCHAIN_H
