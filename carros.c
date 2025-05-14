#include "structs.h"

Veiculo* veiculos = NULL;

void CarregarVeiculos(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    Veiculo *nova, *ultima = NULL;
    char matricula[10], marca[50], modelo[50];
    int ano, dono, codVeiculo;

    while (fscanf(f, "%s %s %s %d %d %d", matricula, marca, modelo, &ano, &dono, &codVeiculo) == 6) {
        nova = (Veiculo*)malloc(sizeof(Veiculo));
        strcpy(nova->matricula, matricula);
        strcpy(nova->marca, marca);
        strcpy(nova->modelo, modelo);
        nova->ano = ano;
        nova->dono = dono;
        nova->codVeiculo = codVeiculo;
        nova->prox = NULL;

        if (veiculos == NULL) {
            veiculos = nova;
        } else {
            ultima->prox = nova;
        }
        ultima = nova;
    }

    fclose(f);
}

void ListarVeiculos() {
    Veiculo *atual = veiculos;
    printf("\n--- Lista de Veículos ---\n");
    while (atual != NULL) {
        printf("Matrícula: %s\n", atual->matricula);
        printf("Marca: %s\n", atual->marca);
        printf("Modelo: %s\n", atual->modelo);
        printf("Ano: %d\n", atual->ano);
        printf("Dono (NIF): %d\n", atual->dono);
        printf("Código Veículo: %d\n", atual->codVeiculo);
        printf("-----------------------\n");
        atual = atual->prox;
    }
}
