#include "mysh.h"
#include <wordexp.h> // used for wildcard expansion
#include <sstream>

using namespace std;

bool check_wildcard(string input);
void check_for_exit_status(string &input, int code);

void mysh::wildcards(string &input)
{
	if (check_wildcard(input))
		return;
	check_for_exit_status(input, exit_status);
	// preserve quotes by escaping them
	for (uint i = 0; i < input.length(); i++)
	{
		if (input[i] == '"')
		{
			input.insert(i, "\\");
			i++;
		}
	}
	wordexp_t p;
	wordexp(input.c_str(), &p, 0);
	input = "";
	for (uint i = 0; i < p.we_wordc; i++)
	{
		input += p.we_wordv[i];
		input += " ";
	}
	wordfree(&p);
}

// from documentation to prevent segfault
bool check_wildcard(string input)
{
	for (uint i = 0; i < input.length(); i++)
	{
		if (input[i] == '|' || input[i] == '&' || input[i] == ';' || input[i] == '<' || input[i] == '>' || input[i] == '(' 
			|| input[i] == ')' || input[i] == '{' || input[i] == '}')
			return true;
	}
	return false;
}

void check_for_exit_status(string &input, int code)
{
	if (input.find("$?") != string::npos)
	{
		stringstream ss;
		ss << code;
		string exit_code = ss.str();
		input.replace(input.find("$?"), 2, exit_code);
	}
}
