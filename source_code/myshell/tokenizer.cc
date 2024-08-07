#include "mysh.h"

using namespace std;

static bool special_char(char c);

mysh::tk mysh::command(vector<string> args) {
	if (args[0] == "cd")
		return T_CD;
	else if (args[0] == "createalias")
		return T_CREATE_ALIAS;
	else if (args[0] == "destroyalias")
		return T_DESTROY_ALIAS;
	else if (check_alias(args[0]) && !alias_used)
		return T_ALIAS;
	else if (args[0] == "myHistory")
		return T_HISTORY;
	else
		return T_OTHERS;
	
}

mysh::tk mysh::split_tokens(const string input) {
	vector<string> args = tokenize_input(input, " \t");
	mysh::tk token = command(args);
	return token;
}

vector<string> mysh::tokenize_input(const string input, const char *delim)
{
	char *token = strtok((char *)input.c_str(), delim);
	vector<string> tokens;
	while (token != NULL)
	{
		string token_str = token;
		tokens.push_back(token_str);
		token = strtok(NULL, delim);
	}
	return tokens;
}

vector<string> mysh::tokenize_unify(const string input)
{
	// if quotes found, then do not tokenize inside quotes and add it as a single argument
	vector<string> args;
	bool quote = false;
	string arg = "";
	for (uint i = 0; i < input.length(); i++)
	{
		if (input[i] == '"')
		{
			if (quote)
			{
				args.push_back(arg);
				arg = "";
				quote = false;
			}
			else
				quote = true;
		}
		else if (input[i] == ' ' || input[i] == '\t')
		{
			if (quote)
				arg += input[i];
			else
			{
				if (arg != "")
				{
					args.push_back(arg);
					arg = "";
				}
			}
		}
		else
			arg += input[i];
	}
	if (arg != "")
		args.push_back(arg);
	return args;
}

void mysh::expand_command(string &input) // state finite machine for pattern matching
{
	for (uint i = 0; i < input.length(); i++)
	{
		if (input[i] == '<')
		{
			string file = "";
			int j = i + 1;
			while (isspace(input[j]))
				j++;
			while (!isspace(input[j]) && input[j] != '\0' && !special_char(input[j]))
			{
				file += input[j];
				j++;
			}
			input_redirection(file.c_str());
			if (special_char(input[j]))
				input.replace(i, j - i, " ");
			else
				input.replace(i, j - i, "");
		}
		else if (input[i] == '>')
		{
			if (input[i + 1] == '>')
			{
				string file = "";
				int j = i + 2;
				while (isspace(input[j]))
					j++;
				while (!isspace(input[j]) && input[j] != '\0')
				{
					file += input[j];
					j++;
				}
				append_redirection(file.c_str());
				input.replace(i, j - i, "");
			}
			else
			{
				string file = "";
				int j = i + 1;
				while (isspace(input[j]))
					j++;
				while (!isspace(input[j]) && input[j] != '\0')
				{
					file += input[j];
					j++;
				}
				output_redirection(file.c_str());
				input.replace(i, j - i, "");
			}
		}
	}
}

void mysh::check_bg(string &input)
{
	for (uint i = 0; i < input.length(); i++)
		// if last char is & and after it there are only spaces
		if (input[i] == '&' && (i == input.length() - 1 || isspace(input[i + 1])))
		{
			background_process();
			input.replace(i, 1, "");
		}
}

static bool special_char(char c)
{
	return c == '&' || c == '<' || c == '>';
}
