#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <ctime>
#include <sstream>
#include "tylek8137.hpp"

class Transaction{
    private:
        std::string senderPublicKey;
        double input;
        std::string recieverPublicKey;
        double output;
        std::string transactionHash;
        std::string transactionString;
        std::time_t ticks;
        char* timeStamp;
    
    public:
        Transaction(std::string spk, double in, std::string rpk, double out){
            input = in;
            senderPublicKey = spk;
            output = out;
            recieverPublicKey = rpk;
            std::stringstream transactionStream;
            std::time_t ticks = std::time(nullptr);
            timeStamp = std::asctime(std::localtime(&ticks));
            transactionStream << "_" << spk << "_" << in << "_" << rpk << "_" << out << "_" << ticks;
            transactionString = transactionStream.str();
            transactionHash = tylek8137::getHash(transactionString);
        }
        
        std::string getHash(){
            return transactionHash;
        }

        std::string getSenderPublicKey(){
            return senderPublicKey;
        }

        std::string getRecieverPublicKey(){
            return recieverPublicKey;
        }

        double getInput(){
            return input;
        }

        double getOutput(){
            return output;
        }

        char* getTimeStamp(){
            return timeStamp;
        }

        std::string getTransactionString(){
            return transactionString;
        }
        
        bool operator>(Transaction right){
            return tylek8137::compareHash(transactionHash, right.transactionHash);
        }
        
        bool operator<(Transaction right){
            return !tylek8137::compareHash(transactionHash, right.transactionHash);
        }
};

#endif