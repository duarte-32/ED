#include "structs.h"
#include <time.h>

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

    printf("Data (dd-mm-aaaa hh:mm:ss): \n");
    fgets(nova->data, sizeof(nova->data), stdin);
    nova->data[strcspn(nova->data, "\n")] = '\0';

    printf("Tipo de registo (0=Entrada, 1=Saída): ");
    scanf("%d", &nova->tipoRegisto);


    nova->prox = passagens;
    passagens = nova;


    // Escrever no ficheiro
    FILE* f = fopen("Passagem.txt", "a"); // "a" = append
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro Passagem.txt para escrita.\n");
        return;
    }

    fprintf(f, "%d\t%d\t%s\t%d\n",
            nova->idSensor,
            nova->codVeiculo,
            nova->data,
            nova->tipoRegisto);

    fclose(f);


    printf("Passagem registada com sucesso!\n");
}



void rankingCirculacao(Veiculo* veiculos, Passagem* passagens, Sensor* sensores,
                      const char* dataInicio, const char* dataFim) {
    // Fase 1: Contar veículos únicos no período
    int numVeiculos = 0;

    // Primeiro, contar o número total de veículos
    int totalVeiculos = 0;
    Veiculo* v = veiculos;
    while (v != NULL) {
        totalVeiculos++;
        v = v->prox;
    }

    // Alocar array para códigos de veículos
    int* codigosVeiculos = malloc(totalVeiculos * sizeof(int));
    if (!codigosVeiculos) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    // Identificar veículos únicos no período
    Passagem* p = passagens;
    while (p != NULL) {
        if (strcmp(p->data, dataInicio) >= 0 && strcmp(p->data, dataFim) <= 0) {
            int encontrado = 0;
            for (int i = 0; i < numVeiculos; i++) {
                if (codigosVeiculos[i] == p->codVeiculo) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                codigosVeiculos[numVeiculos++] = p->codVeiculo;
            }
        }
        p = p->prox;
    }

    // Fase 2: Calcular quilometragem para cada veículo
    RankingVeiculo* ranking = malloc(numVeiculos * sizeof(RankingVeiculo));
    if (!ranking) {
        free(codigosVeiculos);
        printf("Erro de alocação de memória.\n");
        return;
    }

    for (int i = 0; i < numVeiculos; i++) {
        ranking[i].codVeiculo = codigosVeiculos[i];
        ranking[i].kmTotal = 0.0;

        // Encontrar todas as passagens de entrada e saída para este veículo
        Passagem* entrada = NULL;
        Passagem* atual = passagens;

        while (atual != NULL) {
            if (atual->codVeiculo == codigosVeiculos[i] &&
                strcmp(atual->data, dataInicio) >= 0 &&
                strcmp(atual->data, dataFim) <= 0) {

                if (atual->tipoRegisto == 0) { // Entrada
                    entrada = atual;
                } else if (atual->tipoRegisto == 1 && entrada != NULL) { // Saída
                    // Calcular distância entre os sensores
                    float distancia = encontrarDistanciaEntreSensores(sensores, entrada->idSensor, atual->idSensor);
                    if (distancia > 0) {
                        ranking[i].kmTotal += distancia;
                    }
                    entrada = NULL;
                }
            }
            atual = atual->prox;
        }
    }

    // Fase 3: Ordenar o ranking
    qsort(ranking, numVeiculos, sizeof(RankingVeiculo), compararRankingVeiculos);

    // Fase 4: Imprimir resultados
    printf("\nRanking de Circulação entre %s e %s:\n", dataInicio, dataFim);
    printf("------------------------------------------------\n");
    printf("Posição\tMatrícula\tMarca\tModelo\tKm Percorridos\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < numVeiculos; i++) {
        Veiculo* veic = encontrarVeiculoPorCodigo(veiculos, ranking[i].codVeiculo);
        if (veic != NULL) {
            printf("%d\t%s\t%s\t%s\t%.2f km\n",
                   i+1, veic->matricula, veic->marca, veic->modelo, ranking[i].kmTotal);
        }
    }
    printf("------------------------------------------------\n");

    // Liberar memória
    free(codigosVeiculos);
    free(ranking);
}

