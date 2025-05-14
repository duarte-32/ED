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
