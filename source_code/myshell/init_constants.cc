#include "mysh.h"

using namespace std;

// Static member variables

const string mysh::shell_prompt = "in-mysh-now:> ";
bool mysh::ctrl_c_pressed = false;
const pid_t mysh::shell_pid = getpid();
pid_t mysh::child_pid = -1;
int mysh::exit_status = 0;
