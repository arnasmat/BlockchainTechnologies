#ifndef ROOTBLOCK_H
#define ROOTBLOCK_H

#include "Blockchain.h"
#include "SystemAlgorithm.h"
#include "general.h"
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

    Block *getHeadBlock() {
        return headBlock;
    }

    void updateHeadBlock(Block *newHead) {
        if (newHead->finaliseBlock()) {
            for (auto &transaction: newHead->getTransactions()) {
                transaction->updateTransactionUtxosAfterBeingFinalised();
            }
            headBlock = newHead;
        }

        // headBlock = newHead;
        // int currentHeight = headBlock->getHeight();
        // if(currentHeight >= NUMBER_OF_BLOCKS_IN_FRONT_TO_VALIDATE) {
        //     Block *finalisableBlock = headBlock;
        //     while(finalisableBlock->getHeight() > currentHeight - NUMBER_OF_BLOCKS_IN_FRONT_TO_VALIDATE) {
        //         finalisableBlock = finalisableBlock->getPreviousBlock();
        //     }
        //     if(finalisableBlock->finaliseBlock()) {
        //         for(auto &transaction : finalisableBlock->getTransactions()) {
        //             transaction->updateTransactionUtxosAfterBeingFinalised();
        //         }
        //     }
        // }
    }
};

#endif
