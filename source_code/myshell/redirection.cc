#include "mysh.h"

using namespace std;


void mysh::input_redirection(const char *arg)
{
	int fd = open(arg, O_RDONLY); // O_RDONLY: read only
	if (fd == -1)
	{
		perror("mysh");
		return;
	}
	else
	{
		input_red_fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void mysh::output_redirection(const char *arg)
{
	int fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0666); // 0666: rw-rw-rw-
	if (fd == -1)
	{
		perror("mysh");
		return;
	}
	else
	{
		output_red_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void mysh::append_redirection(const char *arg)
{
	int fd = open(arg, O_WRONLY | O_CREAT | O_APPEND, 0666); // 0666: rw-rw-rw-
	if (fd == -1)
	{
		perror("mysh");
		return;
	}
	else
	{
		output_red_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
