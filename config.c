#include "includes.h"

int verifica_existe_nome(pessoa* arrayPessoas, pessoa* novo){
    int existe = 0;
    while (arrayPessoas != NULL){
        if (arrayPessoas->id == novo->id){
            existe = 1;
        }
        arrayPessoas = arrayPessoas->prox;
    }
    return existe;
}

plocal ler_espaco(int* tamanho_espaco){
    FILE *f;
    struct sala *pArray, *aux = NULL;

    char nome[20], path[] = "./files/";

    printf("Digite o nome do ficheiro de Espaco: ");
    gets(nome);
    if (strstr(nome, ".bin") == NULL){
        strcat(nome, ".bin");
    }
    strcat(path, nome);
    
    f = fopen(path, "rb");

    if (f == NULL){
        printf("\nErro ao ler ficheiro\n");
        return NULL;
    }else{
        struct sala local;
        int i = 1;

        while (fread(&local, sizeof(struct sala), 1,f)){
            if (aux == NULL){
                aux = malloc(sizeof(struct sala));
            }else{
                aux = realloc(pArray, sizeof(struct sala) * i);
            }

            memcpy(&aux[i-1], &local, sizeof(local));
            
            if (aux != NULL){
                pArray = aux;
                i++;
            }
        }
        *tamanho_espaco = i - 1;
        fclose(f);

        int verifica_licacao = 1;
        for (int j = 0; j < *tamanho_espaco; j++){
            for (int k = 0; k < 3; k++){
                if (pArray[j].liga[k] != -1){
                    verifica_licacao = verifica_ligacao_direta(pArray, *tamanho_espaco, pArray[j].id, pArray[j].liga[k]);
                    if (verifica_licacao == 0){
                        return NULL;
                    }
                }
            }
        }
        return pArray;
    }
}

int verifica_ligacao_direta(local* arrayEspaco, int tam, int id1, int id2){
    int ver_1 = 0;
    int ver_2 = 0;

    for (int i = 0; i < tam; i++){
        if (arrayEspaco[i].id == id1 && ver_1 == 0){
            for (int k = 0; k < 3; k++){
                if (arrayEspaco[i].liga[k] == id2){
                    ver_1 = 1;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < tam; i++){
        if (arrayEspaco[i].id == id2 && ver_2 == 0){
            for (int k = 0; k < 3; k++){
                if (arrayEspaco[i].liga[k] == id1){
                    ver_2 = 1;
                    break;
                }
            }
        }
    }

    if (ver_1 == 1 && ver_2 == 1){
        return 1;
    }else{
        return 0;
    }
}

void distribui_pessoas(local* arrayEspaco, pessoa* arrayPessoas, int tamEspaco){
    int array_ids[tamEspaco];
    pessoa* aux = arrayPessoas;

    for (int i = 0; i < tamEspaco; i++){
        array_ids[i] = arrayEspaco[i].id;
    }

    while (arrayPessoas != NULL){
        if (arrayPessoas->na_sala == NULL){
            int n_rand = intUniformRnd(0, (tamEspaco - 1));

            if (sala_cheia(aux, &arrayEspaco[n_rand]) < arrayEspaco[n_rand].capacidade){
                arrayPessoas->na_sala = &arrayEspaco[n_rand];
            }
        }
        arrayPessoas = arrayPessoas->prox;
    }
}


int sala_cheia(pessoa* arrayPessoas, local* sala){
    int pessoas_na_sala = 0;

    while (arrayPessoas != NULL){
        if (arrayPessoas->na_sala != NULL){
            if (arrayPessoas->na_sala->id == sala->id){
                pessoas_na_sala++;
            }
        }
        arrayPessoas = arrayPessoas->prox;
    }
    return pessoas_na_sala;
}


int verifica_sala_existe(local* arrayEspaco, int id_sala, int tam){
    for (int i = 0; i < tam; i++){
        if (id_sala == arrayEspaco[i].id){
            return i;
        }
    }
    return 0;
}

ppessoas ler_pessoas(){
    FILE *f;
    int i = 0;
    char nome[20], path[] = "./files/";

    ppessoas novo, aux;
    ppessoas p = NULL;

    printf("Digite o nome do ficheiro de pessoas: ");
    gets(nome);
    if (strstr(nome, ".txt") == NULL){
        strcat(nome, ".txt");
    }

    strcat(path, nome);

    f = fopen(path, "r");
    if (f == NULL){
        printf("\nErro ao ler ficheiros\n");
        return NULL;
    }else{
        while (!feof(f)){
            ppessoas novo = malloc(sizeof(pessoa));
            if (novo == NULL){
                printf("erro a alocar memoria");
                free(novo);
            }

            //LEITURA
            fscanf(f, "%s %d %s", novo->id, &novo->idade, novo->estado);
            if (verifica_existe_nome(p, novo) == 0){
                if (novo->estado[0] == 'D'){
                    fscanf(f, "%d", &novo->infetado_ha_dias);
                }
                novo->na_sala = NULL;
                novo->prox = NULL;

                if (p == NULL){
                    p = novo;
                }else{
                    aux = p;
                    while (aux->prox != NULL){
                        aux = aux->prox;
                    }
                    aux->prox = novo;
                }
                i++;
            }
        }
        fclose(f);
        return p;
    }
}


int conta_pessoas(pessoa* arrayPessoas){
    int iter = 0;
    while (arrayPessoas != NULL){
        iter++;
        arrayPessoas = arrayPessoas->prox;
    }
    return iter;
}

int conta_estado(pessoa* arrayPessoas, char estado){
    int iter = 0;
    while (arrayPessoas != NULL){
        if (arrayPessoas->estado[0] == estado){
            iter++;
        }
        arrayPessoas = arrayPessoas->prox;
    }
    return iter;
}
