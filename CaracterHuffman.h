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

    void gerarCodHuffman(NoHuffman* raiz, string codAtual, string codigos[256]) {
        if (raiz == nullptr) return;

        //Se for um nó folha, armazena o código correspondente ao caractere
        if (raiz->esq == nullptr && raiz->dir == nullptr) {
            codigos[static_cast<unsigned char>(raiz->caractere)] = codAtual;
        }

        //Percorre a subárvore esquerda adicionando '0' ao código
        gerarCodHuffman(raiz->esq, codAtual + "0", codigos);
        //Percorre a subárvore direita adicionando '1' ao código
        gerarCodHuffman(raiz->dir, codArual + "1", codigos);
    }

    //Executa a compressão por caractere
    void compress(const string& inputFile, const string& outputFile);

    //Executa a descompressão por caractere
    void decompress(const string& inputFile, const string& outputFile);
}

#endif