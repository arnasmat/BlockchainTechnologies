#ifndef TRANSACTIONQUEUE_H
#define TRANSACTIONQUEUE_H

#include <vector>

#include "Transaction.h"
#include "User.h"
#include "UTXO.h"
#include "HeadBlock.h"

namespace TransactionQueue {
    void findPossibleMempoolTransaction(std::vector<Transaction *> &mempool);

    std::vector<Transaction *> pickValidTransactions(std::vector<Transaction *> &mempool, int number);

    void freeMempoolFromMinedTransaction(std::vector<Transaction *> &mempool);
}

#endif
