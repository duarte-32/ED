#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "passagens.h"

Passagem* passagens = NULL;

void CarregarPassagens(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    Passagem *nova, *ultima = NULL;
    int idSensor, codVeiculo, tipoRegisto;
    char data[30];

    while (fscanf(f, "%d %d %s %d", &idSensor, &codVeiculo, data, &tipoRegisto) == 4) {
        nova = (Passagem*)malloc(sizeof(Passagem));
        nova->idSensor = idSensor;
        nova->codVeiculo = codVeiculo;
        strcpy(nova->data, data);
        nova->tipoRegisto = tipoRegisto;
        nova->prox = NULL;

        if (passagens == NULL) {
            passagens = nova;
        } else {
            ultima->prox = nova;
        }
        ultima = nova;
    }

    fclose(f);
}

void registarPassagem(){
    Passagem* nova = malloc(sizeof(Passagem));
    if (!nova) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    printf("Código do sensor: ");
    scanf("%d", &nova->idSensor);
    printf("Código do veículo: ");
    scanf("%d", &nova->codVeiculo);
    getchar();

    printf("Data (dd-mm-aaaa hh:mm:ss): ");
    fgets(nova->data, sizeof(nova->data), stdin);
    nova->data[strcspn(nova->data, "\n")] = '\0';

    printf("Tipo de registo (0=Entrada, 1=Saída): ");
    scanf("%d", &nova->tipoRegisto);

    nova->prox = passagens;
    passagens = nova;

    printf("Passagem registada com sucesso!\n");
}
