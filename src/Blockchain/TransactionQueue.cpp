#include "Blockchain/TransactionQueue.h"

#include "Blockchain/Transaction.h"
#include "Blockchain/User.h"
#include "Blockchain/UTXO.h"
#include "Blockchain/HeadBlock.h"

#include <vector>

namespace TransactionQueue {
    void findPossibleMempoolTransaction(std::vector<Transaction *> &mempool) {
        std::vector<Transaction *> newMempool{};
        std::map<std::string, double> userWantsToSend;

        for (auto transaction: mempool) {
            if (!userWantsToSend.contains(transaction->getSenderPublicKey())) {
                userWantsToSend[transaction->getSenderPublicKey()] = transaction->getOutputs()[0].first;
            } else {
                userWantsToSend[transaction->getSenderPublicKey()] += transaction->getOutputs()[0].first;
            }
            if (UtxoSystem::getInstance().getBalanceOfPublicKey(transaction->getSenderPublicKey()) >= userWantsToSend[
                    transaction->getSenderPublicKey()]) {
                newMempool.push_back(transaction);
            }
        }
        mempool = std::move(newMempool);
    }

    std::vector<Transaction *> pickValidTransactions(std::vector<Transaction *> &mempool, int number) {
        std::vector<Transaction *> validMempool{};

        for (auto transaction: mempool) {
            if (validMempool.size() >= number) {
                break;
            }
            std::vector<Utxo *> chosenUtxos = UtxoSystem::getInstance().findUtxosThatSatisfySum(
                transaction->getSenderPublicKey(), transaction->getOutputs()[0].first
            );
            if (!chosenUtxos.empty()) {
                transaction->fillTransaction(chosenUtxos);
                validMempool.push_back(transaction);
            }
        }
        return validMempool;
    }

    void freeMempoolFromMinedTransaction(std::vector<Transaction *> &mempool) {
        std::vector<Transaction *> newMempool{};

        std::vector<Transaction *> recentlyUsedTransactions = HeadBlock::getInstance().getHeadBlock()->
                getTransactions();
        for (auto &transaction: mempool) {
            if (std::find(recentlyUsedTransactions.begin(), recentlyUsedTransactions.end(), transaction) ==
                recentlyUsedTransactions.end()) {
                newMempool.push_back(transaction);
            }
        }
        mempool = std::move(newMempool);
    }
}
