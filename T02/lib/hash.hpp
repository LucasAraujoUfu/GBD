#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR -1

struct registro{
    int32_t nseq;
    char text[96];
};

int insert(registro r);
int remove(int nseq);
registro find(int nseq);


#endif