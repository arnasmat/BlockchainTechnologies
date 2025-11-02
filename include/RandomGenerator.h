#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <vector>

#include "Blockchain/Transaction.h"
#include "Blockchain/User.h"
#include "Blockchain/UTXO.h"

namespace blockchainRandomGenerator {
    std::vector<User *> generateUsers(const unsigned int numberOfUsers);

    std::vector<Transaction *> generateValidTransactions(
        const std::vector<User *> &users, const unsigned int amountOfTransactions);
}


#endif
