// Inclui as bibliotecas necessárias
#include "hash.hpp"
#include "prim.hpp"
#include "sec.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

    // Função para processar uma linha de entrada (registro) e retornar um objeto Article
    Article processaLinhaRegistro(string linha){
        Article dados;
        size_t pos = 0;
        string token;
        int coluna = 0;

        // Loop para processar cada campo do artigo, delimitados pelo por ";"
        while ((pos = linha.find("\";")) != std::string::npos) {
            token = linha.substr(1, pos-1);
            switch(coluna) {
                case 0:
                    dados.id= atoi(token.c_str());
                    break;
                case 1:
                    strncpy(dados.title, token.c_str(), T_TITLE);
                    dados.title[T_TITLE-1] = 0;
                    break;
                case 2:
                    dados.year = atoi(token.c_str());
                    break;
                case 3:
                    strncpy(dados.author, token.c_str(), T_AUTHOR);
                    dados.author[T_AUTHOR-1] = 0;
                    break;
                case 4:
                    dados.citations = atoi(token.c_str());
                    break;
                case 5:
                    strncpy(dados.update, token.c_str(), T_UPDATE);
                    dados.update[T_UPDATE-1] = 0;
                    break;
            }
            linha.erase(0, pos + string("\";").length());   
            coluna ++;
        }

        // Processa o último campo, snippet
        if(linha.length() > 0){
            token = linha.substr(0, linha.length()-1);

            if(strcmp(token.c_str(), "NULL") != 0){
                token = linha.substr(1, linha.length()-3);
            }
            strncpy(dados.snippet, token.c_str(), T_SNIPPET);
            dados.title[T_SNIPPET-1] = 0;
        }
        return dados;
    }

int main(int argc, char *argv[]){

    

    // Verifica se o nome do arquivo de entrada foi fornecido
    if(argc<2) {
        cout << "\nNecessario inserir o arquivo e sua extensao\n\nExemplo: upload <arquivo>.csv\n";
        return 1;
    }

    // Abre o arquivo de entrada
    ifstream arquivoEntrada(argv[1]);
    if (!arquivoEntrada){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // Cria o arquivo de dados, formato binário
    fstream arquivoDados(HASH_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary);
    /**/inicializaArquivoDeSaida(&arquivoDados);

    // Cria os arquivos de índices primário e secundário, formato binário
    fstream arquivoIndiceSec(SEC_INDEX_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary);
    fstream arquivoIndicePrim(PRIM_INDEX_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary);

    cout << "Carregando registros dos arquivos..."<<endl;

    // Processa cada registro do arquivo
    string linha;

    while (getline(arquivoEntrada, linha)){
        Article artigo = processaLinhaRegistro(linha);
        /**/insereArquivoHash(&arquivoDados, artigo);
    }

    cout << "Arquivo Hash criado, começando o arquivo de índice primário" << endl;
    /**/InsereArqIndicePrim(&arquivoDados, &arquivoIndicePrim);

    cout << "Arquivo de indice primario criado, começando o arquivo de de índice secundário" << endl;
    /**/InsereArqIndiceSec(&arquivoDados, &arquivoIndiceSec);

    cout << "Arquivo de indice secundario criado" << endl;

    cout << "Todos os arquivos foram criados, que comecem as buscas!!" << endl;

    arquivoDados.close();
    arquivoIndicePrim.close();
    arquivoIndiceSec.close();
    
}