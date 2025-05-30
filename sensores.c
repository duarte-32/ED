#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sensores.h"

Sensor *sensores = NULL;

void CarregarSensores(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    Sensor *nova, *ultima = NULL;
    int codSensor;
    char designacao[100], latitude[50], longitude[50];

    while (fscanf(f, "%d %[^\n] %s %s", &codSensor, designacao, latitude, longitude) == 4) {
        nova = (Sensor*)malloc(sizeof(Sensor));
        nova->codSensor = codSensor;
        strcpy(nova->designacao, designacao);
        strcpy(nova->latitude, latitude);
        strcpy(nova->longitude, longitude);
        nova->prox = NULL;

        if (sensores == NULL) {
            sensores = nova;
        } else {
            ultima->prox = nova;
        }
        ultima = nova;
    }

    fclose(f);
}
