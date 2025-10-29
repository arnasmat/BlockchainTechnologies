#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <vector>

#include "Blockchain/Transaction.h"
#include "Blockchain/User.h"
#include "Blockchain/UTXO.h"

namespace blockchainRandomGenerator {
    std::vector<User *> generateUsers(const unsigned int numberOfUsers) {
        std::vector<User *> users;
        for (int i = 1; i <= numberOfUsers; i++) {
            users.push_back(new User(i));
        }
        return users;
    }

    std::vector<Transaction *> generateValidTransactions(
        const std::vector<User *> &users, const unsigned int amountOfTransactions) {
        std::vector<Transaction *> transactions;
        transactions.reserve(amountOfTransactions);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> userDistrib(0, users.size() - 1);
        int i{0};
        while (i < amountOfTransactions) {
            const User *sender{users[userDistrib(gen)]};
            double senderBalance{UtxoSystem::getInstance().getBalanceOfPublicKey(sender->getPublicKey())};

            if (senderBalance == 0) {
                continue;
            }
            std::uniform_real_distribution<double> amountDistrib(std::min(1.0, senderBalance), senderBalance);
            double amount{amountDistrib(gen)/10};

            const User *receiver{users[userDistrib(gen)]};
            Transaction *transaction = new Transaction(sender->getPublicKey(), receiver->getPublicKey(), amount);
            std::vector<Utxo *> chosenUtxos = std::move(UtxoSystem::getInstance().findUtxosThatSatisfySum(sender->getPublicKey(), amount));
            if(chosenUtxos.size()) { 
                transaction->fillTransaction(chosenUtxos);
                transactions.push_back(transaction);
            } else {
                delete transaction;
            }
            i++;
        }
        return transactions;
    }
}


#endif //RANDOMGENERATOR_H
