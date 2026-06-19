#ifndef CARACTER_HUFFMAN_H
#define CARACTER_HUFFMAN_H

#include <string>
#include "NoHuffman.h"

using namespace std;

namespace CharHuffman {
    //Monta tabela de frequência
    void montarTabFreq(const string& inputFile, int freq[256]);

    //monta a árvore de huffman a partir da tabela de frequência
    NoHuffman* montarArvHuffman();

    //gera cod huffman a partir da arv
    void gerarCodHuffman(NoHuffman* raiz, string codAtual, string codigos[256]);

    //Executa a compressão por caractere
    void compress(const string& inputFile, const string& outputFile);

    //Executa a descompressão por caractere
    void decompress(const string& inputFile, const string& outputFile);
}

#endif