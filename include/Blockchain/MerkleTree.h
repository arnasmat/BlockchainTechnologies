#ifndef MERKLETREE_H
#define MERKLETREE_H
#include <vector>

#include "HashAlg/HashGenInterface.h"
#include "general.h"
#include "SystemAlgorithm.h"
#include "Transaction.h"

class MerkleTree : SystemAlgorithm {
private:
    std::string RootMerkleTreeHash{};

public:
    MerkleTree() {
    }

    std::string calculateMerkleTreeHash(const std::vector<Transaction *> &blockTransactions) {
        if (blockTransactions.size() == 0) {
            return hash->generateHash("");
        }

        std::vector<std::string> transactions{};
        for (const auto blockTransaction: blockTransactions) {
            transactions.push_back(hash->generateHash(blockTransaction->getTransactionId()));
        }

        while (transactions.size() > 1) {
            std::vector<std::string> tempTransactions{};
            tempTransactions.reserve(1 + transactions.size() / 2);

            if (transactions.size() % 2 == 1) {
                transactions.push_back(transactions.back());
            }

            for (size_t i = 0; i < transactions.size(); i += 2) {
                tempTransactions.push_back(hash->generateHash(transactions[i] + transactions[i+1]));
            }

            transactions = std::move(tempTransactions);
        }

        RootMerkleTreeHash = transactions[0];
        return RootMerkleTreeHash;
    }
};

#endif
