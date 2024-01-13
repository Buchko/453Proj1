#ifndef STRUCT_DARR
#define STRUCT_DARR
struct darr
{
	void **table;
	int nentries; /*num entries*/
	int size;
};
typedef struct darr darr;
#endif
struct darr *create_darr(int size);
struct darr *resize_darr(struct darr *darp, int newsize);
struct darr *darr_add(struct darr *darp, void *s);