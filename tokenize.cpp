#include "tokenize.h"
#include <iostream>
#include <string>
#include <forward_list>

#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

// Initilaize the values for each keyValuePair whenever they're initialized
// (both are null and don't hold anything so that we can update and insert values)

keyValueInputs::keyValueInputs(string &k, int v){       // constructor for key value class to intialize the values
    key = k;
    value = v;
}

keyValueInputs::~keyValueInputs(){}         // not need (nothing dynamically allocated)

// getter for key and value of any keyValueInputs
string keyValueInputs::getKey() const{      // return the value of the key
    return key;
}

int keyValueInputs::getValue() const{       // return the value of the value
    return value;
}

// Constructor/Destructor for 2-Dimensional Array

twoDimensionalArray::twoDimensionalArray(int hashTableCap){             // constructor for the 2 dimnesional array

    hashTableSize = hashTableCap;                                       // set the size of the array
    hashTablePairsCount = 0;                                            // initialize the nun of elements to be 0

    hashTableVector.push_back("");                                      // create the and initilize with an empty value
    hashTableList = new forward_list<keyValueInputs>[hashTableSize];    // create the array with a size of the hashTableSize
}

twoDimensionalArray::~twoDimensionalArray(){
    delete[] hashTableList;                                             // delete the dynamically created vector
}

// Given Hashing Function that takes a word string and gets the 

unsigned twoDimensionalArray::hashingFunction(const string &stringToHash){      // given hashing function that returns a hashing index given a word(string)
    unsigned ans = 0;
    for (auto it = stringToHash.begin(); it != stringToHash.end(); ++it){
        ans = ans * 29 + *it;
    }
    return ans % hashTableSize;                                                 // returns the hashing value that we get from a given word
}

// Rehashing Function that creates a new hashTable if the load factor is >= 0.8

void twoDimensionalArray::rehashHashTable(){        // if the load factor is greater than or equal to 0.8 run this
    hashTableSize *= 2;                             // double the array size for the new hash table list
    forward_list<keyValueInputs> *newHashTableList = new forward_list<keyValueInputs> [hashTableSize];      // create a new array with double the size
    
    for (int index{0}; index < hashTableSize / 2; ++index){                 // iterate through each index of the array
        for (auto keyValuePair : hashTableList[index]){                     // iterate through all key value pairs in each index
            string keyString = keyValuePair.getKey();
            int token = keyValuePair.getValue();                            // get the keys and values for each key value pair we iterate through

            unsigned hashIndex = hashingFunction(keyString);                // get hashindex for the given key
            keyValueInputs newAddedKeyValuePair{keyString, token};          // create new keyvalue pair
            
            newHashTableList[hashIndex].push_front(newAddedKeyValuePair);   // insert key value pair to the new array
        }   
    }
    delete[] hashTableList;             // deallocate to avoid memory leaks
    hashTableList = newHashTableList;
}

// Function checks whether we need to rehash the table or not (true if we do, false if we don't)

bool twoDimensionalArray::checkRehash(){                        // check if rehashing is needed
    double loadFactor = hashTablePairsCount / hashTableSize;    // load factor is the num of elemnts / size of array

    if (loadFactor >= 0.8){                                     // if it's greater than or equal to 0.8 rehash
        // std::cout << "rehashing needed" << std::endl;
        return true;
    } else {                                                    // if load factor less than 0.8 don't rehash
        // std::cout << "rehashing not needed" << std::endl;
        return false;
    }
}

// MEMBER FUNCTIONS FOR TWO DIMENSIONAL ARRAY CLASS

