#ifndef MALLOC_H
#define MALLOC_H

#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

void *malloc(unsigned long size);

#endif