void rankingPorMarca(Veiculo* veiculos, Passagem* passagens, Distancia* distancias,
                    const char* dataInicio, const char* dataFim) {
    // Fase 1: Contar marcas únicas
    int numMarcas = 0;
    int maxMarcas = 10; // Tamanho inicial do array
    char** marcas = malloc(maxMarcas * sizeof(char*));

    // Coletar marcas únicas
    Veiculo* v = veiculos;
    while (v != NULL) {
        int encontrada = 0;

        // Verificar se a marca já está na lista
        for (int i = 0; i < numMarcas; i++) {
            if (strcmp(marcas[i], v->marca) == 0) {
                encontrada = 1;
                break;
            }
        }

        // Se não encontrada, adicionar
        if (!encontrada) {
            // Redimensionar array se necessário
            if (numMarcas >= maxMarcas) {
                maxMarcas *= 2;
                marcas = realloc(marcas, maxMarcas * sizeof(char*));
            }

            marcas[numMarcas] = malloc(50 * sizeof(char));
            strcpy(marcas[numMarcas], v->marca);
            numMarcas++;
        }
        v = v->prox;
    }

    // Fase 2: Calcular quilometragem por marca
    RankingMarca* ranking = malloc(numMarcas * sizeof(RankingMarca));
    for (int i = 0; i < numMarcas; i++) {
        strcpy(ranking[i].marca, marcas[i]);
        ranking[i].kmTotal = 0.0;

        // Para cada veículo desta marca
        v = veiculos;
        while (v != NULL) {
            if (strcmp(v->marca, marcas[i]) == 0) {
                // Procurar passagens deste veículo no período
                Passagem* entrada = NULL;
                Passagem* p = passagens;

                while (p != NULL) {
                    if (p->codVeiculo == v->codVeiculo &&
                        strcmp(p->data, dataInicio) >= 0 &&
                        strcmp(p->data, dataFim) <= 0) {

                        if (p->tipoRegisto == 0) { // Entrada
                            entrada = p;
                        }
                        else if (p->tipoRegisto == 1 && entrada != NULL) { // Saída
                            float distancia = encontrarDistancia(distancias, entrada->idSensor, p->idSensor);
                            if (distancia > 0) {
                                ranking[i].kmTotal += distancia;
                            }
                            entrada = NULL;
                        }
                    }
                    p = p->prox;
                }
            }
            v = v->prox;
        }
    }

    // Fase 3: Ordenar e exibir resultados
    qsort(ranking, numMarcas, sizeof(RankingMarca), compararRankingMarcas);

    printf("\nRanking por Marca (%s a %s)\n", dataInicio, dataFim);
    printf("---------------------------------\n");
    printf("Posição\tMarca\t\tKm Totais\n");
    printf("---------------------------------\n");

    for (int i = 0; i < numMarcas; i++) {
        printf("%d\t%-10s\t%.2f km\n", i+1, ranking[i].marca, ranking[i].kmTotal);
    }
    printf("---------------------------------\n");

    // Liberar memória
    for (int i = 0; i < numMarcas; i++) {
        free(marcas[i]);
    }
    free(marcas);
    free(ranking);
}


