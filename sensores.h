#ifndef SENSORES_H
#define SENSORES_H

typedef struct Sensor {
    int codSensor;
    char designacao[100];
    char latitude[50];
    char longitude[50];
    struct Sensor* prox;
} Sensor;

void CarregarSensores(const char* arquivo);

#endif