#ifndef USERS_H
#define USERS_H

#include <map>
#include "user.hpp"

class Users{
    private:
        std::map<std::string, User> users;

        Users(){
            
        }
    public:
 		static Users& getInstance(){
 			static Users instance;
 			return instance;
 		}       
        void createNewUser(std::string n, std::string pk, double b){
            users.emplace(pk, User(n, pk, b));
        }
        
        bool changeBalance(std::string pk, double c){
            return users.at(pk).changeBalance(c);
        }
        
        User getUser(std::string pk){
            return users.at(pk);
        }
        
        std::string getPublicKey(size_t index){
            auto iter = users.begin();
            std::advance(iter, index);
            return iter->first;
        }

        size_t size(){
        	return users.size();
        }
};

#endif