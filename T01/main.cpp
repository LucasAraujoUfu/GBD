#include <bits/types/time_t.h>
#include <cstdio>
#include <ctime>
#include <iostream>

#include "lib/disco.hpp"

#define SIZE 300000

using namespace std;

int main(){
    FILE *file = createHeapFile(SIZE);
    time_t in = time(NULL); 
    for (int i=0; i<SIZE; i++) {
        readSeq(file, i);
    }
    time_t fm = time(NULL);
    time_t res = fm-in;
    in = time(NULL);
    for (int i=0; i<SIZE; i++) {
        readRandom(file, i);
    }
    fm = time(NULL);
    printf("Sequencial - %d\n",res);
    printf("Ramdom - %d\n",fm-in);
    fclose(file);
    return 0;
}

