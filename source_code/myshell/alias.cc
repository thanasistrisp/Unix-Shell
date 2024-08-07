#include "mysh.h"

using namespace std;

void mysh::set_alias(vector<string> &args)
{
	if (args.size() != 3)
	{
		cout << "Usage: createalias <aliasname> <\"command\">" << endl;
		return;
	}
	string alias_name = args[1];
	string command = args[2];
	alias_map[alias_name] = command;
}

void mysh::remove_alias(vector<string> &args)
{
	if (args.size() != 2)
	{
		cout << "Usage: removealias <aliasname>" << endl;
		return;
	}
	string alias_name = args[1];
	alias_map.erase(alias_name);
}

bool mysh::check_alias(string alias)
{
	if (alias_map.find(alias) != alias_map.end())
		return true;
	return false;
}

string mysh::convert_alias_str(const vector<string> &args)
{
	// without using convert_alias
	string alias = args[0];
	string command = alias_map[alias];
	string command_args = command;
	for (uint i = 1; i < args.size(); i++)
		command_args += " " + args[i];
	return command_args;
}
