#ifndef DLL_H
#define DLL_H

// Doubly linked list
struct dll
{
    void *val;
    struct dll *next;
    struct dll *prev;
};
typedef struct dll dll;
#endif

dll *dllAppend(dll *head, dll **tail, void *val);
dll *dllCreate(void *val);
int dllRemove(dll *node);