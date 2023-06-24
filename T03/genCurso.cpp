#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

struct Aluno {
    int32_t seq_aluno;
    char codigo_curso[4];
    char nome_aluno[34];
};

struct Curso {
    int32_t seq_curso;
    char codigo_curso[4];
    char nome_curso[18];
};

int main(void){
    int n=100;

    Curso c[100];    

    FILE *f = fopen("curso.data", "wb");

    for(int i=0;i<n;i++){
        Curso cs;
        cs.seq_curso = i;
        for(int i=0;i<16;i++){
            cs.nome_curso[i] = (rand()%26)+65;
        }
        cs.nome_curso[17] = 0;
        strcpy(cs.codigo_curso,cs.nome_curso);
        cs.codigo_curso[3] = 0;
        fwrite(&cs, sizeof(Curso), 1, f);
        c[i] = cs;
    }

    n=400;

    f = fopen("aluno.data", "wb");

    for(int i=0;i<n;i++){
        Aluno al;
        al.seq_aluno = i;
        for(int i=0;i<33;i++){
            al.nome_aluno[i] = (rand()%26)+65;
        }
        al.nome_aluno[33] = 0;
        int p =rand()%100;
        strcpy(al.codigo_curso,c[p].codigo_curso);
        fwrite(&al, sizeof(Curso), 1, f);
    }

    return 0;
}