#include "mysh.h"

using namespace std;

bool mysh::cd(vector<string> args)
{
	if (args.size() > 2)
	{
		cout << "Error: too many arguments" << endl;
		exit_status = 2;
		return false;
	}
	char *temp = get_current_dir_name();
	string current_dir(temp);
	free(temp);
	setenv("PWD", current_dir.c_str(), 1);
	string arg;
	if (args.size() == 1 || args[1] == "~") {
		arg = getenv("HOME");
	}
	else if (args[1] == "-") {
		if (getenv("OLDPWD") == NULL) {
			cout << "Error: OLDPWD not set" << endl;
			return false;
		}
		arg = getenv("OLDPWD");
		cout << getenv("OLDPWD") << endl;
	}
	else
		arg = args[1];

	if (chdir(arg.c_str()) == -1) {
		perror("Error");
		exit_status = 1;
		return false;
	}
	setenv("OLDPWD", current_dir.c_str(), 1);
	return true;
}
