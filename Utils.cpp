#include "Utils.h"

using namespace std;

namespace Utils {
    // Implementação das funções declaradas em Utils.h

    //Ler os bits do arquivo de entrada
    /**
     * @brief Realiza a leitura de bytes de um arquivo binário e converte-os em uma sequência de bits representada como uma string.
     * @param inputFile Fluxo de leitura do arquivo binário compactado.
     * @param bits String de referência que armazenará a sequência acumulada de '0's e '1's.
     */
    void lerBitsArq(ifstream& inputFile, string& bits) {
        char byte;
        while (inputFile.get(byte)) {
            // Lê cada um dos 8 bits do byte lido
            for (int i = 7; i >= 0; i--) {
                //verificar se o i-ésimo bit é 1 ou 0
                if ((byte >> i) & 1) {
                    bits += '1';
                } else {
                    bits += '0';
                }
            }
        }
    }

    //Escrever os bits do arquivo de saída
    /**
     * @brief Escreve uma sequência de bits em um arquivo binário, utilizando um buffer para acumular os bits antes de escrever.
     * @param outputFile Fluxo de escrita binária no disco.
     * @param bits Cadeia de caracteres contendo a codificação de Huffman a ser injetada.
     * @param buffer Byte de acumulação temporária controlado por referência.
     * @param bitsPreenchidos Contador do estado atual de preenchimento do buffer (0 a 8).
     */
    void escreverBitsArq(ofstream& outputFile, const string& bits, unsigned char& buffer, int& bitsPreenchidos) {
        for (char bit : bits) {
            buffer <<= 1; // Desloca o buffer para a esquerda
            if (bit == '1') {
                buffer |= 1; // Define o bit mais à direita se for '1'
            }
            bitsPreenchidos++;

            if (bitsPreenchidos == 8) { // Se o buffer estiver cheio, escreve no arquivo
                outputFile.put(buffer);
                buffer = 0;
                bitsPreenchidos = 0;
            }
        }
    }

    /**
     * @brief Descarrega os bits restantes no buffer para o arquivo de saída, preenchendo com zeros à direita se necessário.
     * @param outputFile Fluxo de escrita binária no disco.
     * @param buffer Byte de acumulação temporária controlado por referência.
     * @param bitsPreenchidos Contador do estado atual de preenchimento do buffer
     */
    void flushBitsArq(ofstream& outputFile, unsigned char& buffer, int& bitsPreenchidos) {
        if (bitsPreenchidos > 0) {
            buffer <<= (8 - bitsPreenchidos);
            outputFile.put(static_cast<char>(buffer));
            buffer = 0;
            bitsPreenchidos = 0;
        }
    }

    //Tokenizar o arquivo de entrada, para compressão por palavra
    /**
     * @brief Quebra o conteúdo de um arquivo de texto em tokens (palavras), preservando pontuação e acentuação.
     * @param inputFile Nome do arquivo de entrada a ser tokenizado.
     * @return Um vetor de strings contendo os tokens extraídos do arquivo.
     */
    vector<string> tokenizar(const string& inputFile) {
        vector<string> tokens;
        ifstream file(inputFile);
        string word;

        while (file >> word) {
            tokens.push_back(word);
        }

        return tokens;
    }


}