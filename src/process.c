#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"

Process *createProcess(char *name) {
  Process *process = (Process *)calloc(1, sizeof(Process));

  // appending "./" to the name of the process so it can run local executables
  char cd[] = "./";
  char *nameSpace = (char *)calloc(MAX_NAME_LEN, sizeof(char));
  strcat(nameSpace, cd);
  strcat(nameSpace, name);

  process->name = nameSpace;
  process->numArgs = 0;
  process->hasStarted = false;
  return process;
}

void printArgs(Process *process) {
  for (int i = 0; i < process->numArgs; i++) {
    printf("%s ", process->args[i]);
  }
  printf("\n");
}
