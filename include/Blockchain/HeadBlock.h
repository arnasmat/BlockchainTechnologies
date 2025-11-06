#ifndef ROOTBLOCK_H
#define ROOTBLOCK_H

#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "libs.h"
#include "Transaction.h"

#include <vector>
#include <algorithm>
#include <map>

//prolly would be better to have this class as blockchain manager which would also create blocks, etc. to fulfill SOLID but a bit too late
class HeadBlock : SystemAlgorithm {
private:
    Block *headBlock = nullptr;

public:
    static HeadBlock &getInstance() {
        static HeadBlock instance;
        return instance;
    }

    Block* getHeadBlock() {
        return headBlock;
    }

    bool validateMerkleRootInNewHeadBlock(Block *newHead) {
        std::vector<Transaction*> goodTransactions{};
        MerkleTree merkleTree;

        if(merkleTree.calculateMerkleTreeHash(newHead->getTransactions()) == newHead->getMerkleRootHash()) {
            return true;
        }
        return false;
    }

    void updateHeadBlock(Block *newHead) {
        if(newHead->finaliseBlock()) {
            for(auto &transaction : newHead->getTransactions()) {
                transaction->updateTransactionUtxosAfterBeingFinalised();
            }
            headBlock = newHead;
        } 
    }

};

#endif    