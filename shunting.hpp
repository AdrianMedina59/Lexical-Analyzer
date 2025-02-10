#pragma once
#include <iostream>
#include <stack>
#include <string>
using namespace std;

class shunting {
public:
	int Precedence(char op);
	bool IsOperand(char c);
	string InfixToPostfix(const string& infix);
};