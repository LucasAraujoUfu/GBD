#ifndef DISCO_C
#define DISCO_C

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "disco.hpp"


FILE * createHeapFile(int nmr_registros){
    FILE *heapFile = fopen("heapFile.bin","wb+");
    if(heapFile==NULL)return NULL;
    for(int i=0;i<nmr_registros;i++){
        registro nr;
        nr.nseq = i;
        strcpy(nr.text, "testando123");
        pushBackRegister(heapFile,nr);
    }

    return heapFile;
}

int pushBackRegister(FILE *file, registro nr){
    if(file == NULL) return MEMORY_ERROR;
    fseek(file, 0, SEEK_END);
    writeRegistro(file, nr);
    return SUCCESS;
}

int readSeq(FILE *file, int nseq){
    if(file == NULL)return MEMORY_ERROR;
    registro r;
    fseek(file, 0, SEEK_SET);
    while(readRegistro(file, &r)!=EOF){
        if(r.nseq == nseq){
            printRegistro(r);
            fseek(file, 0, SEEK_SET);
            return SUCCESS;
        }
    }
    fseek(file, 0, SEEK_SET);
    return ITEM_NOT_FOUND;
}

int updateSeq(FILE *file, int nseq, char newText[96]){
    if(file == NULL)return MEMORY_ERROR;
    fseek(file, 0, SEEK_SET);
    registro r;
    while(readRegistro(file, &r)!=EOF){
        if(r.nseq == nseq){
            strcpy(r.text,newText);
            fseek(file, -sizeof(registro), SEEK_CUR);
            return writeRegistro(file, r);
        }
    }
    return ITEM_NOT_FOUND;
}

int deleteSeq(FILE *file, int nseq){
    if(file == NULL)return MEMORY_ERROR;
    fseek(file, 0, SEEK_SET);
    registro r;
    while(readRegistro(file, &r)!=EOF){
        if(r.nseq == nseq){
            fseek(file, -sizeof(registro), SEEK_CUR);
            r.nseq = -r.nseq;
            writeRegistro(file,r);
            fseek(file, 0, SEEK_SET);
            return SUCCESS;
        }
    }
    return ITEM_NOT_FOUND;
}

int readRandom(FILE *file, int nseq){
    if(file == NULL)return MEMORY_ERROR;
    int f = fseek(file, nseq*sizeof(registro), SEEK_SET);
    if(f)return ITEM_NOT_FOUND;
    registro r;
    readRegistro(file, &r);
    if(r.nseq<0)return ITEM_NOT_FOUND;
    printRegistro(r);
    return SUCCESS;
}

int updateRandom(FILE *file, int nseq, char newText[96]){
    if(file == NULL)return MEMORY_ERROR;
    int f = fseek(file, nseq*sizeof(registro), SEEK_SET);
    if(f)return ITEM_NOT_FOUND;
    registro r;
    r.nseq = nseq;
    strcpy(r.text, newText);
    return writeRegistro(file, r);
}

int deleteRandom(FILE *file, int nseq){
    if(file == NULL)return MEMORY_ERROR;
    int f = fseek(file, nseq*sizeof(registro), SEEK_SET);
    if(f)return ITEM_NOT_FOUND;
    registro r = {-nseq,""};
    return writeRegistro(file, r);
}

void printRegistro(registro r){
    printf("%d - %s\n",r.nseq,r.text);
}

int writeRegistro(FILE *file, registro r){
    if(file == NULL)return MEMORY_ERROR;
    fwrite(&r,sizeof(registro),1,file);
    return SUCCESS;
}

int readRegistro(FILE *file,registro *r){
    if(file == NULL)return MEMORY_ERROR;
    return fread(r, sizeof(registro), 1, file);
}

#endif