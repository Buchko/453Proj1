#include "process.h"
#include <stdlib.h>

Process *createProcess(char *name)
{
    Process *process = (Process *)calloc(1, sizeof(Process));
    process->name = name;
    process->numArgs = 0;
    process->hasStarted = false;
    return process;
}