#include "DFA.hpp"
#pragma once

void DFA::AddTransition(int src, int dst, char sym)
{
	if (Dtran[src].find(sym) != Dtran[src].end())
	{
		cout << "DFA has multiple transitions of the same symbol: '" << sym  << "' from state " << src << endl;

		return;
	}   
	 Dtran[src][sym] = dst; 
}

bool DFA::isAccepted(string& input)
{
	
	//also need to check if there is no duplicate transitions for each state because DFA's only accept 1 transition per symbol of the alphabet
	
	
	return false;
}

bool DFA::isValidDFA(const set<int>& states)
{
	//need to check all the transitions at each state to check if each state has a transition for each symbol of the alphabet
	for (const auto& stateTransitions : Dtran)
	{
		//getting the state from the first Dtran parameter (int)
		int state = stateTransitions.first;

		//now need to check that there is only 1 transition per symbol
		set<char> seenSymbols;

		for (const auto& transition : stateTransitions.second)
		{
			char symbol = transition.first;
			int nextState = transition.second;

			//checking if the symbol is already read from current state
			if (seenSymbols.find(symbol) != seenSymbols.end())
			{
				cout << "Invalid DFA, there is multiple transitions from state " << state << "with symbol: " << symbol << endl;
				return false;
			}
			seenSymbols.insert(symbol);
		}
		if (seenSymbols.size() != alphabet.size())
		{
			cout << "There are missing transitions of the current DFA at state: " << state;
			return false;
		}
	}
	cout << "this is a valid DFA!";
	return true;
}

void DFA::Print()
{
}
