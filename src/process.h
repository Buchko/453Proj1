#ifndef PROCESS
#define PROCESS
#define MAX_ARGS 10
struct Process
{
    char *name;
    char *args[10];
    int numArgs;
};
typedef struct Process Process;
#endif

Process *createProcess(char *name);