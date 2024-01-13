#ifndef PROCESS
#define PROCESS
#define MAX_ARGS 11
struct Process
{
    char *name;
    char *args[10];
    int numArgs;
};
typedef struct Process Process;
#endif

Process *createProcess(char *name);