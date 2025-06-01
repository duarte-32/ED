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
time_t stringParaTempo(const char* data){
    struct tm tm = {0};
    sscanf(data, "%d-%d-%d %d:%d:%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900;
    tm.tm_mon -=1;
    return mktime(&tm);
}



void rankingCirculacao(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim) {
    time_t tInicio = stringParaTempo(dataInicio);
    time_t tFim = stringParaTempo(dataFim);
    RankingVeiculo ranking[1000];
    int n = 0;

    Passagem* p = passagens;
    while (p) {
        if (p->tipoRegisto == 0) { // entrada
            Passagem* s = p->prox;
            while (s) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    time_t t1 = stringParaTempo(p->data);
                    time_t t2 = stringParaTempo(s->data);
                    if (t1 >= tInicio && t2 <= tFim && difftime(t2, t1) > 0) {
                        float d = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        if (d >= 0) {
                            int i, existe = 0;
                            for (i = 0; i < n; i++) {
                                if (ranking[i].codVeiculo == p->codVeiculo) {
                                    ranking[i].kmTotal += d;
                                    existe = 1;
                                    break;
                                }
                            }
                            if (!existe) {
                                ranking[n].codVeiculo = p->codVeiculo;
                                ranking[n].kmTotal = d;
                                n++;
                            }
                        }
                        break;
                    }
                }
                s = s->prox;
            }
        }
        p = p->prox;
    }

    // Ordenar
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (ranking[j].kmTotal > ranking[i].kmTotal) {
                RankingVeiculo tmp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = tmp;
            }

    // Mostrar
    printf("\n--- Ranking de circulação por veículo ---\n");
    for (int i = 0; i < n; i++) {
        Veiculo* v = obterVeiculo(veiculos, ranking[i].codVeiculo);
        if (v)
            printf("Matrícula: %s | Marca: %s | Modelo: %s | KM: %.2f\n", v->matricula, v->marca, v->modelo, ranking[i].kmTotal);
    }
}


void rankingPorMarca(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim) {
    time_t tInicio = stringParaTempo(dataInicio);
    time_t tFim = stringParaTempo(dataFim);
    RankingMarca marcas[100];
    int n = 0;

    Passagem* p = passagens;
    while (p) {
        if (p->tipoRegisto == 0) {
            Passagem* s = p->prox;
            while (s) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    time_t t1 = stringParaTempo(p->data);
                    time_t t2 = stringParaTempo(s->data);
                    if (t1 >= tInicio && t2 <= tFim && difftime(t2, t1) > 0) {
                        float d = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        if (d >= 0) {
                            Veiculo* v = obterVeiculo(veiculos, p->codVeiculo);
                            if (v) {
                                int i, existe = 0;
                                for (i = 0; i < n; i++) {
                                    if (strcmp(marcas[i].marca, v->marca) == 0) {
                                        marcas[i].kmTotal += d;
                                        existe = 1;
                                        break;
                                    }
                                }
                                if (!existe) {
                                    strcpy(marcas[n].marca, v->marca);
                                    marcas[n].kmTotal = d;
                                    n++;
                                }
                            }
                        }
                        break;
                    }
                }
                s = s->prox;
            }
        }
        p = p->prox;
    }

    // Ordenar
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (marcas[j].kmTotal > marcas[i].kmTotal) {
                RankingMarca tmp = marcas[i];
                marcas[i] = marcas[j];
                marcas[j] = tmp;
            }

    printf("\n--- Ranking por marca ---\n");
    for (int i = 0; i < n; i++)
        printf("Marca: %s | KM: %.2f\n", marcas[i].marca, marcas[i].kmTotal);
}

void listarInfracoes(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim) {
    time_t tInicio = stringParaTempo(dataInicio);
    time_t tFim = stringParaTempo(dataFim);

    printf("\n--- Veículos com velocidade média > 120 km/h ---\n");

    Passagem* p = passagens;
    while (p) {
        if (p->tipoRegisto == 0) {
            Passagem* s = p->prox;
            while (s) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    time_t t1 = stringParaTempo(p->data);
                    time_t t2 = stringParaTempo(s->data);
                    double horas = difftime(t2, t1) / 3600.0;

                    if (t1 >= tInicio && t2 <= tFim && horas > 0) {
                        float dist = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        float vmed = dist / horas;

                        if (vmed > 120.0) {
                            Veiculo* v = obterVeiculo(veiculos, p->codVeiculo);
                            if (v) printf("Matrícula: %s | Velocidade média: %.2f km/h\n", v->matricula, vmed);
                        }
                        break;
                    }
                }
                s = s->prox;
            }
        }
        p = p->prox;
    }
}


