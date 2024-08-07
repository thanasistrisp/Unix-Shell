#include "mysh.h"

using namespace std;

void mysh::multi_pipe(string input)
{
	vector<string> commands = tokenize_input(input, "|");
	int num_commands = commands.size();
	int pipefd[num_commands - 1][2];
	for (int i = 0; i < num_commands - 1; i++)
		pipe(pipefd[i]);

	for (int i = 0; i < num_commands; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			if (i != 0)
			{
				dup2(pipefd[i - 1][0], STDIN_FILENO);
				close(pipefd[i - 1][0]);
				close(pipefd[i - 1][1]);
			}
			if (i != num_commands - 1)
			{
				dup2(pipefd[i][1], STDOUT_FILENO);
				close(pipefd[i][0]);
				close(pipefd[i][1]);
			}
			for (int j = 0; j < num_commands - 1; j++)
			{
				close(pipefd[j][0]);
				close(pipefd[j][1]);
			}
			execute_command(commands[i]);
			exit(0);
		}
	}
	for (int i = 0; i < num_commands - 1; i++)
	{
		close(pipefd[i][0]);
		close(pipefd[i][1]);
	}
	for (int i = 0; i < num_commands; i++)
		wait(NULL);
}
