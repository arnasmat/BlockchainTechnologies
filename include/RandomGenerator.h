#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <vector>

class User;
class Transaction;

namespace blockchainRandomGenerator {
    std::vector<User *> generateUsers(unsigned int numberOfUsers);

    std::vector<Transaction *> generateValidTransactions(
        const std::vector<User *> &users, unsigned int amountOfTransactions);
}

#endif //RANDOMGENERATOR_H
