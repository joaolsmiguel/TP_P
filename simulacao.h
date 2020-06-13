/*
Joao luis silva miguel
2018018852

*/
#include "includes.h"

#ifndef SIMULACAO_H
#define SIMULACAO_H

void infeta_na_sala(pessoa* arrayPessoa);//TAXA DE DISSEMINACAO
int prob_recuperacao(pessoa* arrayPessoas);//PROBABLIDADE DE RECUPERACAO
int max_dura_infecao(pessoa* arrayPessoas);//DURACAO MAXIUMA INFECAO  FEITA
void prob_imune(pessoa* arrayPessoas);//TAXA DE IMUNIDADE


//---------SIMULACAO-----------

void menu(int dia);

void comeca_simulacao(local* arrayEspaco, pessoa* arrayPessoas, int tam_espaco);
void avancar_1_iter();
void mostra_estatisticas(pessoa* arrayPessoas, local* arrayEspaco,int dia, int tam_espaco);//BUG
void transfer_pessoas(pessoa* arrayPessoas, local* arraryEspaco, int tam);
void transfer(pessoa* arrayPessoas, local* sala_origem, local* sala_destino, int n_pessos_transfer);
void termina_simulacao(pessoa* arrayPessoas);
//guarda para um ficheiro (report.txt) o relatorio "final"
//guarda num ficheiro txt com nome a escolha a populacao existente na ultima iteracao

void update_dias(pessoa* arrayPessoas);
void infeta_pessoa(pessoa* arrayPessoas);

void adiciona_doente(local* arrayEspaco, pessoa* arrayPessoas, int tam_espaco);

pessoa* volta_iter_atras(int dia, int* recua);//return ficheiro dia anterior que le
void cria_temp_fich(pessoa* arrayPessoas, int dia);//fazer - copia arrayPessoas para dentro de um array[3]
void apaga_backup_antigos(int dia);

#endif /* SIMULACAO_H */
