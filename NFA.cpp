#include <set>
#include <map>
#include<stack>
#include <queue>
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

DFA NFA::NFA2DFA()
{
	map < set<int>, int> stateMapping;  //Mapping NFA states to a unique DFA state
	queue<set<int>> stateQueue; //Queue for unprocessed DFA states
	set<int> dfaFinalStates; //Final states for DFA
	int dfaStateCounter = 0;  //counter to assign state ID to DFA states

	//getting epsilon closure of inital state
	set<int> startState = EpsilonClosure({ init_state });
	stateMapping[startState] = dfaStateCounter++;
	stateQueue.push(startState);
	
	//starting DFA creation with alphabet and inital start state as 0
	DFA dfa(alphabet, 0, {});
	int deadState = -1;

	//processing each DFA State
	while (!stateQueue.empty())
	{
		set<int> currentNFAStates = stateQueue.front();
		stateQueue.pop();
		int CurrentDFAState = stateMapping[currentNFAStates];

		//checking if current DFA state is a final state
		for (int nfaState : currentNFAStates)
		{
			if (fin_states.find(nfaState) != fin_states.end())
			{
				dfaFinalStates.insert(CurrentDFAState);
				break;
			}
		}
		
		//processing transitions for each symbol in the alphabet
		for (char symbol : alphabet)
		{
			set<int> nextNFAStates;

			//for each NFA state in the current DFA state
			for (int nfaState : currentNFAStates)
			{
				if (Ntran[nfaState].find(symbol) != Ntran[nfaState].end())
				{
					nextNFAStates.insert(Ntran[nfaState][symbol].begin(), Ntran[nfaState][symbol].end());
				}
			}

			//getting epsilon closure of rest of states
			nextNFAStates = EpsilonClosure(nextNFAStates);



			//if no states can be reached skip
			if (nextNFAStates.empty()) {
				if (deadState == -1)
				{
					deadState = dfaStateCounter++;
					//adding self loops to dead state
					for (char deadSymbol : alphabet)
					{
						dfa.AddTransition(deadState, deadState, deadSymbol);
					}
				}
				dfa.AddTransition(CurrentDFAState, deadState, symbol);
				continue;
			}

			//check if the set of NFA states are already a DFA state
			if (stateMapping.find(nextNFAStates) == stateMapping.end())
			{
				stateMapping[nextNFAStates] = dfaStateCounter++;
				stateQueue.push(nextNFAStates);
			}

			//Add the transition to DFA
			dfa.AddTransition(CurrentDFAState, stateMapping[nextNFAStates], symbol);
		}

	}
	//setting final states
	dfa.setFinalStates(dfaFinalStates);
	return dfa;
}


