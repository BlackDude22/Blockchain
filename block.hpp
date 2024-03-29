#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <ctime>
#include <sstream>
#include <algorithm>
#include "tylek8137.hpp"
#include "transactionpool.hpp"
#include "users.hpp"
#include "globals.hpp"

class Block{
    private:
        std::string currentBlockHash;
        std::string previousBlockHash;
        char* timeStamp;
        std::string version;
        std::string merkleRootHash;
        unsigned int nonce;
        std::string difficultyTarget;
        std::vector<Transaction> transactions;
    public:
        Block(std::string pbh){
            previousBlockHash = pbh;
            version = "1.0";
        }

        Block(){}
        
        void mine(bool &continueMining){
            TransactionPool& tp = TransactionPool::getInstance();
            insertValidTransactions();
            while(true){
                merkleRootHash = calculateMerkleRoot(transactions);
                for(nonce = 0; nonce < MAX_NONCE; nonce++){
                    std::stringstream hashStream;
                    hashStream << previousBlockHash << merkleRootHash << nonce;
                    currentBlockHash = tylek8137::getHash(hashStream.str());
                    if (!tylek8137::compareHash(currentBlockHash, DIFFICULTY_TARGET)){
                        difficultyTarget = DIFFICULTY_TARGET;
                        return;
                    }
                    if (!continueMining)
                        return;
                }
                std::next_permutation(transactions.begin(), transactions.end());
            }
        }

        std::string getCurrentBlockHash(){
            return currentBlockHash;
        }

        std::string getPreviousBlockHash(){
            return previousBlockHash;
        }

        std::string getMerkleRootHash(){
            return merkleRootHash;
        }

        char* getTimeStamp(){
            return timeStamp;
        }

        std::string getVersion(){
            return version;
        }

        unsigned int getNonce(){
            return nonce;
        }

        std::string getDifficultyTarget(){
            return difficultyTarget;
        }

        std::vector<Transaction> getTransactions(){
            return transactions;
        }

        void setTimeStamp(){
            std::time_t ticks = std::time(nullptr);
            timeStamp = std::asctime(std::localtime(&ticks)); 
        }

        void operator=(Block right){
            currentBlockHash = right.currentBlockHash;
            previousBlockHash = right.previousBlockHash;
            timeStamp = right.timeStamp;
            version = right.version;
            merkleRootHash = right.merkleRootHash;
            nonce = right.nonce;
            difficultyTarget = right.difficultyTarget;
            transactions = right.transactions;
        }

    private:
        void insertValidTransactions(){
            TransactionPool& tp = TransactionPool::getInstance();
            Users& u = Users::getInstance();

            std::map<std::string, User> userMap;
            while(transactions.size() < BLOCK_TRANSACTION_COUNT){
                Transaction t = tp.getRandomTransaction();
                bool skip = false;
                for (Transaction tt : transactions)
                    if (tt == t){
                        skip = true;
                        break;
                    }
                if (skip)
                    continue;
                std::string spk = t.getSenderPublicKey();
                std::string rpk = t.getRecieverPublicKey();
                userMap.emplace(spk, u.getUser(spk));
                userMap.emplace(rpk, u.getUser(rpk));
                if (!userMap.at(spk).changeBalance(-t.getInput()))
                    continue;
                else if (!userMap.at(rpk).changeBalance(-t.getOutput()))
                    userMap.at(spk).changeBalance(t.getInput());
                else 
                    transactions.push_back(t);
            }
        }
};
#endif