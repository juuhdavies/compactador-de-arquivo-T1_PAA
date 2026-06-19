#include "CaracterHuffman.h"
#include "NoHuffman.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <chrono>



using namespace std;

namespace CharHuffman{
    //implementação das funções de caracterHuffman.h

    void montarTabFreq(const string& inputFile, int freq[256]) {
        ifstream file(inputFile, ios::binary);
        char byte;

        while (file.get(byte)) {
            freq[static_cast<unsigned char>(byte)]++;
        }
    }

    NoHuffman* montarArvHuffman(const int freq[256]) {

        //Criar uma fila de prioridade para construir a árvore de Huffman
        priority_queue<NoHuffman*, vector<NoHuffman*>, compararNos> fila;
        for (int i = 0; i < 256; ++i) {
            if (freq[i] > 0) {
                fila.push(new NoHuffman(static_cast<char>(i), freq[i]));
            }
        }
        //se arq está vazio, retorna null
        if(fila.empty()) return nullptr; 

        //Construir a árvore de Huffman
        while (fila.size() > 1) {
            NoHuffman* esq = fila.top(); fila.pop(); //pega o nó com menor frequência
            NoHuffman* dir = fila.top(); fila.pop(); //pega o nó com segunda menor frequência
            NoHuffman* novoNo = new NoHuffman('\0', esq->freq + dir->freq); //cria um novo nó com frequência igual à soma das frequências dos dois nós
            novoNo->esq = esq; //o nó com menor frequência fica à esquerda
            novoNo->dir = dir; //o nó com segunda menor frequência fica à direita
            fila.push(novoNo); //adiciona o novo nó de volta à fila
        }

        return fila.top(); //o nó restante é a raiz da árvore de Huffman

    }

    void gerarCodHuffman(NoHuffman* raiz, string codAtual, string codigos[256]) {
        if (raiz == nullptr) return;

        //Se for um nó folha, armazena o código correspondente ao caractere
        if (raiz->esq == nullptr && raiz->dir == nullptr) {
            codigos[static_cast<unsigned char>(raiz->caractere)] = codAtual;
        }

        //Percorre a subárvore esquerda adicionando '0' ao código
        gerarCodHuffman(raiz->esq, codAtual + "0", codigos);
        //Percorre a subárvore direita adicionando '1' ao código
        gerarCodHuffman(raiz->dir, codAtual + "1", codigos);

    }

    void liberarArv(NoHuffman* raiz) {
        if (raiz == nullptr) return;
        liberarArv(raiz->esq);
        liberarArv(raiz->dir);
        delete raiz;
    }

    //Compressão char por char utilizando huffman. Monta tabela de frequência, constrói arv de huffman e escreve bits codificados no arq de saída
    void compress(const string& inputFile, const string& outputFile) {
        // Contagem de tempo de execução da compressão/descompressão
        auto start = chrono::high_resolution_clock::now();

        int freq[256] = {0};
        montarTabFreq(inputFile, freq);

        NoHuffman* raiz = montarArvHuffman(freq);
        if (raiz == nullptr) {
            cerr << "Erro: O arquivo de entrada esta vazio." << endl;
            return;
        }

        string codigos[256];
        gerarCodHuffman(raiz, "", codigos);

        ifstream input(inputFile, ios::binary); //abre o arquivo de entrada em modo binário
        ofstream output(outputFile, ios::binary); //abre o arquivo de saída em modo binário

        output.write(reinterpret_cast<char*>(freq), sizeof(freq)); //escreve a tabela de frequência no início do arquivo de saída

        char byte; //armazenar byte lido no arq de entrada
        unsigned char byteIndex; //indice do byte lido

        unsigned char buffer = 0;
        int bitspreenchidos = 0;
        
        while(input.get(byte)) {
            byteIndex = static_cast<unsigned char>(byte);
            string cod = codigos[byteIndex];
            Utils::escreverBitsArq(output, cod, buffer, bitspreenchidos);
        }

        Utils::flushBitsArq(output, buffer, bitspreenchidos); //escreve os bits restantes no buffer para o arquivo de saída

        liberarArv(raiz); //libera memória da árvore de Huffman

        input.close();
        output.close();
        cout << "Compactacao concluida!" << endl;
        
        //Mostra o tempo de execução da compressão/descompressão
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tempo de execucao: " << duration.count() << " ms" << endl;

    }


    void decompress(const string& inputFile, const string& outputFile) {
        // Contagem de tempo de execução da compressão/descompressão
        auto start = chrono::high_resolution_clock::now();

        ifstream input(inputFile, ios::binary);
        ofstream output(outputFile, ios::binary);

        int freq[256];
        input.read(reinterpret_cast<char*>(freq), sizeof(freq)); //lê a tabela de frequência do início do arquivo de entrada

        long long totalCaracteres = 0;
        for (int i = 0; i < 256; ++i) {
            totalCaracteres += freq[i];
        }

        NoHuffman* raiz = montarArvHuffman(freq);
        if (raiz == nullptr) {
            cerr << "Erro: O arquivo de entrada esta vazio ou corrompido." << endl;
            return;
        }

        string bits;
        Utils::lerBitsArq(input, bits); //lê os bits codificados do arquivo de entrada
        long long totalDecodificados = 0; //contador para verificar se todos os caracteres foram decodificados

        NoHuffman* atual = raiz;
        for (char bit : bits) {
            if (bit == '0') {
                atual = atual->esq; //vai para a subárvore esquerda
            } else {
                atual = atual->dir; //vai para a subárvore direita
            }

            //Se chegar em um nó folha, escreve o caractere correspondente no arquivo de saída
            if (atual->esq == nullptr && atual->dir == nullptr) {
                output.put(atual->caractere);
                atual = raiz; //volta para a raiz da árvore
                totalDecodificados++;

                if (totalDecodificados == totalCaracteres) {
                    break; //todos os caracteres foram decodificados
                }
            }
        }



        //liberar mem arvore
        liberarArv(raiz);

        input.close();
        output.close();
        cout << "Descompactacao concluida!" << endl;

        //Mostra o tempo de execução da compressão/descompressão
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tempo de execucao: " << duration.count() << " ms" << endl;
    }
}