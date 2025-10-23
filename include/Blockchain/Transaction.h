#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <vector> 
#include <string>
#include <utility>
#include <ctime>  

#include "SystemAlgorithm.h"
#include "HashAlg/HashGenInterface.h"
#include "UTXO.h"
#include "UTXOSystem.h"

class Transaction : SystemAlgorithm {
    std::string transactionId{};
    std::string senderPublicKey{};
    std::vector<std::pair<const Transaction *, unsigned int>> inputs; // transaction and index of valid output of other transaction
    std::vector<std::pair<double, std::string>> outputs; // amount and public key of receiver

public:
    Transaction(const std::string &senderPk, const std::string &receiverPk, const double amount, std::vector<Utxo*> chosenUtxos)
        : senderPublicKey(senderPk){
            std::string content{std::to_string(time(nullptr)+amount)};
            double achievedSum = 0;
            for(auto &eachUtxo : chosenUtxos){
                inputs.push_back({eachUtxo->getTransaction(), eachUtxo->getVout()});
                achievedSum += eachUtxo->getAmount();
                content += eachUtxo->getTransaction()->getTransactionId() + std::to_string(time(nullptr)); //so that every transasctionID be different
                UtxoSystem::getInstance().deleteUtxo(senderPk, eachUtxo);
            }
            outputs.push_back({amount, receiverPk});
            if(senderPk != "SYSTEM") {
                double fee = achievedSum - amount; 
                if(fee > 0){
                    content += std::to_string(fee);
                    outputs.push_back({fee, senderPk}); // so far it is simply being sent back to sender as change
                }  
            }    
        transactionId = hash->generateHash(senderPublicKey + receiverPk + content);    
        UtxoSystem::getInstance().addNewUtxos(outputs, this);           
    }

    std::string getTransactionId() const {
        return transactionId;
    } 

    std::string getSenderPublicKey() const {
        return senderPublicKey;
    }

    const std::vector<std::pair<const Transaction *, unsigned int>>& getInputs() const {
        return inputs;
    }

    const std::vector<std::pair<double, std::string>>& getOutputs() const {
        return outputs;
    }

};

#endif //TRANSACTION_H
