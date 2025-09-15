// include libraries here (STL not allowed)
#include "tokenize.h"
#include <iostream>
#include <string>
#include <forward_list>

#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;


int main(){

    // ONLY USED FOR CREATE FUNCTION -> OTHERS USE GETLINE METHOD
    string commandString{};                                         // input for the create command 
    int commandValueInt{};                                          // takes the input for the size of the hashTable

    // CREATE FUNCTION
    cin >> commandString;
    cin >> commandValueInt;
    twoDimensionalArray TwoDArray{commandValueInt};                 // create 2 directional array
    cout << "success" << endl;
    // END CREATE FUNCTION

    while (true){        
        string inputString{};                                       // input string with the command and arguments
        getline(cin , inputString);

        int spaceIndex = inputString.find(" ");                     // find space to separate out arguments and command
        string command = inputString.substr(0, spaceIndex);
        string argumentString = inputString.substr(spaceIndex + 1, inputString.length() - spaceIndex - 1); // argument is the spots after the space until end of the word

        if (command == "insert"){
            bool insertResult{};
            insertResult = TwoDArray.insertWord(argumentString);    // check whether the command (function) was executed correctly

            if (insertResult == true){                              // if true print success, else print failure
                cout << "success" << endl;  
            } else {
                assert(insertResult == false);
                cout << "failure" << endl;
            }

        } else if (command == "load"){              
            bool loadResult{};  
            loadResult = TwoDArray.loadFile(argumentString);        // check if loaded correctly

            if (loadResult == true){                                // if ture then print sucess else if it's false print failure
                cout << "success" << endl;
            } else {
                // assert(loadResult == false);
                cout << "failure" << endl;
            }

        } else if (command == "tok"){   
            int tokenResult{};
            tokenResult = TwoDArray.findTokenFromWord(argumentString);              // takes the value of the token we get from the word

            cout << tokenResult << endl;                            // print the token we got from the word

        } else if (command == "ret"){ 
            string retreiveWordResult{};
            retreiveWordResult = TwoDArray.retreiveWordFromToken(argumentString);   // takes the value of the string that corresponds with the token

            cout << retreiveWordResult << endl;                     // prints the word that got returned
        } else if (command == "tok_all"){
            istringstream wordString(argumentString);               // using istringstream, split string into multiple words

            string outputString{""};                                // start with emtpy outputstring
            string individualWord;                                  // initilize the individual words

            while (!wordString.eof()){
                wordString >> individualWord;                                   // go to the next word
                int tokenToAdd = TwoDArray.findTokenFromWord(individualWord);   // get the token for the specific word
                outputString += to_string(tokenToAdd);              // change the integer to a string and add it to the output string
                outputString += " ";
            }
            
            if (!outputString.empty()){
                outputString.pop_back();                            // if the outputstring isn't empty, pop the back to get rid of the extra space
            }
            
            cout << outputString << endl;                           // print the output string

        } else if (command == "ret_all"){
            istringstream tokenString(argumentString);              // repeat the same steps as the tok_all for ret_all

            string outputString{};
            string individualToken{};

            while (!tokenString.eof()){                                                     // runs until string of tokens gets to the end
                tokenString >> individualToken;                                             // iterate through each of the tokens
                string wordToAdd = TwoDArray.retreiveWordFromToken(individualToken);        // get the words to add from the function to retreive a word from a give token
                outputString += wordToAdd;                          // and the string of the wor to the outputString
                outputString += " ";                                // space in between each word
            }       

            if (!outputString.empty()){
                outputString.pop_back();                            // if output string isn't empty get rid of the extra space
            }

            cout << outputString << endl;                           // print the output string

        } else if (command == "print"){ 
            bool printResult{};                     
            printResult = TwoDArray.printAllKeys(argumentString);   // takes the result of the print(if it prints or not)
            
            if (printResult == true){                               // either way just continue, the stuff print is in the member funciton itself
                continue;
            } else {
                continue;   
            }  
            
        } else if (command == "exit"){                              // exits the code when called
            break;
        }
    }

}