#include "structs.h"

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
