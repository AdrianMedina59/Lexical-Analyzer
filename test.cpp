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
	


	DFA dfa2 = nfa.NFA2DFA();
	dfa2.Print();
	string test = "bb";

	dfa2.isAccepted(test);

	cout << "Is " << test << " accepted in this DFA?" << (dfa2.isAccepted(test) ? " Yes" : " No") << endl;
}


