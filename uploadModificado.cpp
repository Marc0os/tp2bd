// Inclui as bibliotecas necessárias
#include "hash.hpp"
#include "../B+Tree/IndicePrimario.hpp"
#include "../B+Tree/IndiceSecundario.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

// Classe para gerenciar os dados do artigo
class GerenciadorDados {
public:
    // Função para remover arquivos antigos
    void removerArquivosAntigos() {
        remove(HASH_FILE_NAME);
        remove(PRIM_INDEX_FILE_NAME);
        remove(SEC_INDEX_FILE_NAME);
    }

    // Função para copiar uma string
    void copiarString(char* destino, string origem, int tamanho) {
        strncpy(destino, origem.c_str(), tamanho);
        destino[tamanho-1] = 0;
    }

    // Função para processar uma linha de entrada e retornar um objeto Article
    Article processarLinhaEntrada(std::string linha){
        Article dados;
        std::string delimitador = "\";";
        size_t pos = 0;
        std::string token;
        int coluna = 0;

        // Loop para processar cada campo do artigo
        while ((pos = linha.find(delimitador)) != std::string::npos) {
            token = linha.substr(1, pos-1);
            switch(coluna) {
                case 0:
                    dados.id= atoi(token.c_str());
                    break;
                case 1:
                    copiarString(dados.title, token.c_str(), T_TITLE);
                    break;
                case 2:
                    dados.year = atoi(token.c_str());
                    break;
                case 3:
                    copiarString(dados.author, token.c_str(), T_AUTHOR);
                    break;
                case 4:
                    dados.citations = atoi(token.c_str());
                    break;
                case 5:
                    copiarString(dados.update, token.c_str(), T_UPDATE);
                    break;
            }
            linha.erase(0, pos + delimitador.length());   
            coluna ++;
        }

        // Processa o último campo
        if(linha.length() > 0){
            token = linha.substr(0, linha.length()-1);

            if(strcmp(token.c_str(), "NULL") != 0){
                token = linha.substr(1, linha.length()-3);
            }

            copiarString(dados.snippet, token.c_str(), T_SNIPPET);
        }
        return dados;
    }
};

int main(int argc, char *argv[]){
    // Verifica se o nome do arquivo de entrada foi fornecido
    if(argc<2) {
        cout << "Por favor, forneça o nome do arquivo de entrada.\nExemplo: upload <arquivo>\n";
        return 1;
    }

    // Abre o arquivo de entrada
    ifstream arquivoEntrada(argv[1]);
    if (!arquivoEntrada){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // Cria um objeto da classe GerenciadorDados
    GerenciadorDados gerenciador;
    gerenciador.removerArquivosAntigos();

    // Cria o arquivo de dados
    fstream arquivoDados(HASH_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary);
    inicializaArquivoDeSaida(&arquivoDados);

    // Cria os arquivos de índice
    fstream arquivoIndiceSec(SEC_INDEX_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary);
    fstream arquivoIndicePrim(PRIM_INDEX_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary);

    cout << "Iniciando a carga dos registros no arquivo de dados. Isso pode levar algum tempo..."<<endl;
    cout << "Que tal aproveitar para tomar um café?..."<<endl;

    // Processa cada linha do arquivo de entrada
    string linha;
    while (getline(arquivoEntrada, linha)){
        Article artigo = gerenciador.processarLinhaEntrada(linha);
        insereArquivoHash(&arquivoDados, artigo);
    }

    cout << "Arquivo Hash criado, começando o arquivo de índice primário" << endl;
    InsereArqIndicePrim(&arquivoDados, &arquivoIndicePrim);

    cout << "Arquivo de indice primario criado, começando o arquivo de de índice secundário" << endl;
    InsereArqIndiceSec(&arquivoDados, &arquivoIndiceSec);

    cout << "Arquivo de indice secundario criado" << endl;

    cout << "Todos os arquivos foram criados, que comecem as buscas!!" << endl;
    arquivoDados.close();
    arquivoIndicePrim.close();
    arquivoIndiceSec.close();
    
}
