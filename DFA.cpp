#include "DFA.hpp"
#pragma once

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
	}
}

void DFA::Print()
{
}
