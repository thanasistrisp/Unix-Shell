#include "mysh.h"

using namespace std;

void mysh::show_history(vector<string> args)
{
	if (args.size() == 1)
	{
		if (history.size() == 0) {
			cout << "No history" << endl;
			return;
		}
		for (int i = history.size() - 1; i >= 0; i--)
			cout << history.size() - i << " " << history[i] << endl;
		return;
	}
	string result = "";
	if (args.size() > 2)
		cout << "Invalid arguments" << endl;
	int index = stoi(args[1]) - 1;
	if (index < 0 || (uint)index >= history.size())
	{
		cout << "Invalid index" << endl;
		return;
	}
	result = history[history.size() - index - 1];
	vector<string> args_v = tokenize_unify(result);
	if (args_v[0] == "back")
	{
		cout << "Cannot go back to back command. It could cause infinite loop!" << endl;
		return;
	}
	history.push_back(result);
	execute_command(result);
}
