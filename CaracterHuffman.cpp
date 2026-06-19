#include "CaracterHuffman.h"
#include "NoHuffman.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>



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

    //Compressão char por char utilizando huffman. Monta tabela de frequência, constrói arv de huffman e escreve bits codificados no arq de saída
    void compress(const string& inputFile, const string& outputFile) {
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

        input.close();
        output.close();
        cout << "Compactacao concluida!" << endl;

    }

}