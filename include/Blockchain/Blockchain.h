#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <vector>

#include "SystemAlgorithm.h"
#include "Transaction.h"


class Block : public SystemAlgorithm {
    std::string previousBlockHash;
    const unsigned int timestamp;
    const double version;
    const unsigned int nonce;
    std::string merkleRootHash;
    const unsigned short int difficultyTarget;

    std::vector<Transaction *> transactions;

public:
    Block(Block *previousBlock, const unsigned int timestamp, const double version, const unsigned int nonce,
          const unsigned short int difficultyTarget, std::vector<Transaction *> transactions)
        : SystemAlgorithm(),
          previousBlockHash(hash->generateHash(previousBlock->getBlockAsString())),
          timestamp(timestamp),
          version(version),
          nonce(nonce),
          merkleRootHash(""), // TODO
          difficultyTarget(std::min(difficultyTarget, MAX_HASH_LENGTH)),
          transactions(std::move(transactions)) {
    }

    std::string getBlockAsString() const {
        std::string output =
        previousBlockHash + std::to_string(timestamp) + std::to_string(version) +
                 std::to_string(nonce) + merkleRootHash + std::to_string(difficultyTarget);
        for (Transaction* i: transactions) {
            output += i->getTransactionId();
        }
        return output;
    }
};


#endif //BLOCKCHAIN_H
