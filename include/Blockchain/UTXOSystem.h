#ifndef UTOXSYS_H
#define UTOXSYS_H

#include "UTXO.h"
#include "SystemAlgorithm.h"

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

    std::vector<Utxo *> getUtxosForUser(const std::string &userPK) const {
        auto it = userUtxos.find(userPK);
        if (it != userUtxos.end()) {
            return it->second;
        }
        return {};
    }

    std::vector<Utxo *> findUtxosThatSatisfySum(const std::string &sendersPK, double neededAmount) {
        if (userUtxos.find(sendersPK) == userUtxos.end()) {
            return {}; //treated as false
        }

        std::sort(userUtxos[sendersPK].begin(), userUtxos[sendersPK].end(), [](Utxo *x, Utxo *y) {
            return x->getAmount() > y->getAmount();
        });

        std::vector<Utxo *> pendingUtxos{};
        double achievedSum = 0;

        size_t index = 0;
        for (const auto &utxo: userUtxos[sendersPK]) {
            if (achievedSum < neededAmount) {
                achievedSum += utxo->getAmount();
                pendingUtxos.push_back(utxo);
                index++;
            } else {
                break;
            }
        }

        if (achievedSum < neededAmount) {
            return {}; //treated as false
        }

        userUtxos[sendersPK].erase(userUtxos[sendersPK].begin(), userUtxos[sendersPK].begin() + index);
        if (userUtxos[sendersPK].size() == 0) {
            userUtxos.erase(sendersPK);
        }
        return pendingUtxos;
    }

    void addNewUtxos(const std::vector<std::pair<double, std::string> > &outputs, Transaction *transaction) {
        for (unsigned int i = 0; i < outputs.size(); i++) {
            userUtxos[outputs[i].second].push_back(new Utxo(transaction, i, outputs[i].first, outputs[i].second));
        }
    }

    void deleteUtxo(const std::string &senderPk, Utxo *utxoToBeDeleted) {
        if (userUtxos.find(senderPk) != userUtxos.end()) {
            auto iter = std::find(userUtxos[senderPk].begin(), userUtxos[senderPk].end(), utxoToBeDeleted);
            if (iter != userUtxos[senderPk].end()) {
                userUtxos[senderPk].erase(iter);
                delete utxoToBeDeleted;
                if (userUtxos[senderPk].empty()) {
                    userUtxos.erase(senderPk);
                }
            }
        }
    }

    double getBalanceOfPublicKey(const std::string &userPK) {
        double balance = 0;
        for (auto &utxo: userUtxos[userPK]) {
            balance += utxo->getAmount();
        }
        return balance;
    }
};

#endif
