#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <iostream>
#include <list>
#include "transactionpool.hpp"
#include "tylek8137.hpp"
#include "block.hpp"
#include "globals.hpp"

class BlockChain{
private:
	std::list<Block> blockChain;
	std::string genesisString;

	BlockChain(){
		genesisString = "geriausiasblockchainas";
	}
public:
	static BlockChain& getInstance(){
		static BlockChain instance;
		return instance;
	}

	std::string getLastBlockHash(){
		if (blockChain.empty())
			return genesisString;
		else return blockChain.back().getCurrentBlockHash();
	}

	size_t size(){
		return blockChain.size();
	}

	std::list<Block> getBlockChain(){
		return blockChain;
	}

	bool addBlock(Block& block){
		if (block.getPreviousBlockHash() != getLastBlockHash())
			return false;
		if (block.getMerkleRootHash() != calculateMerkleRoot(block.getTransactions()))
			return false;
		if (block.getCurrentBlockHash() != tylek8137::getHash(block.getPreviousBlockHash() + block.getMerkleRootHash() + std::to_string(block.getNonce())))
			return false;
		if (tylek8137::compareHash(block.getCurrentBlockHash(), DIFFICULTY_TARGET))
			return false;
		if (!checkTransactionValidity(block.getTransactions()))
			return false;
		block.setTimeStamp();
		blockChain.push_back(block);
        commitTransactions(block.getTransactions());
        return true;
	}
private:
	bool checkTransactionValidity(std::vector<Transaction> transactions){
        Users& u = Users::getInstance();
		std::map<std::string, User> userMap;
        for(Transaction t : transactions){
        	std::string spk = t.getSenderPublicKey();
            std::string rpk = t.getRecieverPublicKey();
            userMap.emplace(spk, u.getUser(spk));
            userMap.emplace(rpk, u.getUser(rpk));
            if (!userMap.at(spk).changeBalance(-t.getInput()) || !userMap.at(rpk).changeBalance(-t.getOutput()))
            	return false;
        }
        return true;
    }

    void commitTransactions(std::vector<Transaction> transactions){
    	Users& u = Users::getInstance();
    	for (Transaction t : transactions){
    		std::string spk = t.getSenderPublicKey();
            std::string rpk = t.getRecieverPublicKey();
            double in = t.getInput();
            double out = t.getOutput();
            u.changeBalance(t.getSenderPublicKey(), -t.getInput());
            u.changeBalance(t.getSenderPublicKey(), t.getOutput());
            u.changeBalance(t.getRecieverPublicKey(), t.getInput());
            u.changeBalance(t.getRecieverPublicKey(), -t.getOutput());
    	}
    }
};

#endif