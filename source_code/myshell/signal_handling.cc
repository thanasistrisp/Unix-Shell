#include "mysh.h"

using namespace std;

void mysh::ctrl_c(int sig)
{
	pid_t pid = getpid();
	if (pid != shell_pid)
		return;
	if (child_pid != -1) {
		kill(child_pid, SIGINT);
		return;
	}
	signal(sig, ctrl_c);
	cout << '\n' + shell_prompt;
	fflush(stdout);
	ctrl_c_pressed = true;
}

void mysh::ctrl_z(int sig)
{
	pid_t pid = getpid();
	if (pid != shell_pid)
		return;
	if (child_pid != -1) { // child process is running
		cout << '\n' << "Process " << child_pid << " stopped" << endl;
		kill(child_pid, SIGTSTP);
		fflush(stdout);
		return;
	}
	signal(sig, ctrl_z);
	cout << "\b\b  \b\b"; // clears ctrl-z from screen (default shell behavior)
  	fflush(stdout);
}

void mysh::resume_parent(int sig)
{
	signal(sig, resume_parent);
	// capture child exit status
	waitpid(-1, &exit_status, WNOHANG);
	if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	else
		exit_status = 0;
}
