#include <signal.h>
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
#define DEBUG false
#define debug_print(fmt, ...)                                                  \
  do {                                                                         \
    if (DEBUG)                                                                 \
      fprintf(stderr, fmt, __VA_ARGS__);                                       \
  } while (0)

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
  debug_print("%s", "Timer expired!\n");
}

void setupTimer() {
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
}

void schedule(dll *processes) {
  setupTimer();

  bool isListEmpty = false;
  while (!isListEmpty) {
    Process *process = processes->val;
    if (process == NULL) {
      printf("process is null\n");
      continue;
    }

    // starting or continuing the process
    if (!process->hasStarted) {
      process->hasStarted = true;
      int childPid = fork();
      if (childPid == 0) {
        debug_print("executing process %s\n", process->name);
        execvp(process->name, process->args);
      }
      process->pid = childPid;
    } else {
      debug_print("continuing process %s\n", process->name);
      kill(process->pid, SIGCONT);
    }

    // waiting on the process
    int status = -1;
    waitpid(process->pid, &status, 0);
    debug_print("finished waiting, status is %d\n", status);

    // handling either timer interrupt or process exit
    processes = processes->next;
    if (WIFEXITED(status)) {
      isListEmpty = dllRemove((processes->prev));
    } else {
      kill(process->pid, SIGSTOP);
    }
  }
}

int main(int argc, char *argv[]) {
  dll *processes = parseArgs(argc, argv);
  schedule(processes);
}
