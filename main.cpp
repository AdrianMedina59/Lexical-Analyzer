#include "lexer.hpp"
#include "shunting.hpp"
#include "test.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

/*
* AUTHOR : Adrian Medina ID:1224575755
* 
This is the main class file. It's main functionality is to parse the file contents
and creates the list of tokens and list of DFA's from the token's regex. Then using a 
lexer object to deal with logic.This class file then prints out the token followed by 
the portion of the input string that accepts it.
*/


// Function to check if a string starts with 't' followed by a digit
bool isTokenName(const string& str) {
    //checking the the first char of the string is 't' and length is greater than 1 and is a digit at index 1
    return !str.empty() && str[0] == 't' && str.length() > 1 && isdigit(str[1]);
}

void loadInputFile(istream& input)
{
    

    string line, tokenLine, testString;
    vector<string> tokenNames, regex_List;
    vector <Token> tokens;
    bool acceptsEpsilon = false;

    // Read the first line containing tokens
    if (getline(input, tokenLine)) {
       
    }

    //Reading the second line containing the test string
    if (getline(input, line))
    {
        size_t start = line.find('"');
        size_t end = line.rfind('"');
        if (start != string::npos && end != string::npos && start != end)
        {
            testString = line.substr(start + 1, end - start - 1);
           
            
        }
    }
    stringstream ss(tokenLine);
    string tokenData;
    bool stopParsing = false;

    while (getline(ss, tokenData, ',')) {
        stringstream tokenStream(tokenData);
        string tokenName, regexPart, regex;
        tokenStream >> tokenName;

        if (isTokenName(tokenName)) {
            while (tokenStream >> regexPart) {
                if (regexPart == "#") {  // Stop at '#'
                    stopParsing = true;
                    break;
                }
                if (!regex.empty()) regex += " ";
                regex += regexPart;
            }
            //adding to tokenNames string vector
            tokenNames.push_back(tokenName);

            regex_List.push_back(regex);
        }
    }

    //Coverting string vector to token vector
    for (const string& name : tokenNames)
    {
        Token token;
        token.name = name;
        tokens.push_back(token);
    }
    
    

    vector<DFA> DFA_List;
    //converting regex list to DFA list
    for (const string& regex : regex_List)
    {
       
        NFA nfa;
        DFA dfa;
        shunting shunting;
        string postfix = shunting.InfixToPostfix(regex);
        nfa = NFA::RegexToNFA(postfix);
        dfa = nfa.NFA2DFA();
        DFA_List.push_back(dfa);
        
    }

    

    lexer Lexer(testString);

    //setting the lexer token list and DFA list
    Lexer.SetTokens(tokens);
    Lexer.SetDFAList(DFA_List);

    //debugging
    /*Lexer.printTokens();
    Lexer.PrintDFAList();
    Lexer.PrintInputString();*/
    
    

    Token token;
    vector<Token> epsilonTokens;
    vector<Token> lexedTokens;
    for (int i = 0; i < DFA_List.size(); i++)
    {
        if (DFA_List[i].acceptsEpsilon())
        {
            epsilonTokens.push_back(tokens[i]);
            acceptsEpsilon = true;

            
        }
    }
    
    if (acceptsEpsilon == true && !epsilonTokens.empty())
    {
        cout << "EPSILON IS NOT A TOKEN ";
        for (const auto& token : epsilonTokens)
        {
            cout << token.name << " ";
        
        }
        cout << endl;
    }
    

    while ((token = Lexer.getToken()).name != "EOS" && acceptsEpsilon == false)
    {
        if (token.name == "Error")
        {
            cout << "ERROR" << endl;
            return;
        }
        else {
            cout << token.name << " , " << '"' << token.value << '"' << endl;
            lexedTokens.push_back(token);
        }
    }
   
   
}

int main() {
 
    loadInputFile(cin);
}
