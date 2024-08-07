#pragma once

#include "defines.h"

class mysh
{
private:
	static void ctrl_c(int sig);
	static void ctrl_z(int sig);
	static void resume_parent(int sig);

	static int exit_status;
	static bool ctrl_c_pressed;
	
	static const pid_t shell_pid;
	static pid_t child_pid;
	static const std::string shell_prompt;


	std::string input;
	int input_red_fd = -1;
	int output_red_fd = -1;
	bool bg_process = false;
	bool alias_used = false;
	bool history_command = false;

	void execute_command(std::string); // execute known commands (from path variable)

	void multi_pipe(std::string);
	void wildcards(std::string &input);


	// Custom commands that need to be implemented separately
	bool cd(std::vector<std::string> args);
	bool echo(std::vector<std::string> args);

	std::map<std::string, std::string> alias_map;
	void set_alias(std::vector<std::string> &args);
	bool check_alias(std::string alias);
	void remove_alias(std::vector<std::string> &args);
	std::string convert_alias_str(const std::vector<std::string> &args);

	// Parsing
	enum  tk
	{ // tokens in a command
		T_CD,
		T_ECHO,
		T_CREATE_ALIAS,
		T_DESTROY_ALIAS,
		T_ALIAS,
		T_HISTORY,
		T_OTHERS // no build in command (external)
	};

	fixed_queue<std::string> history{HISTORY_SIZE};

	void show_history(std::vector<std::string> args);

	void builtin_commands(tk token, std::string input);

	tk split_tokens(const std::string input); // split tokens in a command

	std::vector<std::string> tokenize_input(const std::string input, const char *delim); // tokenize input string to vector of strings
	std::vector<std::string> tokenize_unify(const std::string input); // tokenize input string to vector of strings

	void expand_command(std::string &input); // expand command to different techniques, changes input
	void check_bg(std::string &input);
	

public:
	void input_redirection(const char *arg);
	void output_redirection(const char *arg);
	void append_redirection(const char *arg);
	void background_process() { bg_process = true;}

	tk command(std::vector<std::string>);

	mysh();
	void run();
};
