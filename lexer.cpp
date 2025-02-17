#include "lexer.hpp"

lexer::lexer(const string& input)
{
	this->input = input;
	this->pos = 0;
}

//returns the token from the input string
Token lexer::getToken()
{
	
	//skip whitespace
	while (pos < input.size() && isspace(input[pos]))
	{
		
		pos++;
	}
	//checking for the end of string
		if (pos >= input.size())
		{
			return Token("EOS", "");
		}

		size_t LongestMatchLength = 0;
		string longestTokenMatch;
		string TokenMatchName;
		
		
			//checking if any dfa in the list is accepted by the current string
			for (size_t i = 0; i < DFA_List.size(); i++)
			{
				for (size_t len = 1; (len + pos) <= input.size(); len++)
				{
					string subStr = input.substr(pos, len);

					if (subStr.empty())
					{
						continue;
					}

					
					
					//if the dfa is accepted by the substring we get the token name and substring 
					if (DFA_List[i].isAccepted(subStr))
					{

						if (subStr.length() > LongestMatchLength)
						{
							LongestMatchLength = subStr.length();
							longestTokenMatch = subStr;
							TokenMatchName = tokens[i].name;
						}
					}
				}
			}

			if (LongestMatchLength > 0)
			{
				
				pos += LongestMatchLength;
				return Token(TokenMatchName, longestTokenMatch);
			}
			string invalidChar(1, input[pos]);
			pos++;
			return Token("Error", invalidChar);
		}
	

	

//adding token to token vector
void lexer::addToken(string& tokenName)
{
	Token token;
	token.name = tokenName;
	tokens.push_back(token);
}

//method to add DFA to vector
void lexer::addDFA(string& regex)
{
	//making an nfa with regex
	NFA nfa;
	nfa.RegexToNFA(regex);

	//making dfa from nfa and pushing into dfa vector
	DFA dfa;
	dfa = nfa.NFA2DFA();
	DFA_List.push_back(dfa);
}

void lexer::SetTokens(vector<Token> tokens)
{
	this->tokens = tokens;
}

void lexer::SetDFAList(vector<DFA> DFA_List)
{
	this->DFA_List = DFA_List;
}

void lexer::printTokens()
{
	cout << "\nLexer Token List:\n";
	for (size_t i = 0; i < tokens.size(); i++) {
		cout << "Token: " << tokens[i].name << endl;;
	}
}

void lexer::PrintDFAList()
{
	for (size_t i = 0; i < DFA_List.size(); i++) {
		DFA_List[i].Print();
	}
}

void lexer::PrintInputString()
{
	cout <<"Input String: " << input << endl;
}