void listarInfracoesVelocidade(Passagem* passagens, Veiculo* veiculos, Sensor* sensores,
                              const char* dataInicio, const char* dataFim) {
    // Fase 1: Contar número total de passagens para dimensionar arrays
    int totalPassagens = 0;
    Passagem* p = passagens;
    while (p != NULL) {
        totalPassagens++;
        p = p->prox;
    }

    // Alocar arrays temporários
    int* codVeiculos = malloc(totalPassagens * sizeof(int));
    int* sensoresEntrada = malloc(totalPassagens * sizeof(int));
    int* sensoresSaida = malloc(totalPassagens * sizeof(int));
    char** datasEntrada = malloc(totalPassagens * sizeof(char*));
    char** datasSaida = malloc(totalPassagens * sizeof(char*));

    for (int i = 0; i < totalPassagens; i++) {
        datasEntrada[i] = malloc(20 * sizeof(char));
        datasSaida[i] = malloc(20 * sizeof(char));
    }

    // Fase 2: Identificar pares de entrada/saída válidos no período
    int numPares = 0;
    Passagem* entrada = NULL;
    p = passagens;

    while (p != NULL) {
        if (strcmp(p->data, dataInicio) >= 0 && strcmp(p->data, dataFim) <= 0) {
            if (p->tipoRegisto == 0) { // Registro de entrada
                entrada = p;
            }
            else if (p->tipoRegisto == 1 && entrada != NULL && entrada->codVeiculo == p->codVeiculo) {
                // Registro de saída correspondente a uma entrada
                codVeiculos[numPares] = p->codVeiculo;
                sensoresEntrada[numPares] = entrada->idSensor;
                sensoresSaida[numPares] = p->idSensor;
                strcpy(datasEntrada[numPares], entrada->data);
                strcpy(datasSaida[numPares], p->data);
                numPares++;
                entrada = NULL;
            }
        }
        p = p->prox;
    }

    // Fase 3: Verificar infrações de velocidade
    Infracao* infracoes = malloc(numPares * sizeof(Infracao));
    int numInfracoes = 0;

    for (int i = 0; i < numPares; i++) {
        float distancia = encontrarDistanciaEntreSensores(sensores, sensoresEntrada[i], sensoresSaida[i]);
        float horas = calcularDiferencaHoras(datasEntrada[i], datasSaida[i]);

        if (distancia > 0 && horas > 0) {
            float velocidade = distancia / horas;

            if (velocidade > 120.0) { // Limite de velocidade excedido
                Veiculo* v = encontrarVeiculoPorCodigo(veiculos, codVeiculos[i]);
                if (v != NULL) {
                    // Verificar se já existe registro para esta matrícula
                    int encontrada = 0;
                    for (int j = 0; j < numInfracoes; j++) {
                        if (strcmp(infracoes[j].matricula, v->matricula) == 0) {
                            infracoes[j].numInfracoes++;
                            encontrada = 1;
                            break;
                        }
                    }

                    if (!encontrada) {
                        strcpy(infracoes[numInfracoes].matricula, v->matricula);
                        infracoes[numInfracoes].numInfracoes = 1;
                        numInfracoes++;
                    }
                }
            }
        }
    }

    // Fase 4: Ordenar e exibir resultados
    qsort(infracoes, numInfracoes, sizeof(Infracao), compararInfracoes);

    printf("\nInfrações de Velocidade (%s a %s)\n", dataInicio, dataFim);
    printf("---------------------------------\n");
    printf("Matrícula\tNº Infrações\n");
    printf("---------------------------------\n");

    for (int i = 0; i < numInfracoes; i++) {
        printf("%s\t%d\n", infracoes[i].matricula, infracoes[i].numInfracoes);
    }

    if (numInfracoes == 0) {
        printf("Nenhuma infração registrada no período.\n");
    }
    printf("---------------------------------\n");

    // Liberar memória
    for (int i = 0; i < totalPassagens; i++) {
        free(datasEntrada[i]);
        free(datasSaida[i]);
    }
    free(datasEntrada);
    free(datasSaida);
    free(codVeiculos);
    free(sensoresEntrada);
    free(sensoresSaida);
    free(infracoes);
}

void rankingInfracoesPorVeiculo(Passagem* passagens, Distancia* distancias, Veiculo* veiculos,
                               const char* dataInicio, const char* dataFim) {
    // Fase 1: Contar número total de veículos para dimensionar array
    int totalVeiculos = 0;
    Veiculo* v = veiculos;
    while (v != NULL) {
        totalVeiculos++;
        v = v->prox;
    }

    // Alocar array para infrações
    Infracao* infracoes = malloc(totalVeiculos * sizeof(Infracao));
    int numInfracoes = 0;

    // Fase 2: Processar todas as passagens no período
    Passagem* entrada = NULL;
    Passagem* atual = passagens;

    while (atual != NULL) {
        if (strcmp(atual->data, dataInicio) >= 0 && strcmp(atual->data, dataFim) <= 0) {
            if (atual->tipoRegisto == 0) { // Registro de entrada
                entrada = atual;
            }
            else if (atual->tipoRegisto == 1 && entrada != NULL && entrada->codVeiculo == atual->codVeiculo) {
                // Registro de saída correspondente
                float distancia = encontrarDistanciaEntreSensores(distancias, entrada->idSensor, atual->idSensor);
                float horas = calcularDiferencaHoras(entrada->data, atual->data);

                if (distancia > 0 && horas > 0) {
                    float velocidade = distancia / horas;

                    if (velocidade > 120.0) { // Limite de velocidade excedido
                        Veiculo* veic = encontrarVeiculoPorCodigo(veiculos, atual->codVeiculo);
                        if (veic != NULL) {
                            // Verificar se já existe registro para esta matrícula
                            int encontrada = 0;
                            for (int i = 0; i < numInfracoes; i++) {
                                if (strcmp(infracoes[i].matricula, veic->matricula) == 0) {
                                    infracoes[i].numInfracoes++;
                                    encontrada = 1;
                                    break;
                                }
                            }

                            if (!encontrada) {
                                strcpy(infracoes[numInfracoes].matricula, veic->matricula);
                                infracoes[numInfracoes].numInfracoes = 1;
                                numInfracoes++;
                            }
                        }
                    }
                }
                entrada = NULL;
            }
        }
        atual = atual->prox;
    }

    // Fase 3: Ordenar e exibir resultados
    qsort(infracoes, numInfracoes, sizeof(Infracao), compararInfracoes);

    printf("\nRanking de Infrações por Veículo (%s a %s)\n", dataInicio, dataFim);
    printf("------------------------------------------------\n");
    printf("Posição\tMatrícula\tMarca\tModelo\tNº Infrações\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < numInfracoes; i++) {
        Veiculo* veic = encontrarVeiculoPorMatricula(veiculos, infracoes[i].matricula);
        if (veic != NULL) {
            printf("%d\t%s\t%s\t%s\t%d\n",
                   i+1, veic->matricula, veic->marca, veic->modelo, infracoes[i].numInfracoes);
        }
    }

    if (numInfracoes == 0) {
        printf("Nenhuma infração registrada no período.\n");
    }
    printf("------------------------------------------------\n");

    // Liberar memória
    free(infracoes);
}

