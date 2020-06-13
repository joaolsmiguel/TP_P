#include "includes.h"

int main() {
    initRandom();
    
    
    int tamanho_espaco;
    local* pEspaco = NULL;
    pessoa* pPessoas = NULL;

    //LEITURA DE DADOS
    pEspaco = ler_espaco(&tamanho_espaco);
    pPessoas = ler_pessoas();

    if (pEspaco != NULL && pPessoas != NULL){
        distribui_pessoas(pEspaco, pPessoas, tamanho_espaco);//distribucao random

        //SIMULACAO
        comeca_simulacao(pEspaco, pPessoas, tamanho_espaco);

        //LIBERTACAO MEMORIA
        free(pEspaco);
        while (pPessoas != NULL){
            pessoa* aux = p;
            p = p ->prox;
            free(aux);
        }
    }
    return (0);
}
