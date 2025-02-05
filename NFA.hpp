#pragma once
#include <set>
#include <map>
#include<stack>
#include <iostream>
using namespace std;
class NFA
{
public:
	NFA(set<char> A, int I, set<int> F) : alphabet(A), init_state(I), fin_states(F) {}

	//adding transtions by setting the edge from source node and symbol to the desired destination node
	void AddTransition(int src, set<int> dst, char sym) { Ntran[src][sym] = dst; }
	set<int> EpsilonClosure(const set<int>& states);
	bool isAccepted(string& input); //checks if the input string is accepted by the NFA
	void Print();
	

private:
	map< int, map<char, set<int>> > Ntran;
	set<char> alphabet;						//set of input symbols in the alphabet
	int init_state;							//initial state of the NFA
	set<int> fin_states;					//final states of the NFA




};