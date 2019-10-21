#ifndef TRANSACTION_POOL_H
#define TRANSACTION_POOL_H

#include <map>
#include <stdlib.h>
#include "transaction.hpp"

class TransactionPool{
    private:
        std::map<std::string, Transaction> transactionPool;
        TransactionPool(){
            
        }
    public:
        static TransactionPool& getInstance(){
        	static TransactionPool instance;
        	return instance;
        }

        void addNewTransaction(std::string spk, double in, std::string rpk, double out){
            Transaction t(spk, in, rpk, out);
            transactionPool.emplace(t.getHash(), t);
        }
        
        size_t size(){
            return transactionPool.size();
        }

        Transaction getRandomTransaction(){
            size_t randomIndex = rand()%size();
            auto iter = transactionPool.begin();
            std::advance(iter, randomIndex);
            return iter->second;
        }

        void removeTransaction(std::string transactionKey){
            transactionPool.erase(transactionKey);
        }

        bool contains(std::string transactionKey){
            if (transactionPool.count(transactionKey) == 0)
                return false;
            return true;
        }
};

std::string calculateMerkleRoot(std::vector<Transaction> transactions){
    std::vector<std::string> merkleTree;
    for (Transaction t : transactions)
        merkleTree.push_back(t.getHash());
        
    while (merkleTree.size() > 1){
        if (merkleTree.size()%2 != 0)
            merkleTree.push_back("");
        std::vector<std::string> newMerkleTree;
        for (int i = 0; i < merkleTree.size(); i += 2){
            std::string first = merkleTree.at(i);
            std::string second = merkleTree.at(i+1);
            newMerkleTree.push_back(tylek8137::getHash(first + second));
        }
        merkleTree = newMerkleTree;
    }
    return merkleTree.at(0);
}

#endif