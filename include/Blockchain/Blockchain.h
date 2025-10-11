#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <vector>

#include "SystemAlgorithm.h"
#include "Transaction.h"

constexpr unsigned int DIFFICULTY_TARGET_INCREASE_INTERVAL = 10;

class Block : public SystemAlgorithm {
    const Block *previousBlock;
    std::string previousBlockHash;
    const time_t timestamp;
    const double version;
    const unsigned int nonce;
    std::string merkleRootHash;
    unsigned short int difficultyTarget;

    std::vector<Transaction *> transactions;

public:
    Block(const Block *previousBlock, const time_t timestamp, const double version, const unsigned int nonce,
          std::vector<Transaction *> transactions)
        : previousBlock(previousBlock),
          previousBlockHash(previousBlock
                                ? previousBlock->getBlockHash()
                                : "0000000000000000000000000000000000000000000000000000000000000000"),
          timestamp(timestamp),
          version(version),
          nonce(nonce),
          merkleRootHash(""), // TODO
          transactions(std::move(transactions)) {
        difficultyTarget = std::min(
            3 + ((previousBlock ? previousBlock->getHeight() + 1 : 0) / DIFFICULTY_TARGET_INCREASE_INTERVAL),
            static_cast<unsigned int>(MAX_HASH_LENGTH)
        );

    }

    std::string getBlockAsString() const {
        std::string output =
                previousBlockHash + std::to_string(timestamp) + std::to_string(version) +
                std::to_string(nonce) + merkleRootHash + std::to_string(difficultyTarget);
        for (Transaction *i: transactions) {
            output += i->getTransactionId();
        }
        return output;
    }

    std::string getBlockHash() const {
        return hash->generateHash(getBlockAsString());
    }

    unsigned int getDifficultyTarget() const {
        return difficultyTarget;
    }

    unsigned int getHeight() const {
        unsigned int height = 0;
        const Block *current = previousBlock;
        while (current) {
            height++;
            current = current->previousBlock;
        }
        return height + 1;
    }
};

// TODO: class block


#endif //BLOCKCHAIN_H