void rankingInfracoes(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, const char* dataInicio, const char* dataFim) {
    time_t tInicio = stringParaTempo(dataInicio);
    time_t tFim = stringParaTempo(dataFim);
    InfracaoVeiculo lista[1000];
    int n = 0;

    Passagem* p = passagens;
    while (p) {
        if (p->tipoRegisto == 0) {
            Passagem* s = p->prox;
            while (s) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    time_t t1 = stringParaTempo(p->data);
                    time_t t2 = stringParaTempo(s->data);
                    double horas = difftime(t2, t1) / 3600.0;

                    if (t1 >= tInicio && t2 <= tFim && horas > 0) {
                        float dist = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        float vmed = dist / horas;

                        if (vmed > 120.0) {
                            int i, existe = 0;
                            for (i = 0; i < n; i++) {
                                if (lista[i].codVeiculo == p->codVeiculo) {
                                    lista[i].numInfracoes++;
                                    existe = 1;
                                    break;
                                }
                            }
                            if (!existe) {
                                lista[n].codVeiculo = p->codVeiculo;
                                lista[n].numInfracoes = 1;
                                n++;
                            }
                        }
                        break;
                    }
                }
                s = s->prox;
            }
        }
        p = p->prox;
    }

    // Ordenar
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (lista[j].numInfracoes > lista[i].numInfracoes) {
                InfracaoVeiculo tmp = lista[i];
                lista[i] = lista[j];
                lista[j] = tmp;
            }

    printf("\n--- Ranking de infrações por veículo ---\n");
    for (int i = 0; i < n; i++) {
        Veiculo* v = obterVeiculo(veiculos, lista[i].codVeiculo);
        if (v) printf("Matrícula: %s | Infrações: %d\n", v->matricula, lista[i].numInfracoes);
    }
}

