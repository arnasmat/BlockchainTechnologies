#ifndef USER_H
#define USER_H
#include <list>
#include <string>

#include "SystemAlgorithm.h"
#include "Transaction.h"
#include "HashAlg/HashGenInterface.h"

// NOTE: This class is meant to represent a node's knowledge about a certain user in the blockchain network
// -> Therefore using it you have to handle setting/getting values yourself.
class User : SystemAlgorithm {
    unsigned int id{}; //Instead of vardas
    unsigned int estimatedBalance{0};
    std::string publicKey{};

public:
    User(const int id): id(id) {
        publicKey = hash->generateHash(std::to_string(id));
    }

    std::string getPublicKey() const {
        return publicKey;
    }

    unsigned int getEstimatedBalance() const {
        return estimatedBalance;
    }

    void setEstimatedBalance(const unsigned int value) {
        estimatedBalance = value;
    }
};

#endif //USER_H
