#ifndef PROCESS
#define PROCESS

#include <stdbool.h>
#define MAX_ARGS 11
struct Process
{
    char *name;
    char *args[10];
    int numArgs;
    bool hasStarted;
};
typedef struct Process Process;
#endif

Process *createProcess(char *name);