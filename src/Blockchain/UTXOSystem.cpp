#include "Blockchain/UTXOSystem.h"
#include "Blockchain/Transaction.h"
#include "Blockchain/UserSystem.h"
#include "Blockchain/User.h"
#include "Blockchain/UTXO.h"
#include <algorithm>

std::vector<Utxo *> UtxoSystem::findUtxosThatSatisfySum(const std::string &sendersPK, double neededAmount) {
    if (userUtxos.find(sendersPK) == userUtxos.end()) {
        return {};
    }

    std::sort(userUtxos[sendersPK].begin(), userUtxos[sendersPK].end(), [](Utxo *x, Utxo *y) {
        return x->getAmount() > y->getAmount();
    });

    std::vector<Utxo *> pendingUtxos{};
    double achievedSum = 0;

    // get the user instance of the sendersPK
    User *sender = UserSystem::getInstance().findUserMatchingPublicKey(sendersPK);
    if (sender == nullptr) {
        return {};  // prolly will never happen, just in case
    }

    //this utxo reservation also checks if utxo really belongs to that user
    for (auto &utxo: userUtxos[sendersPK]) {
        if (achievedSum < neededAmount) {
            if(utxo->reserveUtxo()) {
                if(sender->verifyUtxo(utxo->getTransaction()->getTransactionId(), utxo->getVout(), utxo->getId())) {
                    achievedSum += utxo->getAmount();
                    pendingUtxos.push_back(utxo);
                } else {
                    utxo->unreserveUtxo();
                }
            }
        } else {
            break;
        }
    }

    //    for (auto &utxo: userUtxos[sendersPK]) {
    //         if (achievedSum < neededAmount) {
    //             if(utxo->reserveUtxo()) {
    //                 achievedSum += utxo->getAmount();
    //                 pendingUtxos.push_back(utxo);
    //             }
    //         } else {
    //             break;
    //         }
    //     }

    if (achievedSum < neededAmount) {
        for(auto &utxo : pendingUtxos) {
            utxo->unreserveUtxo();
        }
        return {};
    }

    return pendingUtxos;
}