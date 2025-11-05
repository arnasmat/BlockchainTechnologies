#ifndef USER_H
#define USER_H
#include <list>
#include <string>

#include "SystemAlgorithm.h"
#include "HashAlg/HashGenInterface.h"
#include "UTXOSystem.h"

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

    User(const std::string &pk): publicKey(pk) {
    }

    std::string getPublicKey() const {
        return publicKey;
    }

    unsigned int getEstimatedBalance() const {
        return estimatedBalance;
        // no lmao this doesnt owkr
    }

    void setEstimatedBalance() {
        estimatedBalance = UtxoSystem::getInstance().getBalanceOfPublicKey(hash->generateHash(std::to_string(id)));
        //tik zinantis private key gali gauti balansa
    }
};

#endif //USER_H