bool twoDimensionalArray::insertWord(string &wordToInsert){     // insert word to the 2dimnesional array
    int hashingIndex = hashingFunction(wordToInsert);           //get hashing index of the word

    for (auto keyValuePair : hashTableList[hashingIndex]){      // for each key value at the hashingIndex check that word doesn't already exist
        string keyValue = keyValuePair.getKey();
        if (keyValue == wordToInsert){                          // if it exists, return false
            return false;
        }
    }
    for (char letter : wordToInsert){                           // iterate through each letter in the word that we want to insert
        if (isalpha(letter)){                                   // if the letter is alphabetical, then continue
            continue;          
        } else {
            return false;                                       // if the letter if not in the alphabet, then return false (failure)
        }
    }

    int index = hashTableVector.size();                     // index is the size of the vector (since it scales as values are added size is +1 of the greatest index)
    
    hashTableVector.push_back(wordToInsert);                // push the word to the back of the vector

    keyValueInputs addNewPair{wordToInsert, index};         // create key value pair for the word and the index (token)
    hashTableList[hashingIndex].push_front(addNewPair);     // push the key value pair to the front of the list at the hashing index

    hashTablePairsCount++;                      // increment the num of pairs
    
    bool checkLoadFactor = checkRehash();       // check if rehashing needed
    if (checkLoadFactor == true){               // if returns true, then rehashing needed
        rehashHashTable();
    }
    
    return true;                                // return true if a word is inserted
}

bool twoDimensionalArray::loadFile(const string &fileName){     // loads a given file by inserting its contents to the 2dimensional array
    ifstream fileToRead(fileName);                              // reads the file indicated by fileName
    string wordToAdd;  
    bool testInsert{false};                        
    while (!fileToRead.eof()){                                  // Reads the file and then using the insert function it inserts the words
        fileToRead >> wordToAdd;             
        if (insertWord(wordToAdd)){                             // if a word is EVER added then update testInsert to true to indicate
            testInsert = true;                                  // that AT LEAST ONE word has been added
        }  
    }
    return testInsert;                                          // if no values are added at all, then testInsert stays false indicating 
}                                                               // that there was NEVER a value inserted from the file we wanted to load (no unique words to add)


int twoDimensionalArray::findTokenFromWord(const string &word){     // given a word find the token
    int hashingIndex = hashingFunction(word);                       // find the hashing index of the word

    for (auto keyValuePair : hashTableList[hashingIndex]){          // for each key value pair at the hashing index, find if the word exists
        string keyValue = keyValuePair.getKey();
        int value = keyValuePair.getValue();
        if (keyValue == word){                                      // if the word exists, the token you want to return is the hashhingIndex
            return value;
        }
    }
    return -1;                                                      // if word doesn't exist, return -1
}


string twoDimensionalArray::retreiveWordFromToken(const string &token){
    int tokenInt = stoi(token);                                     // change the token from a string to an int
    if ((tokenInt <= hashTablePairsCount) && (tokenInt > 0)){       // if the int is within the num of pairs (elems) in the vector
        return hashTableVector[tokenInt];                           // return the word at the index corresponding to the token
    } else {
        return "N/A";                                               // if the token is out of bounds, then N/A is returned
    }
}

bool twoDimensionalArray::printAllKeys(const string &k){        // printing all the keys at a given index
    int kIndex = std::stoi(k);                                  // change the index to an integer
    string outputString{""};                                    // empty string to begin with

    bool isHashTableListEmpty = hashTableList[kIndex].empty();      // check if the array is empty or not
    if ((isHashTableListEmpty == true) || (kIndex < 0) || (kIndex > hashTableSize)){
        return false;                                               // if it's empty return false (no elements)
    } else {                                                        // if not empty
        for (auto keyValuePair : hashTableList[kIndex]){            // for each key value pair at the index argument
            string keyValue = keyValuePair.getKey();                // get the key for each key value pair
            outputString += keyValue;                               // add the key to the outputstring along with a space at the end of it
            outputString += " ";      
        }
        if (!outputString.empty()){
            outputString.pop_back();            // if the outputString is not empty, then pop the back to get rid of the extra space
        }   
        cout << outputString << endl;           // print the outputstring
            return true;
        }
}