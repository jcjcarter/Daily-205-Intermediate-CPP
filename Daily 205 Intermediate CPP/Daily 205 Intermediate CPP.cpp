// Daily 205 Intermediate CPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std; // Toy projects only

					 // Handle the whitespace, and convert * to x.
string cleanInput(const string& in)
{
	auto out = string{};
	const auto validChars = string{ "0123456789+-*x/()" };
	copy_if(begin(in), end(in), back_inserter(out), [&validChars](char c) { return validChars.find(c) != validChars.npos; });
	transform(begin(out), end(out), begin(out), [](char c) {return (c == '*' ? 'x' : c); });
	return out;
}

// Pass iterator pointing to an opening bracket.
// Returns iterator pointing one past closing bracket.
template <class It, class ValType>
It advanceToCloseBracket(It start, It finish, ValType open, ValType close)
{
	auto i = start;
	advance(i, 1);
	while (i != finish)
	{
		if (*i == close)
		{
			return ++i;
		}
		if (*i == open)
		{
			i = advanceToCloseBracket(i, finish, open, close);
		}
		else
		{
			++i;
		}
	}
	cerr << "No closing bracket found.\n";
	return finish;
}

pair<string, string> splitAtSymbol(const string& in, char splitter)
{
	auto it = rbegin(in);
	while (it != rend(in))
	{
		auto ch = *it;
		if (ch == splitter)
		{
			const auto location = in.size() - distance(rbegin(in), it) - 1;
			return make_pair(in.substr(0, location), in.substr(location + 1));
		}
		else if (ch == ')')
		{
			it = advanceToCloseBracket(it, rend(in), ')', '(');
		}
		else
		{
			++it;
		}
	}
	return make_pair("", "");
}

string turnToRPN(const string& in)
{
	// Check for empty string.
	if (in.empty())
	{
		return "";
	}

	// If the whole thing is in brackets, deal with it.
	if (in.front() == '(' && advanceToCloseBracket(begin(in), end(in), '(', ')') == end(in))
	{
		return turnToRPN(in.substr(1, in.size() - 2));
	}

	// Parse for -, then +, then x, then /.
	const auto ops = string{ "-+x/" };
	char op = 0;
	auto result = pair<string, string>{};
	for (auto thisop : ops)
	{
		result = splitAtSymbol(in, thisop);
		if (!result.first.empty() && !result.second.empty())
		{
			op = thisop;
			break;
		}
	}

	if (op == 0)
	{
		return in;
	}

	auto output = ostringstream{};
	output << turnToRPN(result.first) << ' ' << turnToRPN(result.second) << ' ' << op << ' ';
	return output.str();
}

double solveRPNeqn(const string& in)
{
	auto stream = istringstream{ in };
	auto stack = vector<double>{};

	auto token = string{};
	;

	while (stream >> token)
	{
		if (isdigit(token.front()))
		{
			stack.push_back(stod(token));
		}
		else
		{
			if (stack.size() < 2)
			{
				cerr << "Not enough values on stack to perform RPN.\n";
				return 0.0;
			}
			const auto arg2 = stack.back();
			stack.pop_back();
			const auto arg1 = stack.back();
			stack.pop_back();
			const auto op = token.front();
			switch (op)
			{
			case 'x':
				stack.push_back(arg1*arg2);
				break;
			case '/':
				stack.push_back(arg1 / arg2);
				break;
			case '+':
				stack.push_back(arg1 + arg2);
				break;
			case '-':
				stack.push_back(arg1 - arg2);
				break;
			default:
				cerr << "Invalid operator: " << op << endl;
				return 0.0;
			}
		}
	}

	if (stack.empty())
	{
		cerr << "Invalid RPN: no result.\n";
		return 0.0;
	}
	else
	{
		return stack.back();
	}
}

int main()
{
	while (true)
	{
		auto line = string{};
		getline(cin, line);
		line = cleanInput(line);
		const auto result = turnToRPN(line);
		const auto solve = solveRPNeqn(result);
		cout << "Cleaned: " << line <<
			"\nRPN: " << result <<
			"\nSolution: " << solve << "\n\n";
	}
	return 0;
}
