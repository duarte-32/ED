
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "donos.h"

Dono *donos = NULL;

void CarregarDonos(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    Dono *nova, *ultima = NULL;
        char linha[200];

        while (fgets(linha, sizeof(linha), f)) {

            // Remove o \n no final se existir
            linha[strcspn(linha, "\n")] = 0;

            nova = (Dono*)malloc(sizeof(Dono));

            // Lê o nome até detetar num numero(que corresponde ao código postal)
            // Isto evita a não leitura do código postal.
            if (sscanf(linha, "%d %99[^0-9] %19s",
                      &nova->numContribuinte,
                      nova->nome,
                      nova->codPostal) != 3) {
                free(nova);
                printf("Formato inválido na linha: %s\n", linha);
                continue;
            }


            nova->prox = NULL;

            if (donos == NULL) {
                donos = nova;
            } else {
                ultima->prox = nova;
            }
            ultima = nova;
        }


    fclose(f);
}

void ListarDonos() {
    Dono *atual = donos;
    if (atual == NULL) {
        printf("Não há donos registados.\n");
        return;
    }
    printf("\n--- Lista de Donos ---\n");
    while (atual != NULL) {
        printf("NIF: %d\n", atual->numContribuinte);
        printf("Nome: %s\n", atual->nome);
        printf("Codigo Postal: %s\n", atual->codPostal);
        printf("--------------------\n");
        atual = atual->prox;
    }
}

void registarDono(){
    Dono* novo = malloc(sizeof(Dono));
    if(!novo){
        printf("Erro de alocação de memória.\n");
        return;
    }

    printf("Número de Contribuinte: ");
    scanf("%d", &novo->numContribuinte);
    getchar(); //Limpar buffer

    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0'; //Remover \n

    novo->prox = donos;
    donos = novo;

    printf("Dono registado com sucesso.\n");
}

int compararNomes (const void* a, const void* b){
    char* nomeA = *(char**)a;
    char* nomeB = *(char**)b;
    return strcmp(nomeA, nomeB);
}

void listarCondutoresOrdemAlfa(Dono* listaDonos){
    int count = 0;
    Dono* atual = listaDonos;

    while(atual != NULL){
        count++;
        atual = atual->prox;
    }

    //Array de ponteiros para os nomes
    char** nomes = (char**)malloc(count * sizeof(char*));
    if(nomes == NULL){
        printf("Erro de alocação de memória.\n");
        return;
    }

    //Preencher array
    atual = listaDonos;
    for(int i=0; i < count; i++){
        nomes[i] = strdup(atual->nome); //Copiar nome
        atual = atual->prox;
    }

    qsort(nomes, count, sizeof(char*), compararNomes);

    printf("Nomes dos Condutores (ordenados alfabeticamente):\n");
    for(int i = 0; i < count; i++){
        printf("%s\n", nomes[i]);
        free(nomes[i]); //Libertar memória do nome
    }

    free(nomes); //Libertar memória do array
}

int compararContribuintes(const void* a, const void* b){
    Dono* da = *(Dono**)a;
    Dono* db = *(Dono**)b;
    return da->numContribuinte - db->numContribuinte;
}

void listarCondutoresPorContribuinte(Dono* listaDonos){
    if (listaDonos == NULL) {
        printf("Lista de donos está vazia.\n");
        return;
    }

    int count = 0;
    Dono* temp = listaDonos;
    while (temp != NULL) {
        count++;
        temp = temp->prox;
    }

    Dono** arrayDonos = malloc(count * sizeof(Dono*));
    if (arrayDonos == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    temp = listaDonos;
    for (int i = 0; i < count; i++) {
        arrayDonos[i] = temp;
        temp = temp->prox;
    }

    qsort(arrayDonos, count, sizeof(Dono*), compararContribuintes);

    printf("\n--- Condutores ordenados por Número de Contribuinte ---\n");
    for (int i = 0; i < count; i++) {
        printf("NIF: %d | Nome: %s\n", arrayDonos[i]->numContribuinte, arrayDonos[i]->nome);
    }

    free(arrayDonos);
}
