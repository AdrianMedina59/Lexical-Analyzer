#include "test.hpp"

void test::NFA_Test()
{

	set<char> alphabet = { 'a','b' };
	int inital_state = 0;
	set<int> final_states = { 2,4 };

	NFA nfa(alphabet, inital_state, final_states);
	nfa.AddTransition(0, { 1,3 }, '_'); // the '_' is epsilon
	nfa.AddTransition(1, { 2 }, 'a');
	nfa.AddTransition(2, { 2 }, 'a');
	nfa.AddTransition(3, { 4 }, 'b');
	nfa.AddTransition(4, { 4 }, 'b');
	nfa.Print();
	set<int> ecset = nfa.EpsilonClosure({ 0 });
	for (const auto& element : ecset) cout << element << " "; cout << endl;

	string test = "ab";
	cout << "Is " << test << " accepted in this NFA?" << (nfa.isAccepted(test) ? " Yes" : " No") << endl;
	
}

void test::DFA_Test()
{
	set<char> alphabet = { 'a','b' };
	int inital_state = 0;
	set<int> final_states = { 3 };

	DFA dfa(alphabet, inital_state, final_states);
	dfa.AddTransition(0, 1 , 'a');
	dfa.AddTransition(0, 0 , 'b');

	dfa.AddTransition(1, 2 , 'a');
	dfa.AddTransition(1,  1 , 'b');
	

	dfa.AddTransition(2,  2 , 'b');
	dfa.AddTransition(2,  3 , 'a');

	dfa.AddTransition(3,  3 , 'b');
	dfa.AddTransition(3,  2 , 'a');

	dfa.isValidDFA({ 0 });
}
