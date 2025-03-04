﻿#include <set>
#include <map>
#include<stack>
#include <queue>
#include <iostream>
#include "NFA.hpp"
using namespace std;


int NFA::stateCounter = 0;
NFA::NFA()
{
	init_state = -1;
}

void NFA::AddEpsilonTransition(int src, int dst)
{
	Ntran[src]['_'].insert(dst);
}

//Merge transitions from another NFA 
void NFA::MergeTransitions(const NFA& other)
{
	for (const auto& state : other.Ntran)
	{
		int stateID = state.first;
		const auto& symbolMap = state.second;

		for (const auto& symbolTransition : symbolMap)
		{
			char symbol = symbolTransition.first;
			const auto& destinationStates = symbolTransition.second;

			//Merge each destination set
			Ntran[stateID][symbol].insert(destinationStates.begin(), destinationStates.end());
		}
	}
	//Merging final states
	fin_states.insert(other.fin_states.begin(), other.fin_states.end());
}

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

NFA NFA::Concatenation(NFA nfa1, NFA nfa2) {

	NFA result = nfa1;

	// Merge transitions directly
	result.MergeTransitions(nfa2);

	// Add ε-transitions from final states of nfa1 to the start state of nfa2
	for (int state : nfa1.fin_states) {
		result.AddEpsilonTransition(state, nfa2.init_state);
	}

	// Update final states to nfa2's final states
	result.fin_states.clear();
	result.fin_states.insert(nfa2.fin_states.begin(), nfa2.fin_states.end());

	return result;
}

//Union of two NFAs
NFA NFA::Union(NFA nfa1, NFA nfa2)
{
	NFA result;
	int newStart = stateCounter++;
	int newFinal = stateCounter++;

	result.init_state = newStart;
	result.fin_states.insert(newFinal);

	result.MergeTransitions(nfa1);
	result.MergeTransitions(nfa2);

	result.AddEpsilonTransition(newStart, nfa1.init_state);
	result.AddEpsilonTransition(newStart, nfa2.init_state);

	for (int state : nfa1.fin_states) {
		result.AddEpsilonTransition(state, newFinal);
	}
	for (int state : nfa2.fin_states) {
		result.AddEpsilonTransition(state, newFinal);
	}

	return result;

}

//Kleene star of NFA
NFA NFA::KleenStar(NFA nfa)
{
	NFA result;;
	int newStart = stateCounter++;
	int newFinal = stateCounter++;

	result.init_state = newStart;
	result.fin_states.insert(newFinal);

	result.MergeTransitions(nfa);
	result.AddEpsilonTransition(newStart, newFinal);
	result.AddEpsilonTransition(newStart, nfa.init_state);

	for (int state : nfa.fin_states) {
		result.AddEpsilonTransition(state, newFinal);
		result.AddEpsilonTransition(state, nfa.init_state);
	}

	return result;
	
}

//print NFA
void NFA::Print()
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

set<int> NFA::GetAllStates()
{
	set<int> states;
	
	for (const auto& entry : Ntran) {
		int state = entry.first;
		states.insert(state);
	}

	
	for (const auto& entry : Ntran) {
		const auto& symbolMap = entry.second;
		for (const auto& symbolEntry : symbolMap) {
			const auto& destStates = symbolEntry.second;
			states.insert(destStates.begin(), destStates.end());
		}
	}


	
	states.insert(init_state);
	states.insert(fin_states.begin(), fin_states.end());

	return states;

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



int NFA::GetMaxState()
{
	if (Ntran.empty()) return -1;

	int maxState = 0;
	for (const auto& transition : Ntran) {
		maxState = std::max(maxState, transition.first);
		for (const auto& symbolMap : transition.second) {
			for (int dest : symbolMap.second) {
				maxState = std::max(maxState, dest);
			}
		}
	}
	return maxState;
}

//converting Regular expression to NFA
 NFA NFA::RegexToNFA(const string& regex)
{
	stack<NFA> nfaStack;
	for (char symbol : regex)
	{
		//if symbol is letter or digit
		if (isalnum(symbol))
		{
			NFA baseNFA;
			int startState = stateCounter++;
			int endState = stateCounter++;

			baseNFA.init_state = startState;
			baseNFA.fin_states.insert(endState);
			baseNFA.Ntran[startState][symbol].insert(endState);
			baseNFA.alphabet.insert(symbol);

			nfaStack.push(baseNFA);
		}
		//dealing with concatination
		else if (symbol == '.')
		{
			NFA nfa2 = nfaStack.top();
			nfaStack.pop();
			NFA nfa1 = nfaStack.top();
			nfaStack.pop();
			NFA concatNFA = NFA::Concatenation(nfa1, nfa2);

			concatNFA.alphabet.insert(nfa1.alphabet.begin(), nfa1.alphabet.end());
			concatNFA.alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

			nfaStack.push(concatNFA);
		}
		//dealing with Union
		else if (symbol == '|')
		{
			NFA nfa2 = nfaStack.top();
			nfaStack.pop();
			NFA nfa1 = nfaStack.top();
			nfaStack.pop();
			NFA unionNFA = NFA::Union(nfa1, nfa2);

			unionNFA.alphabet.insert(nfa1.alphabet.begin(), nfa1.alphabet.end());
			unionNFA.alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

			nfaStack.push(unionNFA);
		}
		//dealing with star
		else if (symbol == '*')
		{
			NFA nfa = nfaStack.top();
			nfaStack.pop();
			NFA starNFA = NFA::KleenStar(nfa);

			starNFA.alphabet.insert(nfa.alphabet.begin(), nfa.alphabet.end());
			nfaStack.push(starNFA);
		}
	}
	return nfaStack.top();
}


