#include "simulacao.h"

int max_dura_infecao(pessoa* arrayPessoas){
    while (arrayPessoas != NULL){
        int max_dias = (5 + floor(arrayPessoas->idade / 10));
        if (arrayPessoas->infetado_ha_dias == max_dias){
            prob_imune(arrayPessoas);
        }
        arrayPessoas = arrayPessoas->prox;
    }
}

void prob_imune(pessoa* arrayPessoas){
    if (probEvento(0.2) == 1){
        arrayPessoas->estado[0] = 'I';
    }else{
        arrayPessoas->estado[0] = 'S';
    }
}

void infeta_na_sala(pessoa* arrayPessoa){
    pessoa* inicio = arrayPessoa;
    pessoa* aux = arrayPessoa;

    while (arrayPessoa != NULL){
        if (arrayPessoa->estado[0] == 'D'){
            int n_pessoas_sala = sala_cheia(aux, arrayPessoa->na_sala);
            
            if (n_pessoas_sala > 1){
                int infeta_n_pessoas = floor(n_pessoas_sala * 0.05);//0.05
                if (infeta_n_pessoas >= 1){
                    for (int i = 0; i < infeta_n_pessoas;){
                        if (i >= n_pessoas_sala){ break; }
                        int rand_pessoa = intUniformRnd(1, n_pessoas_sala);

                        for (int j = 0; j < rand_pessoa;){
                            while (aux != NULL){
                                if (aux->na_sala == arrayPessoa->na_sala){
                                    j++;
                                    if (rand_pessoa == j){
                                        infeta_pessoa(aux);
                                        i++;
                                        break;
                                    }
                                }
                                aux = aux->prox;
                            }
                            break;
                        }
                        aux = inicio;
                    }
                }
            }
        }
        arrayPessoa = arrayPessoa->prox;
    }
}

void infeta_pessoa(pessoa* arrayPessoas){
    if (arrayPessoas->estado[0] != 'D'){
        arrayPessoas->estado[0] = 'D';
        arrayPessoas->infetado_ha_dias = 0;
    }
}

int prob_recuperacao(pessoa* arrayPessoas){
    while (arrayPessoas != NULL){
        if (arrayPessoas->estado[0] != 'D'){
            float prob_recuperacao = (1 / (double)arrayPessoas->idade);
            printf("%f",prob_recuperacao);
            if (probEvento(prob_recuperacao) == 1){
                prob_imune(arrayPessoas);
            }
        }
        arrayPessoas = arrayPessoas->prox;
    }
}

void update_dias(pessoa* arrayPessoas){
    max_dura_infecao(arrayPessoas);
    prob_recuperacao(arrayPessoas);
    infeta_na_sala(arrayPessoas);

    while (arrayPessoas->prox != NULL){
        if (arrayPessoas->estado[0] == 'D' || arrayPessoas->estado[0] == 'd'){
            arrayPessoas->infetado_ha_dias++;
        }
        arrayPessoas = arrayPessoas->prox;
    }
}

void menu(int dia){
    printf("DIA: %d\n", dia);
    printf("1 - Avancar 1 iteracao\n");
    printf("2 - Apresentar estatistica\n");
    printf("3 - Adicionar Doente\n");
    printf("4 - Transferir Pessoas\n");
    printf("5 - Recuar na simulacao\n");
    printf("6 - Terminar simulacao\n");
    printf("\nIndique a funcao que deseja iniciar: ");
}

