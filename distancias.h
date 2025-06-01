#ifndef DISTANCIAS_H
#define DISTANCIAS_H

typedef struct Distancia{
    int codSensor1;
    int codSensor2;
    float distancia;
    struct Distancia* prox;
} Distancia;

void CarregarDistancias(const char* arquivo);
float obterDistanciaEntreSensores(Distancia* distancias, int s1, int s2);

#endif
