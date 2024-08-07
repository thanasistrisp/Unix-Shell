#include "defines.h"

using namespace std;

string remove_whitespace(string str)
{
	string new_str = "";
	for (char c : str)
	{
		if (not isspace(c))
			new_str += c;
	}
	return new_str;
}

char **vector_to_char_array(vector<string> &command)
{
	char **args = new char *[command.size() + 1];
	int i = 0;
	for (auto elem : command)
	{
		args[i] = new char[elem.size() + 1];
		strcpy(args[i], elem.c_str());
		i++;
	}
	args[i] = NULL;
	return args;
}

char **convert_to_char_array(vector<string> args)
{
	char **argsarr = new char *[args.size() + 1];
	int i = 0;
	for (auto arg : args)
	{
		argsarr[i] = new char[arg.size() + 1];
		strcpy(argsarr[i], arg.c_str());
		i++;
	}
	argsarr[i] = NULL;
	return argsarr;
}
