#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "darr.h"
#include "process.h"

int MAX_ARGUMENTS = 10;

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
            continue;
        }
        // parsing the arguements if it isn't a new process
        process->args[process->numArgs] = argv[i];
        process->numArgs++;
    }
    return processes;
}

int main(int argc, char *argv[])
{
    int numProcesses = getNumProcesses(argc, argv);
    Process **processes = parseArgs(argc, argv, numProcesses);
    printf("numProcesses is %d\n", numProcesses);

    // cleanup
    free(processes);
}