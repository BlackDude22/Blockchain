#ifndef USER_H
#define USER_H

#include <string>

class User{
    private:
        std::string name;
        std::string publicKey;
        double balance;
    public:
        User(std::string n, std::string pk, double b){
            name = n;
            publicKey = pk;
            balance = b;
        }
        
        std::string getName(){
            return name;
        }
        
        std::string getPublicKey(){
            return publicKey;
        }
        
        double getBalance(){
            return balance;
        }
        
        bool changeBalance(double c){
            if (balance + c < 0)
                return false;
            balance += c;
            return true;
        }
};

#endif