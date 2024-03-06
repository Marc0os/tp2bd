#include <iostream>     
#include <stdio.h>
#include <fstream>
#include <string.h>
#include "../data/header.hpp"
#include "hash.hpp"


int collisionCount = 0;

void inicializaArquivoDeSaida(fstream *f){
    Block buffer = {0};

    //Preenche os buckets com blocos vazios
    for(int i=0; i<N_BUCKETS; i++) {
        f->write((char*)&buffer,sizeof(Block));
    }
}

int hashing(int id) {
    // Função hashing modular. Converte dados de entrada em um valor de hash fixo.
    return id % N_BUCKETS;
}


Block consultaBucketPorId(fstream *f, int id) {

    Block buffer={0};

    int key = hashing(id);

    //Busca a posição do bucket no arquivo de dados
    f->seekg (key*sizeof(Block),ios::beg);

    //Copia o bloco correspondente no buffer
    f->read((char*)&buffer,sizeof(Block));

    return buffer;
}

void imprimirRegistroArt(Article article) {
    cout<< "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" <<
         "\n*ID: "             << article.id
        << "\n*Titulo: "         << article.title 
        << "\n*Ano: "            << article.year
        << "\n*Autor: "          << article.author
        << "\n*Citações:"        << article.citations
        << "\n*Atualização: "    << article.update
        << "\n*Snippet: "        << article.snippet 
        << "\n\n";
}

bool insereArquivoHash(fstream *f, Article article) {
	//Insere no arquivo de dados o ponteiro que identifica o artigo

    int i;
    Block buffer= consultaBucketPorId(f,article.id);
    Article *n_article = (Article*)&buffer.body;

    //Verifica se há espaço disponível no bloco
    if(buffer.nRegisters < COUNT_BLOCk_REG) {
        for(i=0; i<buffer.nRegisters; i++) {
            if(article.id < n_article[i].id) {
                //Desloca os artigos armazenados para manter a ordem crescente na nova inserção
                for(int j=buffer.nRegisters; j>i; j--) {
                    memcpy(&n_article[j],(char*)&n_article[i-1], sizeof(Article));
                }
                break;
            }
        }
        //Insere artigo
        memcpy(&n_article[i],(char*)&article, sizeof(Article));

        //Atualiza a quantidade de registros ocupados no bloco
        buffer.nRegisters++;

        //Volta o cursor para o início do bloco        
        f->seekp( - sizeof(Block) , ios::cur);

        //Escreve o bloco no arquivo de dados
        f->write((char*)&buffer,sizeof(Block));

        return true;
    }else {
        collisionCount++;
        return false;   
    }
}

Article buscaRegistroPorId(fstream *f,int id) {
	/*
	Caso não encontre, retorna os campos do registro, a quantidade
	de blocos lidos para encontrá-lo e a quantidade total de blocos do arquivo 
	de dados.
	*/

	Block buffer= consultaBucketPorId(f,id);
    Article *v_article = (Article*)&buffer.body;

    int begin= 0;
    int end = buffer.nRegisters - 1;

    while (begin <= end) {  //Percorre o corpo do bloco para achar o registro com o Id informado (busca binária)
        int i = (begin + end) / 2;  // Calcula o meio do sub-vetor
        if (v_article[i].id == id) {
            imprimirRegistroArt(v_article[i]);
            cout<< "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" <<
            "\nBlocos lidos: 1" <<       // 1 bucket = 1 bloco. Busca é feita no bloco que armazena o registro informado
            "\nTotal de Blocos alocados: "<<N_BUCKETS<<       //Total de blocos alocados no arquivo de dados
            "\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" <<endl;
            return v_article[i];
        }
        if (v_article[i].id < id) {  // Item está no sub-vetor à direita 
            begin = i + 1;
        } else {  // vector[i] > item. Item está no sub-vetor à esquerda
            end = i;
        }
    }

    cout << "\nO registro não foi encontrado.\n";
    return {0};
}


Block buscaBucketPorPosicao(fstream *f, int posicao) {
    Block buffer={0};
    //Busca a posição do bucket no arquivo de dados
    f->seekg (posicao*sizeof(Block),ios::beg);
    //Copia o bloco correspondente no buffer
    f->read((char*)&buffer,sizeof(Block));
    
    return buffer;
}


Article buscaBucketPorTitulo(fstream *f, int posicao, char title[TITLE_SIZE]){

    Block buffer = buscaBucketPorPosicao(f, posicao);
    Article *n_article = (Article*)&buffer.body;

    for(int i=0; i<buffer.nRegisters; i++) { //Busca sequencial no bloco pelo registro com o título informado
        if(strcmp(title, n_article[i].title) == 0){
            imprimirRegistroArt(n_article[i]);
            return n_article[i];
        }
    }

    cout << "O registro não foi encontrado.\n";
    return {0};
}