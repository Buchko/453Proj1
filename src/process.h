#ifndef PROCESS
#define PROCESS

#include <stdbool.h>
#define MAX_ARGS 11
#define MAX_NAME_LEN 103 // 100 for name length and 2 for ./ and 1 for NULL
struct Process {
  char *name;
  char *args[10];
  int numArgs;
  bool hasStarted;
  int pid;
};
typedef struct Process Process;
#endif

Process *createProcess(char *name);
void printArgs(Process *p);
