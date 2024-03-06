#include <iostream>     
#include "hash/hash.hpp"

int main(int argc, char *argv[])
{
    if(argc<2) {
        cout << "Digite o ID que deseja buscar.\n\nEx: findrec <ID>\n";
        return 1;
    }

    fstream *hashFile = new fstream(DATA_FILE,fstream::in|ios::binary);

    if (hashFile->is_open()) {
        buscaRegistroPorId(hashFile,atoi(argv[1]));   
    }
    else {
        cout << "Erro ao abrir o arquivo.\n";
    }
    return 0;
}