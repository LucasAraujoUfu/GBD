#include <cstdio>
#include <cstdlib>

#include "../lib/disco.hpp"

#define SUCCESS 0
#define FAIL 1

void test_pushBackRegister(){
    registro r = {0,"testando 123"};

    int f = pushBackRegister(NULL, r);
    if(f != MEMORY_ERROR)
        fprintf(stderr, "pushBackRegister - Fail (não retornou MEMORY_ERROR)\n");
    else
        fprintf(stderr, "pushBackRegister - OK\n");

    
    FILE *file = fopen("test.bin", "wb");

    if(file==NULL)
        fprintf(stderr, "Erro ao criar o arquivo para teste");
    else{
        f = pushBackRegister(file, r);

        if(f != SUCCESS)
            fprintf(stderr, "pushBackRegister - Fail (não retornou SUCCESS)\n");
        else{
            //verificar se o arquivo está correto.
            fprintf(stderr, "pushBackRegister - OK\n");
        }
    }
}

void test_readSeq();
