#ifndef TRANSACTION_POOL_H
#define TRANSACTION_POOL_H

#include <deque>
#include "transaction.hpp"

class TransactionPool{
    private:
        std::deque<Transaction> transactionPool;

        TransactionPool(){
            
        }
    public:
        static TransactionPool& getInstance(){
        	static TransactionPool instance;
        	return instance;
        }

        void addNewTransaction(std::string spk, double in, std::string rpk, double out){
            transactionPool.emplace_back(spk, in, rpk, out);
        }
        
        Transaction getTransaction(){
            Transaction result = transactionPool.at(0);
            transactionPool.pop_front();
            return result;
        }
        
        void returnTransaction(Transaction t){
            transactionPool.push_back(t);
        }
        
        size_t size(){
            return transactionPool.size();
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