#ifndef HEADER
#define HEADER

// tamanho dos campos alfa apresentados na tabela
#define TITLE_SIZE 300
#define AUTHOR_SIZE 150
#define UPDATE_SIZE 20
#define SNIPPET_SIZE 1024

#define COUNT_BLOCk_REG 10

#define N_BUCKETS 270973 /**/
#define DATA_FILE "arquivoDados"

//Tamanho do corpo do bloco
#define BLOCK_SIZE COUNT_BLOCk_REG * (12 + TITLE_SIZE +  AUTHOR_SIZE +  UPDATE_SIZE + SNIPPET_SIZE)

using namespace std;

struct Article{
    unsigned int id;
    char title[TITLE_SIZE];
    unsigned int year;
    char author[AUTHOR_SIZE];
    unsigned int citations;
    char update[UPDATE_SIZE];
    char snippet[SNIPPET_SIZE];
}; 

struct Block{
    unsigned int nRegisters; //Guarda informação sobre a quantidade de registros armazenados no bloco
    char body[BLOCK_SIZE];       //Vetor de artigos
};

void imprimirRegistroArt(Article article);

#endif