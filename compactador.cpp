#include <iostream>
#include <string>
#include <chrono>
#include "caracterHuffman.h"
#include "palavraHuffman.h"

using namespace std;

void mostrarMenu(){
    cout << "---------------------------------" << endl;
    cout << "Escolha uma opçao:" << endl;
    cout << "1. Compactar arquivo por caractere" << endl;
    cout << "2. Descompactar arquivo por caractere" << endl;
    cout << "3. Compactar arquivo por palavra" << endl;
    cout << "4. Descompactar arquivo por palavra" << endl;
    cout << "0. Sair" << endl;
}

int main(){
    int opcao;
    string inputFile, outputFile;

    do{
        mostrarMenu();
        cin >> opcao;

        cout <<"Digite nome do arquivo de entrada: ";
        cin >> inputFile;
        cout << "Digite nome do arq de saida: ";
        cin >> outputFile;

        // Contagem de tempo de execução da compressão/descompressão
        auto start = chrono::high_resolution_clock::now();

        switch(opcao){
            case 1:
                CharHuffman::compress(inputFile, outputFile);
                break;
            case 2:
                CharHuffman::decompress(inputFile, outputFile);
                break;
            case 3:
                WordHuffman::compress(inputFile, outputFile);
                break;
            case 4:
                WordHuffman::decompress(inputFile, outputFile);
                break;
            default:
                cout << "Opcao invalida!" <<endl;
                break;
        }

        //Mostra o tempo de execução da compressão/descompressão
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tempo de execucao: " << duration.count() << " ms" << endl;

    }while (opcao != 0);

    return 0;
}