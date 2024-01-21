#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "darr.h"
#include "dll.h"
#include "process.h"

#define MS 1000

int getNumProcesses(int argc, char *argv[]) {
  int numColons = 0;

  for (int i = 0; i < argc; i++) {
    char *currentArg = argv[i];
    if (strcmp(":", currentArg) == 0) {
      numColons++;
    }
  }
  int numProcesses = numColons + 1;
  return numProcesses;
}

dll *parseArgs(int argc, char *argv[]) {
  bool isNewProcess = true;
  bool isSplit;
  Process *process;
  dll *processesHead = NULL;
  dll *processesTail = NULL;
  for (int i = 1; i < argc; i++) {
    isSplit = strcmp(":", argv[i]) == 0;
    if (isSplit) {
      isNewProcess = true;
      continue;
    }

    if (isNewProcess) {
      isNewProcess = false;
      process = createProcess(argv[i]);
      if (processesHead == NULL) {
        processesHead = dllCreate(process);
        processesTail = processesHead;
      } else {

        dllAppend(processesHead, &processesTail, process);
      }
    }
    // adding the argument. (even if the argument is the name of the process)
    process->args[process->numArgs] = argv[i];
    process->numArgs++;
  }

  // Making the dll circular
  if (processesTail != NULL) {
    processesTail->next = processesHead;
    processesHead->prev = processesTail;
  }
  return processesHead;
}

void timer_handler(int signum, siginfo_t *info, void *context) {
  // This function will be called when the timer expires
  printf("Timer expired!\n");
  printf("Passed in int is %d", *(int *)context);
}

void schedule(dll *processes) {
  // setting up singnal handling
  struct itimerval timer;
  struct timeval timeVal = {.tv_sec = 0, .tv_usec = 500 * MS};
  struct timeval intVal = {.tv_sec = 0, .tv_usec = 500 * MS};
  timer.it_value = timeVal;
  timer.it_interval = intVal;
  setitimer(ITIMER_REAL, &timer, NULL);

  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = timer_handler;

  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  bool isListEmpty = false;
  while (!isListEmpty) {
    Process *process = processes->val;
    if (process == NULL) {
      printf("process is NULL\n");
      continue;
    }
    // starting
    process->hasStarted = true;
    int childPid = fork();
    if (childPid == 0) {
      execvp(process->name, process->args);
    }
    waitpid(childPid, NULL, 0);
    processes = processes->next;
    isListEmpty = dllRemove((processes->prev));
  }
}

int main(int argc, char *argv[]) {
  // int numProcesses = getNumProcesses(argc, argv);
  dll *processes = parseArgs(argc, argv);
  schedule(processes);

  // // cleanup
  // for (int i = 0; i < numProcesses; i++)
  // {
  //     free(processes[i]);
  // }
  // free(processes);
}
