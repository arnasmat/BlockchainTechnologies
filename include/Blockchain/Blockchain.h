#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <cmath>
#include <vector>

#include "Helper.h"
#include "SystemAlgorithm.h"
#include "Transaction.h"

// TODO: move all these defines somewhere else ig? idk
constexpr unsigned int DIFFICULTY_TARGET_INCREASE_INTERVAL = 10;
constexpr unsigned int DEFAULT_DIFFICULTY = 12; // bits
constexpr unsigned int TARGET_BLOCK_TIME = 5; //sec
constexpr unsigned int TARGET_TOLERANCE = 2;
constexpr unsigned int INITIAL_REWARD = 100;
constexpr unsigned int HEIGHT_FOR_HALVING_REWARD = 20;
// TODO: make the difficulty increase depending on the timestamp of the block that was 4 before

class Block : public SystemAlgorithm {
    // Header
    const Block *previousBlock;
    std::string previousBlockHash;
    std::string minerPublicKey;
    const time_t timestamp;
    const float version;
    const unsigned int nonce;
    std::string merkleRootHash;
    const unsigned short int difficultyTarget;

    // Body
    std::vector<Transaction *> transactions;

public:
    Block(const Block *previousBlock, const std::string &minerPk, const time_t timestamp, const float version,
          const unsigned int nonce,
          std::vector<Transaction *> transactions)
        : previousBlock(previousBlock),
          previousBlockHash(previousBlock
                                ? previousBlock->getBlockHash()
                                : "0000000000000000000000000000000000000000000000000000000000000000"),
          minerPublicKey(minerPk),
          timestamp(timestamp),
          version(version),
          nonce(nonce),
          difficultyTarget(calculateDifficulty()),
          merkleRootHash(""), // TODO
          transactions(std::move(transactions)) {
        // TODO: implement the reward transaction ty
        transactions.insert(transactions.begin(), new Transaction("SYSTEM", minerPk, calculateBlockReward()));
    }

    std::string getBlockAsString() const {
        // TODO: make merkleroothash work and then transaction hashes will work!
        return previousBlockHash + std::to_string(timestamp) + std::to_string(version) +
               std::to_string(nonce) + merkleRootHash + std::to_string(difficultyTarget);
    }

    std::string getBlockHash() const {
        return hash->generateHash(getBlockAsString());
    }

    unsigned int getDifficultyTarget() const {
        return difficultyTarget;
    }

    unsigned int getHeight() const {
        // should it just be precalculated or should it be verified each time the function is called? idk!
        unsigned int height = 0;
        const Block *current = previousBlock;
        while (current) {
            height++;
            current = current->previousBlock;
        }
        return height + 1;
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
        return true;
    }

    double calculateBlockReward() const {
        return static_cast<double>(INITIAL_REWARD / std::pow(
                                       2, static_cast<int>(getHeight() / HEIGHT_FOR_HALVING_REWARD)));
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
