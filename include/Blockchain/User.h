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
    unsigned int id{}; //Instead of vardas, works like private key
    std::string publicKey{};

public:
    User(const int id): id(id) {
        publicKey = hash->generateHash(std::to_string(id));
    }

    std::string getPublicKey() const {
        return publicKey;
    }

    std::string makeSignature(std::string content) {
        return hash->generateHash(hash->generateHash(std::to_string(id)) + content);
    }

    bool verifyUtxo(std::string transactionId, unsigned int vout, std::string utxoId) {
        return hash->generateHash(transactionId + std::to_string(vout) + hash->generateHash(std::to_string(id))) == utxoId;
    }
    
};

#endif //USER_H
