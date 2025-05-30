#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "distancias.h"


Distancia *distancias = NULL;

void CarregarDistancias(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    Distancia *nova, *ultima = NULL;
    int codSensor1, codSensor2;
    float distancia;

    while (fscanf(f, "%d %d %f", &codSensor1, &codSensor2, &distancia) == 3) {
        nova = (Distancia*)malloc(sizeof(Distancia));
        nova->codSensor1 = codSensor1;
        nova->codSensor2 = codSensor2;
        nova->distancia = distancia;
        nova->prox = NULL;

        if (distancias == NULL) {
            distancias = nova;
        } else {
            ultima->prox = nova;
        }
        ultima = nova;
    }

    fclose(f);
}
