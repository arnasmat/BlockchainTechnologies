#ifndef MERKLETREE_H
#define MERKLETREE_H
#include "HashAlg/HashGenInterface.h"
#include "Transaction.h"

class MerkleTree : SystemAlgorithm {
    private:
        std::string RootMerkleTreeHash{};

    public:
        MerkleTree() {
             RootMerkleTreeHash = hash->generateHash("");
        }

        std::string calculateMerkleTreeHash(std::vector<Transaction *> blockTransactions) {
            std::vector<std::string> transactions{};
            for(int i=0; i<blockTransactions.size(); i++){
                transactions.push_back(hash->generateHash(blockTransactions[i]->getTransactionId()));
            }   
            while(transactions.size() > 1){
                std::vector<std::string> tempTransactions{};
                tempTransactions.reserve(1+transactions.size()/2);
                for(int i=0; i<transactions.size()/2; i++){
                    tempTransactions.push_back(hash->generateHash(transactions[2*i] + transactions[2*i+1]));
                }
                if(transactions.size() % 2 == 1){
                    tempTransactions.push_back(transactions.back());
                }
                transactions = std::move(tempTransactions);
            }
            RootMerkleTreeHash = transactions[0];
            return RootMerkleTreeHash;
        }   
}
#endif