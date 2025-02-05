#include <set>
#include <map>
#include<stack>
#include <iostream>
#include "NFA.hpp"
using namespace std;



//epsilon closure of the set of states in the NFA
//a DFS traversal of epsilon transition
set<int> NFA::EpsilonClosure(const set<int>& states)
{
	//depth first search stack
	stack<int> dfs_stack;

	set<int> epsilon_closure_set = states;

	//iterating through all the states
	for (int state : states)
	{
		//pushing the current state to the dfs stack
		dfs_stack.push(state);
		
		//while the dfs stack is not empty
		//set get the state top of the stack and pop it to start trasversal from that state
		while (!dfs_stack.empty())
		{
			int state = dfs_stack.top();
			dfs_stack.pop();

			//check all epsilon transitions from this state
			if (Ntran[state].find('_') != Ntran[state].end())
			{
				for (int next_state : Ntran[state]['_'])
				{
					if (epsilon_closure_set.find(next_state) == epsilon_closure_set.end())
					{
						epsilon_closure_set.insert(next_state);
						dfs_stack.push(next_state);
					}
				}
			}
		}
	}
	return epsilon_closure_set;
}

bool NFA::isAccepted(string& input)
{
	//getting the epsilon closure of the start state if there is '_' transitions
	set<int> current_states = EpsilonClosure({ init_state });

	//iterating through the input string each symbol
	for (char symbol : input)
	{
		set<int> next_states;
	
		for (int state : current_states)
		{
			if (Ntran[state].find(symbol) != Ntran[state].end())
			{
				for (int next_state : Ntran[state][symbol])
				{
					next_states.insert(next_state);
				}
			}
		}

		//computing the epsilon closure of the next states
		current_states = EpsilonClosure(next_states);
	}
	//check if any state in the current set is the final state
	for (int state : current_states)
	{
		if (fin_states.find(state) != fin_states.end())
		{
			return true;
		}
	}
	return false;
}

//print NFA
void NFA::Print()
{
	cout << "NFA Transitions: \n";
	for (const auto& nfa_row : Ntran)
	{
		cout << nfa_row.first << ":\t";

		for (const auto& transition : nfa_row.second)
		{
			cout << transition.first << ": { ";
			for (int state : transition.second)
				cout << state << " ";
			cout << "} ";
		}
		cout << endl;
	}
}


