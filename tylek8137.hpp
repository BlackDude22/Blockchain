#ifndef TYLEK8137_H
#define TYLEK8137_H

#include <sstream>
#include <cmath>
#include <algorithm>
#include <iomanip>

class TYLEK8137{
	private:
		const static size_t blockLength = 8;
		const static size_t blockCount = 8;
	public:
		TYLEK8137(){
		}

		std::string getHash(std::string input){
			unsigned long int* hashArray = new unsigned long int[blockCount] ();
			for (int i = 0; i < input.length(); i += blockCount){
				std::string substring = input.substr(i, blockCount);
				unsigned long int* tempArray = stringToHash(substring);
				generateHash(tempArray, 2);
				addHash(hashArray, tempArray);
				generateHash(hashArray, 1);
				free(tempArray);
			}
			generateHash(hashArray, 2);
			std::string result = hexify(hashArray);
			free(hashArray);
			return result;
		}
		
	private:
		unsigned long int* stringToHash(std::string input){
			unsigned long int* hashArray = new unsigned long int[blockCount] ();
			for (int i = 0; i < blockCount && i < input.length(); ++i)
				hashArray[i] = (unsigned long int)input.at(i);
			return hashArray;
		}

		void generateHash(unsigned long int* hashArray, int iterations){
			for (int i = 0; i < iterations; i++){
				unsigned long int previousArraySquared[blockCount] = {};
				for(int i = 0; i < blockCount; i++)
					previousArraySquared[i] = getSquareRootNumbers(hashArray[i]);
				mix(hashArray);
				for(int i = 0; i < blockCount; i++)
					hashArray[i] = hashArray[i]^previousArraySquared[i];
			}
		}

		unsigned long int getSquareRootNumbers(unsigned long int input){
			double root = sqrt(input);
			double whole, frac;
			frac = std::modf(root, &whole);
			return ((unsigned long int)(frac*0x100000000));
		}

		void mix(unsigned long int* hashArray){
			unsigned long int previousArray[blockCount*3] = {};
			for (int i = 0; i < blockCount; i++){
				previousArray[i] = hashArray[i];
				hashArray[i] = 0x0;
			}

			for (int i = 0; i < blockCount; i++)
				for (int j = 0; j < blockCount; j++){
					hashArray[i] *= 0x10;
					hashArray[i] += previousArray[j]%0x10;
					previousArray[j] /= 0x10;
				}
		}

		std::string hexify(unsigned long int* hashArray){
			std::stringstream ss;
			for(int i = 0; i < blockCount; i++)
				ss << std::setfill('0') << std::setw(8) << std::hex << hashArray[i];
			return ss.str();
		}

		void addHash(unsigned long int* hash1, unsigned long int* hash2){
			for (int i = 0; i < blockCount; i++)
				hash1[i] = hash1[i]^hash2[i];
		}
};

namespace tylek8137{
	TYLEK8137 hasher;

	std::string getHash(std::string input){
		return hasher.getHash(input);
	}

	bool compareHash(std::string left, std::string right){
    for (int i = 0; i < left.length(); i += 8){
        std::stringstream leftStream;
        unsigned long int leftInt;
        leftStream << std::hex << left.substr(i, 8);
        leftStream >> leftInt;
        std::stringstream rightStream;
        unsigned long int rightInt;
        rightStream << std::hex << right.substr(i, 8);
        rightStream >> rightInt;
        if (leftInt > rightInt)
            return true;
    }
    return false;
}
}

#endif