void listarVelocidadesMedias(Passagem* passagens, Distancia* distancias, Veiculo* veiculos,
                           const char* dataInicio, const char* dataFim) {
    // Fase 1: Contar veículos únicos no período
    int numVeiculos = 0;
    int* codVeiculos = NULL;

    // Primeiro contar o número total de veículos para alocação
    int totalVeiculos = 0;
    Veiculo* v = veiculos;
    while (v != NULL) {
        totalVeiculos++;
        v = v->prox;
    }

    // Alocar array para códigos de veículos
    codVeiculos = malloc(totalVeiculos * sizeof(int));

    // Identificar veículos únicos no período
    Passagem* p = passagens;
    while (p != NULL) {
        if (strcmp(p->data, dataInicio) >= 0 && strcmp(p->data, dataFim) <= 0) {
            int encontrado = 0;
            for (int i = 0; i < numVeiculos; i++) {
                if (codVeiculos[i] == p->codVeiculo) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                codVeiculos[numVeiculos++] = p->codVeiculo;
            }
        }
        p = p->prox;
    }

    // Fase 2: Calcular velocidades médias
    VelocidadeMedia* velocidades = malloc(numVeiculos * sizeof(VelocidadeMedia));

    for (int i = 0; i < numVeiculos; i++) {
        Veiculo* veic = encontrarVeiculoPorCodigo(veiculos, codVeiculos[i]);
        if (veic != NULL) {
            strcpy(velocidades[i].matricula, veic->matricula);

            float totalKm = 0.0;
            float totalHoras = 0.0;
            Passagem* entrada = NULL;
            Passagem* atual = passagens;

            while (atual != NULL) {
                if (atual->codVeiculo == codVeiculos[i] &&
                    strcmp(atual->data, dataInicio) >= 0 &&
                    strcmp(atual->data, dataFim) <= 0) {

                    if (atual->tipoRegisto == 0) { // Entrada
                        entrada = atual;
                    }
                    else if (atual->tipoRegisto == 1 && entrada != NULL) { // Saída
                        float distancia = encontrarDistanciaEntreSensores(distancias, entrada->idSensor, atual->idSensor);
                        float horas = calcularDiferencaHoras(entrada->data, atual->data);

                        if (distancia > 0 && horas > 0) {
                            totalKm += distancia;
                            totalHoras += horas;
                        }
                        entrada = NULL;
                    }
                }
                atual = atual->prox;
            }

            velocidades[i].velocidadeMedia = (totalHoras > 0) ? (totalKm / totalHoras) : 0.0;
        }
    }

    // Fase 3: Ordenar e exibir resultados
    qsort(velocidades, numVeiculos, sizeof(VelocidadeMedia), compararVelocidadesMedias);

    printf("\nVelocidades Médias (%s a %s)\n", dataInicio, dataFim);
    printf("------------------------------------------------\n");
    printf("Posição\tMatrícula\tMarca\tModelo\tVelocidade Média\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < numVeiculos; i++) {
        Veiculo* veic = encontrarVeiculoPorCodigo(veiculos, codVeiculos[i]);
        if (veic != NULL && velocidades[i].velocidadeMedia > 0) {
            printf("%d\t%s\t%s\t%s\t%.2f km/h\n",
                   i+1, veic->matricula, veic->marca, veic->modelo, velocidades[i].velocidadeMedia);
        }
    }

    if (numVeiculos == 0) {
        printf("Nenhum veículo circulou no período especificado.\n");
    }
    printf("------------------------------------------------\n");

    // Liberar memória
    free(codVeiculos);
    free(velocidades);
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

