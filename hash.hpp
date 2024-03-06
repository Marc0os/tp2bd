#ifndef HASH_H
#define HASH_H

#include <fstream>
#include "header.hpp"

using namespace std;

void inicializaArquivoDeSaida(fstream *f);
bool insereArquivoHash(fstream *f, Article article);
Article buscaRegistroPorId(fstream *f,int id);
Article buscaBucketPorTitulo(fstream *f, int posicao, char title[TITLE_SIZE]);

#endif