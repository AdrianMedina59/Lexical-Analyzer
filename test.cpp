#include "test.hpp"
#include "shunting.hpp"

void test::NFA_Test()
{

	string infix;
	cout << "Enter a regular expression: ";
	getline(cin, infix);
	shunting shunting;
	string postfix = shunting.InfixToPostfix(infix);
	cout << "Postfix notation: " << postfix << endl;

	NFA nfa = NFA::RegexToNFA(postfix);
	cout << "Regex to NFA: " << endl;
	nfa.Print();

	cout << "NFA to DFA: " << endl;
	DFA dfa = nfa.NFA2DFA();
	dfa.Print();

	string stringTest;
	cout << "Enter string to test: ";
	getline(cin, stringTest);
	nfa.isAccepted(stringTest);

	cout << "Is " << stringTest << " accepted in this DFA?" << (dfa.isAccepted(stringTest) ? " Yes" : " No") << endl;
	cout << "Is " << stringTest << " accepted in this NFA?" << (nfa.isAccepted(stringTest) ? " Yes" : " No") << endl;
}


