#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 400

// Definição da estrutura do registro de aluno
struct Aluno {
    int seq_aluno;
    char codigo_curso[4];
    char nome_aluno[34];
};

// Definição da estrutura do registro de curso
struct Curso {
    int seq_curso;
    char codigo_curso[4];
    char nome_curso[18];
};

// Função para ler uma página do arquivo de alunos
int readAlunoPage(FILE* file, struct Aluno* page, int pageNumber) {
    fseek(file, pageNumber * PAGE_SIZE, SEEK_SET);
    return fread(page, sizeof(struct Aluno), PAGE_SIZE / sizeof(struct Aluno), file);
}

// Função para ler uma página do arquivo de cursos
int readCursoPage(FILE* file, struct Curso* page, int pageNumber) {
    fseek(file, pageNumber * PAGE_SIZE, SEEK_SET);
    return fread(page, sizeof(struct Curso), PAGE_SIZE / sizeof(struct Curso), file);
}

// Função para realizar a junção dos registros
void mergeJoin(FILE* alunoFile, FILE* cursoFile, int bufferPoolSize, int m, int n) {
    struct Aluno alunoPage[PAGE_SIZE / sizeof(struct Aluno)];
    struct Curso cursoPage[PAGE_SIZE / sizeof(struct Curso)];

    int alunoPageNumber = 0;
    int cursoPageNumber = 0;

    // Buffer Pool para alunos e cursos
    struct Aluno* alunoBufferPool = (struct Aluno*)malloc(PAGE_SIZE * bufferPoolSize);
    struct Curso* cursoBufferPool = (struct Curso*)malloc(PAGE_SIZE * bufferPoolSize);
    int alunoBufferIndex = 0;
    int cursoBufferIndex = 0;

    int alunoBufferFilled = readAlunoPage(alunoFile, alunoBufferPool, alunoPageNumber);
    alunoPageNumber++;
    int cursoBufferFilled = readCursoPage(cursoFile, cursoBufferPool, cursoPageNumber);
    cursoPageNumber++;

    while (alunoBufferFilled > 0) {
        int alunoIndex = 0;
        int cursoIndex = 0;

        while (alunoIndex < alunoBufferFilled && cursoIndex < cursoBufferFilled) {
            if (strcmp(alunoBufferPool[alunoIndex].codigo_curso, cursoBufferPool[cursoIndex].codigo_curso) < 0) {
                alunoIndex++;
            } else if (strcmp(alunoBufferPool[alunoIndex].codigo_curso, cursoBufferPool[cursoIndex].codigo_curso) > 0) {
                cursoIndex++;
            } else {
                // Encontrou uma correspondência, imprime os dados
                printf("SEQ_ALUNO: %d\n", alunoBufferPool[alunoIndex].seq_aluno);
                printf("CODIGO_CURSO: %s\n", alunoBufferPool[alunoIndex].codigo_curso);
                printf("NOME_ALUNO: %s\n\n", alunoBufferPool[alunoIndex].nome_aluno);
                
                // Avança para o próximo registro do buffer de alunos
                alunoIndex++;
            }
        }

        if (alunoIndex == alunoBufferFilled) {
            // Buffer de alunos totalmente percorrido, carrega a próxima página
            alunoBufferFilled = readAlunoPage(alunoFile, alunoBufferPool, alunoPageNumber);
            alunoPageNumber++;
        }

        if(alunoPageNumber>=m)return;

        if (cursoIndex == cursoBufferFilled) {
            // Buffer de cursos totalmente percorrido, carrega a próxima página
            cursoBufferFilled = readCursoPage(cursoFile, cursoBufferPool, cursoPageNumber);
            cursoPageNumber++;
        }

        if(cursoPageNumber>=n)return;
    }

    free(alunoBufferPool);
    free(cursoBufferPool);
}

int main() {
    int n,m;
    printf("Digite o tamanho (em paginas) do arquivo de alunos (N): ");
    scanf("%d",&n);
    printf("Digite o tamanho (em paginas) do arquivo de cursos (M): ");
    scanf("%d",&m);

    FILE* alunoFile = fopen("aluno.data", "rb");
    FILE* cursoFile = fopen("curso.data", "rb");

    if (!alunoFile || !cursoFile) {
        printf("Erro ao abrir os arquivos de entrada.\n");
        return 1;
    }

    int bufferPoolSize;
    printf("Digite o tamanho do buffer pool (B): ");
    scanf("%d", &bufferPoolSize);

    // Realiza a junção dos registros
    mergeJoin(alunoFile, cursoFile, bufferPoolSize,m,n);

    fclose(alunoFile);
    fclose(cursoFile);

    return 0;
}
