#ifndef PALAVRA_HUFFMAN_H
#define PALAVRA_HUFFMAN_H

#include <string>
#include <unordered_map>
#include "NoHuffman.h"

using namespace std;

namespace WordHuffman {
    //Executa a compressão por palavra
    /**
     * @brief compressão baseada em palavras.
     * Grava o cabeçalho estruturado e empacota os dados em nível de bits.
     */
    void compress(const string& inputFile, const string& outputFile);

    //monta a árvore de huffman a partir da tabela de frequência para palavras
    /**
     * @brief Monta a Árvore Binária de Huffman baseando-se no algoritmo guloso clássico.
     * @param freq Tabela hash contendo a distribuição de frequências das palavras.
     * @return NoHuffmanPalavra* Retorna o ponteiro para o nó raiz da árvore gerada.
     */
    NoHuffmanPalavra* montarArvHuffman(const unordered_map<string, int>& freq);

    //libera memoria da árvore de huffman
    /**
     * @brief Desaloca a árvore de Huffman recursivamente.
     */
    void liberarArv(NoHuffmanPalavra* raiz);

    //gera cod Huffman para cada palavra da árvore
    /**
     * @brief Percorre a árvore binária recursivamente para mapear os caminhos de bits.
     * @param raiz Ponteiro para o nó atual.
     * @param codAtual String binária acumulada no caminho até o nó atual.
     * @param codigos Mapa de saída que associará cada palavra ao seu código binário final.
     */
    void gerarCodHuffman(NoHuffmanPalavra* raiz, string codAtual, unordered_map<string, string>& codigos);

    //Monta tabela de frequência das palavras
    /**
     * @brief Constrói a tabela de frequências das palavras a partir do arquivo de texto.
     * @param inputFile Caminho do arquivo de entrada.
     * @param freq Mapa de dispersão onde será armazenada a contagem (Palavra -> Frequência).
     */
    void montarTabFreq(const string& inputFile, unordered_map<string, int>& freq);

    //Executa a descompressão por palavra
    /**
     * @brief descompressão por palavras.
     * Reconstrói a árvore de Huffman usando o cabeçalho e regenera o texto original.
     */
    void decompress(const string& inputFile, const string& outputFile);
}

#endif