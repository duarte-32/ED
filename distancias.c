#include "structs.h"
#include <time.h>

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

//Função para procurar a distÂncia entre dois sensores
float obterDistanciaEntreSensores(Distancia* distancias, int s1, int s2){
    Distancia* d = distancias;
    while(d){
        if((d->codSensor1 == s1 && d->codSensor2 == s2)||
           (d->codSensor1 == s2 && d->codSensor2 == s1)){
            return d->distancia;
        }
        d = d->prox;
    }
    return -1.0;
}
