#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>

#define MAX_NAME_LEN 255

/*creating an array of void pointers that can resize itself if needed*/
/*darr for dynamic array*/
struct darr
{
    void **table;
    int nentries; /*num entries*/
    int size;
};

struct darr *create_darr(int size)
{
    /*intalizes an darr with a given size */

    struct darr *darp = (struct darr *)malloc(sizeof(struct darr));
    void **table = (void **)calloc(sizeof(void *), size);
    darp->table = table;
    darp->nentries = 0;
    darp->size = size;
    return darp;
}

struct darr *resize_darr(struct darr *darp, int newsize)
{
    /*resizes a darr to a new size*/

    struct darr *ndarp = create_darr(newsize);
    int i;
    /*copying the table*/
    for (i = 0; i < darp->nentries; i++)
    {
        ndarp->table[i] = darp->table[i];
    }

    free(darp->table);
    ndarp->nentries = darp->nentries;
    free(darp);
    return ndarp;
}

struct darr *darr_add(struct darr *darp, void *s)
{
    /*adds a new str to the table, resizing if needed*/
    if (darp->nentries == darp->size)
    {
        darp = resize_darr(darp, darp->size * 2);
    }
    darp->table[darp->nentries] = s;
    darp->nentries++;
    return darp;
}
/*
int main(){
    struct darr *mydarp = create_darr(5);
    char *s = "hello";
    mydarp = darr_add(mydarp, (void*)s);
    printf("%s\n", (char*)mydarp->table[0]);
    return 0;
}*/