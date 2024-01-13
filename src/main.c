#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "darr.h"
#include "process.h"

int getNumProcesses(int argc, char *argv[])
{
    int numColons = 0;
    for (int i = 0; i < argc; i++)
    {
        char *currentArg = argv[i];
        if (strcmp(":", currentArg) == 0)
        {
            numColons++;
        }
    }
    int numProcesses = numColons + 1;
    return numProcesses;
}

Process **parseArgs(int argc, char *argv[], int numProcesses)
{
    bool isNewProcess = true;
    bool isSplit;
    Process *process;
    Process **processes = (Process **)calloc(numProcesses, sizeof(Process *));
    int processIndex = 0;
    for (int i = 1; i < argc; i++)
    {
        isSplit = strcmp(":", argv[i]) == 0;
        if (isSplit)
        {
            isNewProcess = true;
            continue;
        }

        if (isNewProcess)
        {
            isNewProcess = false;
            process = createProcess(argv[i]);
            processes[processIndex] = process;
            processIndex++;
        }
        // adding the argument. (even if the argument is the name of the process)
        process->args[process->numArgs] = argv[i];
        process->numArgs++;
    }
    return processes;
}

void execEach(Process **processes, int numProcesses)
{
    for (int i = 0; i < numProcesses; i++)
    {
        Process *process = processes[i];
        if (process == NULL)
        {
            printf("process %d is NULL\n", i);
            continue;
        }
        if (fork() == 0)
        {
            execvp(process->name, process->args);
        }
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    int numProcesses = getNumProcesses(argc, argv);
    Process **processes = parseArgs(argc, argv, numProcesses);
    printf("numProcesses is %d\n", numProcesses);
    execEach(processes, numProcesses);

    // cleanup
    for (int i = 0; i < numProcesses; i++)
    {
        free(processes[i]);
    }
    free(processes);
}