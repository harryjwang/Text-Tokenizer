#include <vector>
#include <forward_list>

#include <string>
#include <iostream>
using namespace std;

class keyValueInputs {      // Key Value class that stores a key (string) and value (int)
    public:
        keyValueInputs(string &key, int value);
        ~keyValueInputs();

        string getKey() const;
        int getValue() const;
    private:
        string key;
        int value;

};


class twoDimensionalArray {

    public:
        twoDimensionalArray(int hashTableSize);                 // creates the 2dimensional array
        ~twoDimensionalArray();

        // void createHashTable(int tableSize); -> Don't need this because create instnace of the class for this
        bool insertWord(string &wordToInsert);
        bool loadFile(const string &fileName);
        int findTokenFromWord(const string &word);
        string retreiveWordFromToken(const string &token);
        bool printAllKeys(const string &k);

    private:
        int hashTableSize{};
        int hashTablePairsCount{};

        forward_list<keyValueInputs> *hashTableList;            // creates a forward list for the hashTable
        unsigned hashingFunction(const string &stringToHash);       
        void rehashHashTable();
        bool checkRehash();

        vector<string> hashTableVector;                         // using vectors, we create the hashVector
};