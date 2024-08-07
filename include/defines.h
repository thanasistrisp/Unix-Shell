#pragma once

#include <iostream>
#include <string>
#include <cstring>

#include <csignal>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>
#include <map>
#include "fixedQueue.h"

#include "helper_fun.h"

#define HISTORY_SIZE 20

#include "mysh.h" // last for no circular dependency
