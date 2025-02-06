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

	//first checking if the DFA is valid
	if (!isValidDFA())
	{
		cout << "This DFA is not valid!" << endl;
		return false;
	}
	
	int currentState = init_state;
	for (char symbol : input)
	{
		if (Dtran.find(currentState) == Dtran.end() || Dtran[currentState].find(symbol) == Dtran[currentState].end())
		{

			return false;
		}
		currentState = Dtran[currentState][symbol];
		
	}
	if (fin_states.find(currentState) != fin_states.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DFA::isValidDFA()
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
	
	return true;
}

void DFA::Print()
{
	cout << "Alphabet: ";
	for (char sym : alphabet)
	{
		cout << sym << " ";
	}
	cout << endl;

	cout << "Initial State: " << init_state << endl;

	cout << "Final States: ";
	for (int state : fin_states)
	{
		cout << state << " ";
	}
	cout << endl;

	cout << "Transitions:" << endl;
	for (const auto& stateTransitions : Dtran)
	{
		int src = stateTransitions.first;
		for (const auto& transition : stateTransitions.second)
		{
			char sym = transition.first;
			int dst = transition.second;
			cout << src << " --" << sym << "--> " << dst << endl;
		}
	}
}
