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

    std::cout << "Creating users..." << std::endl;
    for (int i = 0; i < USER_COUNT; i++)
        u.createNewUser("name"+std::to_string(i), "public_key"+std::to_string(i), rand()%10 + 40);
    std::cout << "Users created." << std::endl;
    std::cout << "Creating transactions..." << std::endl;
    for (int i = 0; i < TRANSACTION_COUNT; i++){
        std::string pk1 = u.getPublicKey(rand()%USER_COUNT);
        std::string pk2 = u.getPublicKey(rand()%USER_COUNT);
        tp.addNewTransaction(pk1, rand()%10 + 1, pk2, 0);
    }
    std::cout << "Transaction created." << std::endl;
    std::string trackedPK = "public_key0";
    std::cout << "Current balance(" << trackedPK << "): " <<  u.getUser(trackedPK).getBalance() << std::endl;
    for (int i = 0; i < 100; i++){
        std::cout << std::endl;
        std::cout << "--- Mining block ---" << std::endl;
        Block b(bc.getLastBlockHash());
        b.mine();
        if (bc.addBlock(b)){
            std::cout << "Block created" << std::endl;
            std::cout << "Block hash:          " << b.getCurrentBlockHash() << std::endl;
            std::cout << "Previous block hash: " << b.getPreviousBlockHash() << std::endl;
            std::cout << "Merkle root hash:    " << b.getMerkleRootHash() << std::endl;
            std::cout << "Nonce:               " << b.getNonce() << std::endl;
            std::cout << "Timestamp:           " << b.getTimeStamp() << std::endl;
            std::cout << "Current balance(" << trackedPK << "): " <<  u.getUser(trackedPK).getBalance() << std::endl;
            std::cout << "Sent transactions: " << std::endl;
            for (Transaction t : b.getTransactions())
                if (t.getSenderPublicKey() == trackedPK)
                    std::cout << t.getTransactionString() << std::endl;
            std::cout << "Recieved transactions: " << std::endl;
            for (Transaction t : b.getTransactions())
                if (t.getRecieverPublicKey() == trackedPK)
                    std::cout << t.getTransactionString() << std::endl;
        }
        else 
            std::cout << "Block rejected" << std::endl;
        std::cout << "--- Block mined ---" << std::endl;
    }  
}