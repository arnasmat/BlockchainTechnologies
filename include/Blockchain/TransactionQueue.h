#ifndef TRANSACTIONQUEUE_H
#define TRANSACTIONQUEUE_H

#include <vector>

#include "Transaction.h"
#include "User.h"
#include "UTXO.h"
#include "HeadBlock.h"
#include "HashAlg/HashGenInterface.h"

namespace TransactionQueue {

    //its for clearing mempool from invalid transactions and transactions that would make users overspend 
    void findPossibleMempoolTransaction(std::vector<Transaction *>& mempool) {
        std::vector<Transaction *> newMempool{};
        std::map<std::string, double> userWantsToSend;

        HashGenInterface* hashGen = new MatrixHash();
        for(auto transaction : mempool) {

            User* sender = UserSystem::getInstance().findUserMatchingPublicKey(transaction->getSenderPublicKey());
            if(!sender) {
                continue; // Invalid sender
            }
            // Verify transaction signature (that it was actually created by the specified sender)
            std::string content = transaction->getSenderPublicKey() + std::to_string(transaction->getTransactionTime()) 
                                + std::to_string(transaction->getOutputs()[0].first) + transaction->getOutputs()[0].second;

            if(hashGen->generateHash(content) != transaction->getTransactionId()) {
                continue; // Signatures do not match
            }

            if(!userWantsToSend.count(transaction->getSenderPublicKey())) {
                    userWantsToSend[transaction->getSenderPublicKey()]=transaction->getOutputs()[0].first;
                } else {
                    userWantsToSend[transaction->getSenderPublicKey()]+=transaction->getOutputs()[0].first;
                }
                if(UtxoSystem::getInstance().getBalanceOfPublicKey(transaction->getSenderPublicKey()) >= userWantsToSend[transaction->getSenderPublicKey()]) {
                    newMempool.push_back(transaction);
                }
        } 
        delete hashGen;           
        mempool = std::move(newMempool);
    }

    //its for getting transactions that dont cause double spending
    std::vector<Transaction *> pickValidTransactions(std::vector<Transaction *>& mempool, int number) {
        
        std::vector<Transaction *> validMempool{};
        
        for(auto transaction : mempool) {            
            if(validMempool.size() >= number) {
                break;
            }    
            
            // Check if sender has enough UTXOs
            std::vector<Utxo *> chosenUtxos = UtxoSystem::getInstance().findUtxosThatSatisfySum(
                transaction->getSenderPublicKey(), 
                transaction->getOutputs()[0].first
            );
            
            if(chosenUtxos.size()) { 
                transaction->fillTransaction(chosenUtxos);
                validMempool.push_back(transaction);
            }
        }
        
        return validMempool;
    }

    //its for getting rid of most recent transactions that have been already mined into block
    void freeMempoolFromMinedTransaction(std::vector<Transaction*>& mempool) {
        std::vector<Transaction *> newMempool{};

        std::vector<Transaction*> recentlyUsedTransactions = HeadBlock::getInstance().getHeadBlock()->getTransactions();
        for(auto &transaction : mempool){
            if(std::find(recentlyUsedTransactions.begin(), recentlyUsedTransactions.end(), transaction) == recentlyUsedTransactions.end()){
                newMempool.push_back(transaction);
            }
        }
        mempool = std::move(newMempool);   
    }

}    

#endif