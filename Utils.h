#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace Utils {
    //escreve sequência de bits no arquivo de saída
    /**
     * @brief Escreve uma sequência de bits em um arquivo binário, utilizando um buffer para acumular os bits antes de escrever.
     * @param outputFile Fluxo de escrita binária no disco.
     * @param bits Cadeia de caracteres contendo a codificação de Huffman a ser injetada.
     * @param buffer Byte de acumulação temporária controlado por referência.
     * @param bitsPreenchidos Contador do estado atual de preenchimento do buffer (0 a 8).
     */
    void escreverBitsArq(ofstream& outputFile, const string& bits, unsigned char& buffer, int& bitsPreenchidos);

    //lê sequência de bits do arquivo de entrada
    /**
     * @brief Realiza a leitura de bytes de um arquivo binário e converte-os em uma sequência de bits representada como uma string.
     * @param inputFile Fluxo de leitura do arquivo binário compactado.
     * @param bits String de referência que armazenará a sequência acumulada de '0's e '1's.
     */
    void lerBitsArq(ifstream& inputFile, string& bits);

    //descarregamento dos bits restantes no buffer para o arquivo de saída
    /**
     * @brief Descarrega os bits restantes no buffer para o arquivo de saída, preenchendo com zeros à direita se necessário.
     * @param outputFile Fluxo de escrita binária no disco.
     * @param buffer Byte de acumulação temporária controlado por referência.
     * @param bitsPreenchidos Contador do estado atual de preenchimento do buffer
     */
    void flushBitsArq(ofstream& outputFile, unsigned char& buffer, int& bitsPreenchidos);

    //quebra palavras em tokens, manter pontuação e acentos
    /**
     * @brief Quebra o conteúdo de um arquivo de texto em tokens (palavras), preservando pontuação e acentuação.
     * @param inputFile Nome do arquivo de entrada a ser tokenizado.
     * @return Um vetor de strings contendo os tokens extraídos do arquivo.
     */
    vector<string> tokenizar(const string& inputFile);
}

#endif