#include <iostream>
#include <ctime>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stdlib.h>
#include <set>
#include <omp.h>
#include "globals.hpp"
#include "blockchain.hpp"
#include "users.hpp"
#include "transactionpool.hpp"

int main()
{
    TransactionPool& tp = TransactionPool::getInstance();
    Users& u = Users::getInstance();
    BlockChain& bc = BlockChain::getInstance();
    int threads = 2;
    std::cout << "Creating users..." << std::endl;
    for (int i = 0; i < USER_COUNT; i++)
        u.createNewUser("name"+std::to_string(i), "public_key"+std::to_string(i), rand()%10 + 40);
    std::cout << u.size() << " users created." << std::endl;
    std::cout << "Creating transactions..." << std::endl;
    while (tp.size() < TRANSACTION_COUNT){
        std::string pk1 = u.getPublicKey(rand()%USER_COUNT);
        std::string pk2 = u.getPublicKey(rand()%USER_COUNT);
        tp.addNewTransaction(pk1, rand()%10 + 1, pk2, 0);
    }


    std::cout << tp.size() << " transactions created." << std::endl;
    std::string trackedPK = "public_key0";
    std::cout << "Current balance(" << trackedPK << "): " <<  u.getUser(trackedPK).getBalance() << std::endl;


    while (tp.size() > 0){
        std::string lastBlockHash = bc.getLastBlockHash();
        bool continueMining = true;
        Block minedBlock(lastBlockHash);
        int minedThread = 0;
        std::cout << std::endl;
        std::cout << "--- Mining block ---" << std::endl;
        #pragma omp parallel num_threads(threads)
        {
            Block b(lastBlockHash);
            b.mine(continueMining);
            #pragma omp critical
            {
                if (bc.addBlock(b)){
                    continueMining = false;
                    minedBlock = b;
                    minedThread = omp_get_thread_num();
                }
            }
        }
        std::cout << "Block created" << std::endl;
        std::cout << "Thread:              " << minedThread << std::endl;
        std::cout << "Block hash:          " << minedBlock.getCurrentBlockHash() << std::endl;
        std::cout << "Previous block hash: " << minedBlock.getPreviousBlockHash() << std::endl;
        std::cout << "Merkle root hash:    " << minedBlock.getMerkleRootHash() << std::endl;
        std::cout << "Nonce:               " << minedBlock.getNonce() << std::endl;
        std::cout << "Timestamp:           " << minedBlock.getTimeStamp() << std::endl;
        std::cout << "Pool size:           " << tp.size() << std::endl;
        std::cout << "Current balance(" << trackedPK << "): " <<  u.getUser(trackedPK).getBalance() << std::endl;
        std::cout << "Sent transactions: " << std::endl;
        for (Transaction t : minedBlock.getTransactions())
            if (t.getSenderPublicKey() == trackedPK)
                std::cout << t.getTransactionString() << std::endl;
        std::cout << "Recieved transactions: " << std::endl;
        for (Transaction t : minedBlock.getTransactions())
            if (t.getRecieverPublicKey() == trackedPK)
                std::cout << t.getTransactionString() << std::endl;
        std::cout << "--- Block mined ---" << std::endl;
    }
}