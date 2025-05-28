#include "structs.h"

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
