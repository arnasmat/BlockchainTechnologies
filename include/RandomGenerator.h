#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <vector>

#include "Blockchain/Transaction.h"
#include "Blockchain/User.h"
#include "Blockchain/UTXO.h"
#include "Blockchain/UserSystem.h"
#include "Blockchain/TransactionQueue.h"

namespace blockchainRandomGenerator {
    std::vector<User *> generateUsers(const unsigned int numberOfUsers) {
        for (int i = 1; i <= numberOfUsers; i++) {
            UserSystem::getInstance().createNewUser(i);
        }
        return UserSystem::getInstance().getSpecifiedNumberOfUsers(numberOfUsers);
    }

    std::vector<Transaction *> generateValidTransactions(
        const std::vector<User *> &users, const unsigned int amountOfTransactions) {
        std::vector<Transaction *> transactions;
        transactions.reserve(amountOfTransactions);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> userDistrib(0, users.size() - 1);
        int i{0};
        while (true) {
            while (i < amountOfTransactions) {
                const User *sender{users[userDistrib(gen)]};
                double senderBalance{UtxoSystem::getInstance().getBalanceOfPublicKey(sender->getPublicKey())};

                if (senderBalance == 0) {
                    continue;
                }
                std::uniform_real_distribution<double> amountDistrib(std::min(1.0, senderBalance), senderBalance);
                double amount{amountDistrib(gen)/amountOfTransactions};

                const User *receiver{users[userDistrib(gen)]};
                Transaction *transaction = new Transaction(sender->getPublicKey(), receiver->getPublicKey(), amount);
                transactions.push_back(transaction);  // ← FIXED: Actually add transaction to vector!
                i++;
            }
            TransactionQueue::findPossibleMempoolTransaction(transactions);
            if(transactions.size() >= amountOfTransactions) {
                break;
            } else {
                i = transactions.size();
            }
        }
        return transactions;
    }
}


#endif
