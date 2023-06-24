#ifndef HASH_C
#define HASH_C

#include "hash.hpp"
#include <cstdio>

struct hash_table{
    int deep;

    FILE *header;
    FILE *buckts;
} data_base = {0,NULL,NULL};



int hash(int n,int deep){
    int mask = 0;
    while(deep--){
        mask<<=1;
        mask++;
    }
    return n&mask;
}

int insert(registro r){
    int hs = hash(r.nseq, data_base.deep);
    fseek(data_base.buckts, hs*404,SEEK_SET);
    int bct;
    fread(&bct,sizeof(int),1,data_base.buckts);
    if(bct>=4){
        /// TODO: slpit e reinserts
    }
    fseek(data_base.buckts,-sizeof(int),SEEK_CUR);
    bct++;
    fwrite(&bct,sizeof(int),1,data_base.buckts);
    fseek(data_base.buckts,bct*100,SEEK_CUR);
    fwrite(&r,sizeof(registro),1,data_base.buckts);
    return SUCCESS;
}

registro find(int nseq){
    int hs = hash(nseq, data_base.deep);
    fseek(data_base.header, hs, SEEK_SET);
    int bct;
    fread(&bct, sizeof(int), 1, data_base.header);

    registro r = {};

    return r;
}

#endif