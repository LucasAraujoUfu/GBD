#ifndef DISCO_H
#define DISCO_H

#include <cstdio>
#include <cstdlib>

#define SUCCESS 0
#define MEMORY_ERROR -1
#define ITEM_NOT_FOUND -2

struct registro{
    int32_t nseq;
    char text[96];
};

FILE* createHeapFile(int nmr_registros);
int pushBackRegister(FILE *file, registro r);
int readSeq(FILE *file, int nseq);
int updateSeq(FILE *file, int nseq, char newText[96]);
int deleteSeq(FILE *file, int nseq);
int readRandom(FILE *file, int nseq);
int updateRandom(FILE *file, int nseq, char newText[96]);
int deleteRandom(FILE *file, int nseq);
void printRegistro(registro r);
int writeRegistro(FILE *file, registro r);
int readRegistro(FILE *file,registro *r);


#endif