#include <iostream>
#include <ctime>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stdlib.h>
#include "globals.hpp"
#include "blockchain.hpp"
#include "users.hpp"
#include "transactionpool.hpp"

int main()
{
    TransactionPool& tp = TransactionPool::getInstance();
    Users& u = Users::getInstance();
    BlockChain& bc = BlockChain::getInstance();

    for (int i = 0; i < USER_COUNT; i++)
        u.createNewUser("name"+std::to_string(i), "public_key"+std::to_string(i), rand()%10 + 40);

    for (int i = 0; i < TRANSACTION_COUNT; i++){
        std::string pk1 = u.getPublicKey(rand()%USER_COUNT);
        std::string pk2 = u.getPublicKey(rand()%USER_COUNT);
        tp.addNewTransaction(pk1, rand()%10, pk2, 0);
    }
    
    std::cout << "Current balance: " <<  u.getUser("public_key0").getBalance() << std::endl;
    for (int i = 0; i < 100; i++){
        Block b(bc.getLastBlockHash());
        b.mine();
        if (bc.addBlock(b)){
            std::cout << "Block created" << std::endl;
            std::cout << "Block hash:          " << b.getCurrentBlockHash() << std::endl;
            std::cout << "Previous block hash: " << b.getPreviousBlockHash() << std::endl;
            std::cout << "Merkle root hash:    " << b.getMerkleRootHash() << std::endl;
            std::cout << "Nonce:               " << b.getNonce() << std::endl;
            std::cout << "Timestamp:           " << b.getTimeStamp() << std::endl;
            std::cout << "Transaction count:   " << b.getTransactions().size() << std::endl;
        }
        else 
            std::cout << "Block rejected" << std::endl;
        std::cout << "Current balance: " <<  u.getUser("public_key0").getBalance() << std::endl;
    }  
}