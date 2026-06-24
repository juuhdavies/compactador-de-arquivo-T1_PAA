#ifndef CARACTER_HUFFMAN_H
#define CARACTER_HUFFMAN_H

#include <string>
#include "NoHuffman.h"

using namespace std;

namespace CharHuffman {
    //Monta tabela de frequência
    /**
     * @brief Constrói a tabela de frequências lendo o arquivo byte a byte.
     * Mapeia cada caractere (0-255) da tabela ASCII estendida ao seu número de ocorrências.
     * @param inputFile Caminho do arquivo original de entrada.
     * @param freq Vetor estático de 256 posições que armazenará as frequências.
     */
    void montarTabFreq(const string& inputFile, int freq[256]);

    //monta a árvore de huffman a partir da tabela de frequência
    /**
     * @brief Monta a Árvore Binária de Huffman Clássica a partir do vetor de frequências.
     * @param freq Vetor estático com as frequências mapeadas.
     * @return NoHuffman* Retorna o ponteiro para o nó raiz da árvore consolidada.
     */
    NoHuffman* montarArvHuffman();

    //gera cod huffman a partir da arv
    /**
     * @brief Percorre a árvore recursivamente (Pré-ordem) para codificar os caminhos binários.
     * @param raiz Ponteiro para o nó atual da árvore.
     * @param codAtual String temporária contendo a sequência de bits calculada até o momento.
     * @param codigos Vetor de strings indexado que armazenará o código de bits final de cada caractere.
     */
    void gerarCodHuffman(NoHuffman* raiz, string codAtual, string codigos[256]);

    //libera memoria da árvore de huffman
    /**
     * @brief Desaloca a árvore de Huffman recursivamente.
     */
    void liberarArv(NoHuffman* raiz);
    
    //Executa a compressão por caractere
        /**
     * @brief compressão baseada em caracteres.
     * Grava o cabeçalho estruturado e empacota os dados em nível de bits.
     */
    void compress(const string& inputFile, const string& outputFile);

    //Executa a descompressão por caractere
        /**
     * @brief descompressão por palavras.
     * Reconstrói a árvore de Huffman usando o cabeçalho e regenera o texto original.
     */
    void decompress(const string& inputFile, const string& outputFile);
}

#endif