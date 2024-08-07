#include "mysh.h"

using namespace std;

mysh::mysh()
{
	// replace with sigaction as signal is deprecated
	struct sigaction sa;
	sa.sa_handler = ctrl_c;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	
	sa.sa_handler = ctrl_z;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGTSTP, &sa, NULL);

	sa.sa_handler = resume_parent;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &sa, NULL);

	// initialize alias map
	alias_map["ls"] = "ls --color=auto";
}


void mysh::run()
{
	cout << shell_prompt;
	while (getline(cin, input))
	{
		if (input == "exit")
			return;

		vector<string> commands = tokenize_input(input, ";");
		for (auto command : commands)
		{
			alias_used = false;
			if (command.find("|") != string::npos)
				multi_pipe(command);
			else
				execute_command(command);
		}

		if (history_command)
			history_command = false;
		else
			history.push_back(input);

		cout << shell_prompt;
		
	}
	if (!ctrl_c_pressed)
		cout << endl;
}

void mysh::execute_command(string input)
{
	// if only spaces and tabs return
	if (input.find_first_not_of(" \t") == string::npos)
		return;

	check_bg(input);

	mysh::tk token = split_tokens(input);

	// check if build in commands --> no fork
	if (token != mysh::T_OTHERS) {
		builtin_commands(token, input);
		return;
	}
	
	pid_t pid = fork();
	child_pid = pid;
	if (pid == 0)
	{
		expand_command(input);
		wildcards(input);
		vector<string> args_v = tokenize_unify(input);
		char **args = convert_to_char_array(args_v);
		// child process
		int code = execvp(args[0], args);
		if (code) {
			perror("Error");
			exit(1); // still reachable memory leak, false positive, as when child done, parent will free memory
					// and there are not definitely lost errors
		}
		exit(0);
	}
	else if (pid > 0)
	{
		// parent process
		if (!bg_process)
			pause(); // need to wait for signal --> either when child done or ctrl-z	
		else {
			cout << "Process " << pid << " started" << endl;
			bg_process = false;
		}		
	}
	else
	{
		// error
		cout << "Error: fork failed" << endl;
	}

	child_pid = -1;
}

void mysh::builtin_commands(mysh::tk token, string input)
{
	expand_command(input);
	wildcards(input);
	vector<string> args_v = tokenize_unify(input);
	switch (token)
	{
	case mysh::T_CD:
		cd(args_v);
		break;
	case mysh::T_CREATE_ALIAS:
		set_alias(args_v);
		break;
	case mysh::T_DESTROY_ALIAS:
		remove_alias(args_v);
		break;
	case mysh::T_ALIAS:
		alias_used = true;
		execute_command(convert_alias_str(args_v));
		break;
	case mysh::T_HISTORY:
		history_command = true;
		show_history(args_v);
		break;
	default:
		break;
	}
}