void calcularVelocidadesMedias(Passagem* passagens, Distancia* distancias, Veiculo* veiculos) {
    VelocidadeMedia lista[1000];
    int n = 0;

    for (Passagem* p = passagens; p; p = p->prox) {
        if (p->tipoRegisto == 0) {
            for (Passagem* s = p->prox; s; s = s->prox) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    time_t t1 = stringParaTempo(p->data);
                    time_t t2 = stringParaTempo(s->data);
                    double h = difftime(t2, t1) / 3600.0;

                    if (h > 0) {
                        float dist = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        if (dist >= 0) {
                            int i, found = 0;
                            for (i = 0; i < n; i++) {
                                if (lista[i].codVeiculo == p->codVeiculo) {
                                    lista[i].totalKm += dist;
                                    lista[i].totalHoras += h;
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found) {
                                lista[n].codVeiculo = p->codVeiculo;
                                lista[n].totalKm = dist;
                                lista[n].totalHoras = h;
                                n++;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    printf("\n--- Velocidade média por veículo ---\n");
    for (int i = 0; i < n; i++) {
        Veiculo* v = obterVeiculo(veiculos, lista[i].codVeiculo);
        if (v && lista[i].totalHoras > 0)
            printf("Matrícula: %s | Vel. Média: %.2f km/h\n", v->matricula, lista[i].totalKm / lista[i].totalHoras);
    }
}



void marcaMaisRapida(Passagem* passagens, Distancia* distancias, Veiculo* veiculos) {
    MarcaVelocidade lista[100];
    int n = 0;

    for (Passagem* p = passagens; p; p = p->prox) {
        if (p->tipoRegisto == 0) {
            for (Passagem* s = p->prox; s; s = s->prox) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    double h = difftime(stringParaTempo(s->data), stringParaTempo(p->data)) / 3600.0;
                    if (h > 0) {
                        float dist = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        if (dist >= 0) {
                            Veiculo* v = obterVeiculo(veiculos, p->codVeiculo);
                            if (v) {
                                int i, found = 0;
                                for (i = 0; i < n; i++) {
                                    if (strcmp(lista[i].marca, v->marca) == 0) {
                                        lista[i].totalKm += dist;
                                        lista[i].totalHoras += h;
                                        found = 1;
                                        break;
                                    }
                                }
                                if (!found) {
                                    strcpy(lista[n].marca, v->marca);
                                    lista[n].totalKm = dist;
                                    lista[n].totalHoras = h;
                                    n++;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    int max = -1;
    float vmax = 0.0;
    for (int i = 0; i < n; i++) {
        float vm = lista[i].totalKm / lista[i].totalHoras;
        if (vm > vmax) {
            vmax = vm;
            max = i;
        }
    }

    if (max != -1)
        printf("Marca mais rápida: %s (%.2f km/h)\n", lista[max].marca, vmax);
}


void donoMaisRapido(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, Dono* donos) {
    DonoVelocidade lista[500];
    int n = 0;

    for (Passagem* p = passagens; p; p = p->prox) {
        if (p->tipoRegisto == 0) {
            for (Passagem* s = p->prox; s; s = s->prox) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    double h = difftime(stringParaTempo(s->data), stringParaTempo(p->data)) / 3600.0;
                    if (h > 0) {
                        float dist = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                        Veiculo* v = obterVeiculo(veiculos, p->codVeiculo);
                        if (v && dist >= 0) {
                            int i, found = 0;
                            for (i = 0; i < n; i++) {
                                if (lista[i].dono == v->dono) {
                                    lista[i].totalKm += dist;
                                    lista[i].totalHoras += h;
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found) {
                                lista[n].dono = v->dono;
                                lista[n].totalKm = dist;
                                lista[n].totalHoras = h;
                                n++;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    int max = -1;
    float vmax = 0;
    for (int i = 0; i < n; i++) {
        float vm = lista[i].totalKm / lista[i].totalHoras;
        if (vm > vmax) {
            vmax = vm;
            max = i;
        }
    }

    if (max != -1) {
        Dono* d = donos;
        while (d) {
            if (d->numContribuinte == lista[max].dono) {
                printf("Dono mais rápido: %s (%.2f km/h)\n", d->nome, vmax);
                break;
            }
            d = d->prox;
        }
    }
}

void mediaPorCodigoPostal(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, Dono* donos, const char* codigoPostal) {
    float totalKm = 0;
    float totalHoras = 0;

    for (Passagem* p = passagens; p; p = p->prox) {
        if (p->tipoRegisto == 0) {
            for (Passagem* s = p->prox; s; s = s->prox) {
                if (s->codVeiculo == p->codVeiculo && s->tipoRegisto == 1) {
                    float h = difftime(stringParaTempo(s->data), stringParaTempo(p->data)) / 3600.0;
                    float d = obterDistanciaEntreSensores(distancias, p->idSensor, s->idSensor);
                    if (h > 0 && d >= 0) {
                        Veiculo* v = obterVeiculo(veiculos, p->codVeiculo);
                        if (v) {
                            Dono* dono = donos;
                            while (dono) {
                                if (dono->numContribuinte == v->dono && strcmp(dono->codPostal, codigoPostal) == 0) {
                                    totalKm += d;
                                    totalHoras += h;
                                    break;
                                }
                                dono = dono->prox;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    if (totalHoras > 0)
        printf("Velocidade média dos condutores com código postal %s: %.2f km/h\n", codigoPostal, totalKm / totalHoras);
    else
        printf("Nenhuma viagem encontrada para código postal %s.\n", codigoPostal);
}



void marcaMaisComum(Veiculo* veiculos) {
    MarcaContador marcas[100];
    int n = 0;

    for (Veiculo* v = veiculos; v; v = v->prox) {
        int i, found = 0;
        for (i = 0; i < n; i++) {
            if (strcmp(marcas[i].marca, v->marca) == 0) {
                marcas[i].total++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(marcas[n].marca, v->marca);
            marcas[n].total = 1;
            n++;
        }
    }

    int max = 0;
    for (int i = 1; i < n; i++) {
        if (marcas[i].total > marcas[max].total)
            max = i;
    }

    printf("Marca mais comum: %s (%d veículos)\n", marcas[max].marca, marcas[max].total);
}

