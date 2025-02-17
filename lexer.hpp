#pragma once
#include <string>
#include <vector>
#include "DFA.hpp"
#include "NFA.hpp"
using namespace std;

struct Token
{
	string name;
	string value;
	Token() = default;
	Token(const std::string& n, const std::string& v) : name(n), value(v) {}
};

class lexer {
public:
	lexer(const string& input);
	Token getToken();					//returns the token from the input string
	void addToken(string& tokenName);  //method to add token to vector
	void addDFA(string& regex);       //method to add DFA to vector
	void SetTokens(vector<Token> tokens);
	void SetDFAList(vector<DFA> DFA_List);

	//helper functions for debugging
	void printTokens();
	void PrintDFAList();
	void PrintInputString();

private:
	string input;
	int pos;
	vector<Token> tokens;
	vector<DFA> DFA_List;
};