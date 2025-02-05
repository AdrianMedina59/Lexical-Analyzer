#pragma once
#include <map>
#include <set>
#include <string>
using namespace std;

class DFA {
public:
	DFA(set<char> A, int I, set<int> F) : alphabet(A), init_state(I), fin_states(F) {}
	void AddTransition(int src, set<int> dst, char sym) { Dtran[src][sym] = dst; }
	bool isAccepted(string& input); //checks if the input string is accepted by the NFA
	void Print();

private:
	map< int, map<char, set<int>> > Dtran;
	set<char> alphabet;						//set of input symbols in the alphabet
	int init_state;							//initial state of the DFA
	set<int> fin_states;					//final states of the DFA
};