#include <stdlib.h>
#include <stdbool.h>
#include "dll.h"

dll *dllAppend(dll *head, dll **tail, void *val)
{
    dll *newNode = (dll *)calloc(1, sizeof(dll));
    newNode->val = val;
    (*tail)->next = newNode;
    newNode->prev = *tail;
    *tail = newNode;
    return head;
}

dll *dllCreate(void *val)
{
    dll *node = (dll *)calloc(1, sizeof(dll));
    node->val = val;
    return node;
}

int dllRemove(dll *node)
// returns 1 if this deletes the last node in the list, 0 otherwise
{
    bool onlyOne = (node->next == node);
    if (!onlyOne)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        return 0;
    }
    free(node);
    return 1;
}