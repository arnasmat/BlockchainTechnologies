#ifndef UTOXSYS_H
#define UTOXSYS_H

#include "UTXO.h"
#include "SystemAlgorithm.h"
#include "UserSystem.h"

#include <vector>
#include <algorithm>
#include <map>

class UtxoSystem : SystemAlgorithm {
private:
    std::map<std::string, std::vector<Utxo *> > userUtxos;

    UtxoSystem() {
    }

    UtxoSystem(const UtxoSystem &) = delete;

    UtxoSystem &operator=(const UtxoSystem &) = delete;

public:
    static UtxoSystem &getInstance() {
        static UtxoSystem instance;
        return instance;
    }

    std::vector<Utxo *> findUtxosThatSatisfySum(const std::string &sendersPK, double neededAmount);

    void addNewUtxos(const std::vector<std::pair<double, std::string> > &outputs, Transaction *transaction) {
        for (unsigned int i = 0; i < outputs.size(); i++) {
            userUtxos[outputs[i].second].push_back(new Utxo(transaction, i, outputs[i].first, outputs[i].second));
        }
    }

    void deleteUtxo(const std::string &senderPk, std::vector<Utxo*> utxosToBeDeleted) {
        for(auto &utxo : utxosToBeDeleted) {
            auto iter = std::find(userUtxos[senderPk].begin(), userUtxos[senderPk].end(), utxo);
            if (iter != userUtxos[senderPk].end()) {
                userUtxos[senderPk].erase(iter);
                delete utxo;
            } else {
                std::cerr << "Warning: Attempted to delete non-existent UTXO" << std::endl;
            }
        }
        if (userUtxos[senderPk].empty()) {
            userUtxos.erase(senderPk);
        }
    }

    double getBalanceOfPublicKey(const std::string &userPK) {
        double balance = 0;
        for (auto &utxo: userUtxos[userPK]) {
            balance += utxo->getAmount();
        }
        return balance;
    }

    int getNumberOfUtxos() {
        int numberOfUtxos = 0;
        for(auto &mapUtxo : userUtxos) {
            numberOfUtxos += mapUtxo.second.size();
        }
        return numberOfUtxos;
    }

};

#endif