void adiciona_doente(local* arrayEspaco, pessoa* arrayPessoas, int tam_espaco){
    pessoa* novo = malloc(sizeof(pessoa));
    pessoa* aux = arrayPessoas;

    fflush(stdin);
    printf("\nInsira o nome do Doente: ");
    gets(novo->id);

    printf("\nInsira a idade do doente: ");
    scanf("%d", &novo->idade);

    int id_sala;
    printf("\nInsira em que sala deseja que o doente seja Inserido: ");
    scanf("%d", &id_sala);

//bug - nao tem bug mas pode ser aperfeicoada e ser exportada para funcao separda.
    for (int i = 0; i < tam_espaco; i++){
        if (id_sala == arrayEspaco[i].id){
            if (arrayEspaco[i].capacidade > sala_cheia(arrayPessoas, &arrayEspaco[i])){
                novo->na_sala = &arrayEspaco[i];
            }
            break;
        }

        if (i == (tam_espaco - 1)){
            printf("\n Nao foi encontrado a sala indicada!\n");
            return;
        }
    }

    novo->estado[0] = 'D';
    novo->infetado_ha_dias = 0;
    novo->prox = NULL;

    while (aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novo;
}

void mostra_estatisticas(pessoa* arrayPessoas, local* arrayEspaco, int dia, int tam_espaco){
    system("cls");
    pessoa* aux = arrayPessoas;
    printf("\nEstatistica do dia %i\n", dia);

    int n_pessoas = conta_pessoas(arrayPessoas);

    //taxa de saudaveis
    printf("Valores em percentagem\n");
    printf("Taxa de saudaveis: %.1f \n", (double)n_pessoas / (double)conta_estado(arrayPessoas, 'S'));
    printf("Taxa de Infetadas: %.1f \n", (double)n_pessoas / (double)conta_estado(arrayPessoas, 'D'));
    printf("Taxa de Imunes: %.1f \n", (double)n_pessoas / (double)conta_estado(arrayPessoas, 'I'));

    printf("\nDistribuicao pessoas pelas salas: \n\n");
    for (int i = 0; i < tam_espaco; i++)
    {
        aux = arrayPessoas;
        while (aux != NULL){
            if (arrayEspaco[i].id == aux->na_sala->id){
                printf("sala %i: ", arrayEspaco[i].id);
                printf("%s", aux->id);
                if (aux->estado[0] == 'D'){
                    printf("\tInfetado ha: %i", aux->infetado_ha_dias);
                }
                printf("\n");
            }
            aux = aux->prox;
        }
    }
    fflush(stdin);
    printf("\nPrima 'enter' para continuar...");
    getchar();
}

void transfer(pessoa* arrayPessoas, local* sala_origem, local* sala_destino, int n_pessoas_transfer){
    pessoa* aux = NULL;
    int n_pessoas_sala_origem = sala_cheia(arrayPessoas, sala_origem);
    
    if (n_pessoas_transfer > n_pessoas_sala_origem){
        printf("\nErro nao e possivel transferir pessoas que nao existem\n");//corrigir
    }else{
        for (int i  = 0; i < n_pessoas_transfer; i++){
            int rand = intUniformRnd(1, sala_cheia(arrayPessoas, sala_origem));
            int j = 1;
            aux = arrayPessoas;

            while (aux != NULL){
                if (aux->na_sala == sala_origem){
                    if (j == rand){
                        aux->na_sala = sala_destino;
                    }
                    j++;
                }
                aux = aux->prox;
            }
        }
    }
}

void transfer_pessoas(pessoa* arrayPessoas, local* arraryEspaco, int tam){
    int n_pessoas = 0;
    int local_1 = 0;
    int local_2 = 0;

    //escolhe sala
    printf("\nPretende mover pessoas de que sala: ");
    scanf("%i", &local_1);
    int pos_sala_1 = verifica_sala_existe(arraryEspaco, local_1, tam);
    if (local_1 > 0 && pos_sala_1 >= 0){

        //n_pessoas pretende transferir
        printf("\nquantas pessoas pretende transferir: ");
        scanf("%i", &n_pessoas);
        int n_pessoas_na_sala_1 = sala_cheia(arrayPessoas, &arraryEspaco[pos_sala_1]);
        if (n_pessoas > 0 && n_pessoas_na_sala_1 != 0){

            //para que sala os move
            printf("\nPara que sala os pretende mover: ");
            scanf("%i", &local_2);
            int pos_sala_2 = verifica_sala_existe(arraryEspaco, local_2, tam);
            if (pos_sala_2 >= 0){
                if (sala_cheia(arrayPessoas, &arraryEspaco[pos_sala_2]) < (arraryEspaco[pos_sala_2].capacidade - n_pessoas)){
                    //trasfere as pessoas
                    if (verifica_ligacao_direta(arraryEspaco, tam, arraryEspaco[pos_sala_1].id, arraryEspaco[pos_sala_2].id) == 1){
                        transfer(arrayPessoas, &arraryEspaco[pos_sala_1], &arraryEspaco[pos_sala_2], n_pessoas);
                    }else{
                        printf("\nNao é possivel mover as pessoas pois as salas nao tem ligacoes diretas\n");
                        //fflush(stdin);
                        printf("\nPrima 'enter' para continuar...");
                        getchar();
                    }
                }
            }
        }
    }else{
        printf("\nO id de sala nao existe!\n");
    }
}

void termina_simulacao(pessoa* arrayPessoas){
    pessoa* aux = NULL;
    
    //relatorio
    FILE* f;
    f = fopen("./files/report.txt", "w");

    if (f == NULL){
        printf("\nErro ao abrir ficheiro para escrita\n");
    }else{
        fprintf(f, "Relatorio simulacao:\n");
        aux = arrayPessoas;
        int n_pessoas = conta_pessoas(arrayPessoas);

        fprintf(f, "\nNumero de pessoas na simulaco: %i\n\n", n_pessoas);

        fprintf(f, "Valores em percentagem:\n");
        fprintf(f, "Taxa de saudaveis: %.1f \n", (double)n_pessoas / (double)conta_estado(arrayPessoas, 'S'));
        fprintf(f, "Taxa de Infetadas: %.1f \n", (double)n_pessoas / (double)conta_estado(arrayPessoas, 'D'));
        fprintf(f, "Taxa de Imunes: %.1f \n\n", (double)n_pessoas / (double)conta_estado(arrayPessoas, 'I'));

        while (aux != NULL){
            fprintf(f, "Nome: %s\t\t Idade: %i\t Sala: %i \tEstado: %c", aux->id, aux->idade ,aux->na_sala->id, aux->estado[0]);
            if (aux->estado[0] == 'D'){
                fprintf(f, "\t dias infecao: %d", aux->infetado_ha_dias);
            }
            fprintf(f, "\n");
            aux = aux->prox;
        }
    }
    fclose(f);

    //populacao final
    char nome[20], path[] = "./files/";
    printf("\nNome para guardar o ficheiro: ");
    fflush(stdin);
    gets(nome);

    if (strstr(nome, ".txt") == NULL){
        strcat(nome, ".txt");
        printf("%s\n", nome);
    }

    strcat(path, nome);
    aux = arrayPessoas;

    f = fopen(path, "w");

    if (f == NULL){
        printf("\nErro ao abrir ficheiro para escrita\n");
    }else{
        fprintf(f, "Dados da poulacao na ultima iteracao:\n\n");
        while (aux != NULL){
            fprintf(f, "Nome: %s\t\t Idade: %i\t Sala: %i\n", aux->id, aux->idade ,aux->na_sala->id);
            aux = aux->prox;
        }
    }
    fclose(f);
}

void apaga_backup_antigos(int dia){
    char path[25];

    snprintf(path, sizeof(path), "./temp_files/temp_dia_%d.bin", dia - 3);
    remove(path);
}

void cria_temp_fich(pessoa* arrayPessoas, int dia){
    char path[25];
    int n_pessoas = conta_pessoas(arrayPessoas);

    snprintf(path, sizeof(path), "./temp_files/temp_dia_%d.bin", dia);
    FILE* f;
    f = fopen(path, "a+b");
    if (f == NULL){
        return;
    }else{
        fwrite(&n_pessoas, sizeof(int), 1, f);
        while (arrayPessoas != NULL){
            fwrite(arrayPessoas, sizeof(pessoa), 1, f);
            fwrite(arrayPessoas->na_sala, sizeof(local), 1, f);
            arrayPessoas = arrayPessoas->prox;
        }
        fclose(f);
        if (dia > 3){
            apaga_backup_antigos(dia);
        }
    }
}

pessoa* volta_iter_atras(int dia, int* recua){//FAZER
    fflush(stdin);
    printf("Quantas iteracoes pretende recuar: ");
    scanf("%i", recua);
    if (*recua >= 1 && *recua <= 3){
       char path[25];
       snprintf(path, sizeof(path), "./temp_files/temp_dia_%d.bin", dia-*recua);
       
       FILE* f;
       f = fopen(path, "rb");
       if (f == NULL){
           printf("Ocurreu um erro!");
           return NULL;
       }else{
            ppessoas novo, aux;
            ppessoas p = NULL;
            int n_pessoas;//fim do ficheiro / nr de pessoas no ficheiro

            fread(&n_pessoas, sizeof(int), 1, f);

            for (int i = 0; i < n_pessoas; i++){
                novo = malloc(sizeof(pessoa));

                fread(novo, sizeof(struct pessoas), 1, f);
                fread(novo->na_sala, sizeof(local), 1, f);

                novo->prox = NULL;
                if (p == NULL){
                    p = malloc(sizeof(pessoa));
                    p = novo;
                }else{
                    aux = p;
                    while (aux->prox != NULL){
                        aux = aux->prox;
                    }
                    aux->prox = novo;
                }
            }
            fclose(f);
            return p;
        }
    }else{
        printf("Valor invalido!");
        return NULL;
    }
}

void comeca_simulacao(local* arrayEspaco, pessoa* arrayPessoas, int tam_espaco){
    int op, dia = 1;
    while (op != 6){
        system("cls");
        menu(dia);
        scanf("%i", &op);
        if (op == 1){
            update_dias(arrayPessoas);
            cria_temp_fich(arrayPessoas, dia);//FAZER
            dia++;
        }else if (op == 2){
            mostra_estatisticas(arrayPessoas, arrayEspaco, dia, tam_espaco);
        }else if (op == 3){
            adiciona_doente(arrayEspaco, arrayPessoas, tam_espaco);//mudar
        }else if (op == 4){
            transfer_pessoas(arrayPessoas, arrayEspaco, tam_espaco);
        }else if(op == 5){
            int recua;//utilizacao com atoi
            pessoa* aux = volta_iter_atras(dia, &recua);
            if (aux != NULL){
                arrayPessoas = aux;
                dia = dia - recua;
            }
        }else if (op == 6){
            termina_simulacao(arrayPessoas);
            {
                DIR *dir;
                struct dirent *ent;
                char path[25];
                if ((dir = opendir ("./temp_files/")) != NULL) {
                    while ((ent = readdir (dir)) != NULL) {
                        if (strstr(ent->d_name, "temp_dia") != NULL){
                            snprintf(path, sizeof(path), "./temp_files/%s", ent->d_name);
                            remove(path);
                        }
                    }
                    closedir (dir);
                }
            }
        }else{
            printf("...Opcao invalida...");
        }
    }
}
