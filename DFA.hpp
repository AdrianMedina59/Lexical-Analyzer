#pragma once
#include <map>
#include <set>
#include <string>
#include <iostream>
using namespace std;

class DFA {
public:
	DFA(set<char> A, int I, set<int> F) : alphabet(A), init_state(I), fin_states(F) {}
	DFA();
	void AddTransition(int src, int dst, char sym);
	bool isAccepted(string& input); //checks if the input string is accepted by the NFA
	bool acceptsEpsilon();   //check if a DFA accepts epsilon
	bool isValidDFA();
	void setFinalStates(set<int> finalStates);
	void Print();

private:
	map< int, map<char, int >> Dtran;  //Dtran maps a state (int) to a map of symbol (char) to a set of possible next states set<int>
	set<char> alphabet;						//set of input symbols in the alphabet
	int init_state;							//initial state of the DFA
	set<int> fin_states;					//final states of the DFA
};