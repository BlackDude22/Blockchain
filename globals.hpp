#ifndef GLOBALS_h
#define GLOBALS_h

#include <string>
#include <deque>

const std::string DIFFICULTY_TARGET = "00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
const unsigned int MAX_NONCE = 4294967295;
const size_t BLOCK_TRANSACTION_COUNT = 100;
const size_t USER_COUNT = 1e3;
const size_t TRANSACTION_COUNT = 1e5;

#endif