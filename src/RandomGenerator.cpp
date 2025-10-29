#include "RandomGenerator.h"
#include "Blockchain/User.h"
#include "Blockchain/Transaction.h"
#include "Blockchain/UTXOSystem.h"
#include <random>

namespace blockchainRandomGenerator {
    std::vector<User *> generateUsers(const unsigned int numberOfUsers) {
        std::vector<User *> users;
        users.reserve(numberOfUsers);
        for (unsigned int i = 1; i <= numberOfUsers; i++) {
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
        unsigned int i = 0;
        while (i < amountOfTransactions) {
            const User *sender = users[userDistrib(gen)];
            double senderBalance = UtxoSystem::getInstance().getBalanceOfPublicKey(sender->getPublicKey());

            if (senderBalance == 0) {
                continue;
            }
            std::uniform_real_distribution<double> amountDistrib(1.0, senderBalance);
            double amount = amountDistrib(gen);

            const User *receiver = users[userDistrib(gen)];
            Transaction *transaction = new Transaction(
                sender->getPublicKey(), receiver->getPublicKey(),
                amount,
                UtxoSystem::getInstance().findUtxosThatSatisfySum(sender->getPublicKey(), amount)
            );
            transactions.push_back(transaction);
            i++;
        }
        return transactions;
    }
}
