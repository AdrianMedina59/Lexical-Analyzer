#pragma once
#include <string>
#include <vector>
#include "DFA.hpp"
#include "NFA.hpp"
using namespace std;

struct Token
{
	string name;
};

class lexer {
public:
	lexer(const string& input);
	Token getToken();
	void addToken(string& tokenName);
	void addDFA(string& regex);

private:
	string input;
	vector<Token> tokens;
	vector<DFA> DFA_List;
};