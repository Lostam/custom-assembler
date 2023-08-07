#ifndef _FPDEFS_H_
#define _FPDEFS_H_

/* A comparison function with the same symantics as strcmp */
typedef int (*cmpFunc)(void *, void *);
/* A function to delete an object */
typedef void (*deleteFunc)(void *);
/* A function to perform an action on an object */
typedef void (*actionFunc)(void *);

#endif
