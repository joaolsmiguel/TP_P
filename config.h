#include "includes.h"

#ifndef CONFIG_H
#define CONFIG_H


typedef struct sala local, *plocal;
struct sala{
    int id;
    int capacidade;
    int liga[3];
};

typedef struct pessoas pessoa, *ppessoas;
struct pessoas{
    char id[50];
    int idade;
    char estado[1]; //S - saudavel, D - doente, I - imune
    int infetado_ha_dias;//default 0, se estiver doente tem o n dias desde a infecao
    pessoa* prox;
    local* na_sala;
};

plocal ler_espaco(int* tamanho_espaco);
ppessoas ler_pessoas();

int verifica_ligacao_direta(local* arrayEspaco, int tam, int id1, int id2);
int verifica_sala_existe(local* arrayEspaco, int id_sala, int tam);
//int verifica_erros_espaco(plocal arrayEspaco, int tam);//bugado

//'vazia '0' return conta_pessoas
int sala_cheia(pessoa* arrayPessoas, local* sala);

void distribui_pessoas(local* arrayEspaco, pessoa* arrayPessoas, int tamEspaco);

int conta_pessoas(pessoa* arrayPessoas);
int conta_estado(pessoa* arrayPessoas, char estado);

int verifica_existe_nome(pessoa* arrayPessoas, pessoa* novo);//1 se existe / 0 se nao existe

void guarda_no_fich();//guarda no fim para um ficheiro binario usandoa  estrutura do tipo local

#endif /* CONFIG_H */
