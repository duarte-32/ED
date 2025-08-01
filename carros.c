#include "structs.h"
#include <time.h>

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

int proximoCodVeiculo(){
    int max=0;
    Veiculo* v = veiculos;
    while(v){
        if(v->codVeiculo > max)
            max = v->codVeiculo;
        v = v->prox;
    }
    return max + 1;
}



void listarVeiculosPorMatriculaPeriodo(Passagem* passagens, Veiculo* veiculos, const char* dataInicio, const char* dataFim) {
    // Contar quantos veículos únicos circularam no período
    int count = 0;
    Passagem* p = passagens;

    // Primeiro passagem: contar veículos únicos no período
    while (p != NULL) {
        if (strcmp(p->data, dataInicio) >= 0 && strcmp(p->data, dataFim) <= 0) {
            // Verificar se já foi contado
            int jaContado = 0;
            Passagem* temp = passagens;
            while (temp != p) {
                if (temp->codVeiculo == p->codVeiculo &&
                    strcmp(temp->data, dataInicio) >= 0 &&
                    strcmp(temp->data, dataFim) <= 0) {
                    jaContado = 1;
                    break;
                }
                temp = temp->prox;
            }
            if (!jaContado) {
                count++;
            }
        }
        p = p->prox;
    }

    if (count == 0) {
        printf("Nenhum veículo circulou no período especificado.\n");
        return;
    }

    // Alocar memória para os veículos no período
    VeiculoPeriodo* veiculosPeriodo = malloc(count * sizeof(VeiculoPeriodo));
    if (veiculosPeriodo == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    // Segunda passagem: preencher o array
    int index = 0;
    p = passagens;
    while (p != NULL && index < count) {
        if (strcmp(p->data, dataInicio) >= 0 && strcmp(p->data, dataFim) <= 0) {
            // Verificar se já está no array
            int jaInserido = 0;
            for (int i = 0; i < index; i++) {
                Veiculo* v = encontrarVeiculoPorCodigo(veiculos, p->codVeiculo);
                if (v != NULL && strcmp(veiculosPeriodo[i].matricula, v->matricula) == 0) {
                    jaInserido = 1;
                    break;
                }
            }

            if (!jaInserido) {
                Veiculo* v = encontrarVeiculoPorCodigo(veiculos, p->codVeiculo);
                if (v != NULL) {
                    strcpy(veiculosPeriodo[index].matricula, v->matricula);
                    strcpy(veiculosPeriodo[index].marca, v->marca);
                    strcpy(veiculosPeriodo[index].modelo, v->modelo);
                    index++;
                }
            }
        }
        p = p->prox;
    }

    // Ordenar por matrícula (usando qsort)
    qsort(veiculosPeriodo, count, sizeof(VeiculoPeriodo), compararPorMatricula);

    // Imprimir resultados
    printf("Veiculos que circularam entre %s e %s:\n", dataInicio, dataFim);
    printf("Matricula\tMarca\tModelo\n");
    for (int i = 0; i < count; i++) {
        printf("%s\t%s\t%s\n", veiculosPeriodo[i].matricula,
               veiculosPeriodo[i].marca, veiculosPeriodo[i].modelo);
    }

    free(veiculosPeriodo);
}




void registarVeiculos(){
    Veiculo* novo = malloc(sizeof(Veiculo));
    if(!novo){
        printf("Erro ao alocar memória.\n");
        return;
    }

    printf("Matrícula: ");
    scanf("%s", novo->matricula);
    printf("Marca: ");
    scanf("%s", novo->marca);
    printf("Modelo: ");
    scanf("%s", novo->modelo);
    printf("Ano: ");
    scanf("%d", &novo->ano);
    printf("NIF do Dono: ");
    scanf("%d", &novo->dono);

    novo->codVeiculo = proximoCodVeiculo();

    novo->prox = veiculos;
    veiculos = novo;


    // Escrever no ficheiro
    FILE* f = fopen("Carros.txt", "a");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro Carros.txt para escrita.\n");
    return;
    }

    fprintf(f, "%s\t%s\t%s\t%d\t%d\t%d\n",
        novo->matricula,
        novo->marca,
        novo->modelo,
        novo->ano,
        novo->dono,
        novo->codVeiculo);

    fclose(f);


    printf("Veículo registado com sucesso! Código: %d\n", novo->codVeiculo);
}




void listarVeiculosOrdenados(Veiculo* listaVeiculos, int criterio) {
    if (listaVeiculos == NULL) {
        printf("Lista de veículos está vazia.\n");
        return;
    }

    int count = 0;
    Veiculo* temp = listaVeiculos;
    while (temp) {
        count++;
        temp = temp->prox;
    }

    Veiculo** array = malloc(count * sizeof(Veiculo*));
    if (!array) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    temp = listaVeiculos;
    for (int i = 0; i < count; i++) {
        array[i] = temp;
        temp = temp->prox;
    }

    switch (criterio) {
        case 1:
            qsort(array, count, sizeof(Veiculo*), compararPorMatricula);
            printf("\n--- Veículos ordenados por Matrícula ---\n");
            break;
        case 2:
            qsort(array, count, sizeof(Veiculo*), compararPorMarca);
            printf("\n--- Veículos ordenados por Marca ---\n");
            break;
        case 3:
            qsort(array, count, sizeof(Veiculo*), compararPorModelo);
            printf("\n--- Veículos ordenados por Modelo ---\n");
            break;
        default:
            printf("Critério de ordenação inválido.\n");
            free(array);
            return;
    }

    for (int i = 0; i < count; i++) {
        printf("Matrícula: %s | Marca: %s | Modelo: %s\n",
            array[i]->matricula,
            array[i]->marca,
            array[i]->modelo);
    }

    free(array);
}

//Percorre alista de veículo e devolve o ponteiro para o veículo que tem o código fornecido
Veiculo* obterVeiculo(Veiculo* lista, int cod){
    while(lista){
        if(lista->codVeiculo == cod) return lista;
        lista = lista->prox;
    }
    return NULL;
}


void carroMaisRapido(Passagem* passagens, Distancia* distancias, Veiculo* veiculos, Dono* donos){
    VelocidadeMedia* velocidades = NULL;
    int tamanho = 0;
    int capacidade = 10;
    velocidades = malloc(capacidade * sizeof(VelocidadeMedia));

    Passagem* p = passagens;
    while(p!=NULL){
        if(p->tipoRegisto == 0){ //Entrada
            Passagem* saida = p->prox;
            while(saida != NULL && !(saida->codVeiculo == p->codVeiculo && saida->tipoRegisto ==1)){
                saida = saida->prox;
            }
            if(saida!=NULL){
                float dist = obterDistanciaEntreSensores(distancias, p->idSensor, saida->idSensor);
                time_t t1 = stringParaTempo(p->data);
                time_t t2 = stringParaTempo(saida->data);
                float horas = difftime(t2, t1)/3600.0; //Calcular o tempo em horas

                if(horas > 0 && dist > 0){
                    int encontrado = 0;
                    for(int i = 0; i> tamanho; i++){
                        if(velocidades[i].codVeiculo == p->codVeiculo){
                            velocidades[i].totalKm += dist;
                            velocidades[i].totalHoras += horas;
                            encontrado = 1;
                            break;
                        }
                    }
                    //Adicionar um novo veículo ao array velocidades
                    if(!encontrado){
                        if(tamanho == capacidade){
                            capacidade *= 2;
                            velocidades = realloc(velocidades, capacidade * sizeof(VelocidadeMedia));
                        }
                        velocidades[tamanho].codVeiculo = p->codVeiculo;
                        velocidades[tamanho].totalKm = dist;
                        velocidades[tamanho].totalHoras = horas;
                        tamanho++;
                    }
                }
            }
        }
        p = p->prox;
    }

    float maxVel = 0;
    int codMax = -1;
    for(int i = 0; i<tamanho; i++){
        float vel = (float)velocidades[i].totalKm/ velocidades[i].totalHoras;
        if(vel > maxVel){
            maxVel = vel;
            codMax = velocidades[i].codVeiculo;
        }
    }

    if(codMax != -1){
        Veiculo* v = obterVeiculo(veiculos, codMax);
        Dono* d = donos;
        while (d!= NULL && d->numContribuinte != v->dono){
            d = d->prox;
        }
        printf("Carro mais rápido: %s %s (%s), Velocidade média: %.2f km/h\n", v->marca, v->modelo, v->matricula, maxVel);
        if(d!=NULL){
            printf("Dono: %s (NIF: %d)\n", d->nome, d->numContribuinte);
        }
    }

    free(velocidades);
}

void marcaMaisComum(Veiculo* veiculos) {
    if (veiculos == NULL) {
        printf("Lista de veículos vazia.\n");
        return;
    }

    MarcaContador* lista = NULL;
    int tamanho = 0, capacidade = 10;

    // Alocação inicial com verificação
    lista = malloc(capacidade * sizeof(MarcaContador));
    if (lista == NULL) {
        perror("Erro ao alocar memória");
        return;
    }

    Veiculo* atual = veiculos;
    while (atual != NULL) {
        // Criar cópia da marca para manipulação segura
        char* copiaMarca = strdup(atual->marca);
        if (copiaMarca == NULL) {
            perror("Erro ao duplicar string");
            free(lista);
            return;
        }

        // Extrair o último token (parte da marca)
        char* ultimoToken = NULL;
        char* token = strtok(copiaMarca, " ");

        while (token != NULL) {
            ultimoToken = token;
            token = strtok(NULL, " ");
        }

        if (ultimoToken != NULL) {
            int encontrado = 0;

            // Procurar a marca na lista
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(lista[i].marca, ultimoToken) == 0) {
                    lista[i].total++;
                    encontrado = 1;
                    break;
                }
            }

            // Se não encontrada, adicionar à lista
            if (!encontrado) {
                if (tamanho == capacidade) {
                    capacidade *= 2;
                    MarcaContador* temp = realloc(lista, capacidade * sizeof(MarcaContador));
                    if (temp == NULL) {
                        perror("Erro ao realocar memória");
                        free(copiaMarca);
                        free(lista);
                        return;
                    }
                    lista = temp;
                }

                strncpy(lista[tamanho].marca, ultimoToken, 49);
                lista[tamanho].marca[49] = '\0'; // Garantir terminação nula
                lista[tamanho].total = 1;
                tamanho++;
            }
        }

        free(copiaMarca);
        atual = atual->prox;
    }

    // Encontrar e imprimir a marca mais comum
    if (tamanho == 0) {
        printf("Nenhuma marca encontrada.\n");
        free(lista);
        return;
    }

    int max = 0;
    char maisComum[50] = "";

    for (int i = 0; i < tamanho; i++) {
        if (lista[i].total > max) {
            max = lista[i].total;
            strncpy(maisComum, lista[i].marca, 49);
            maisComum[49] = '\0';
        }
    }

    printf("Marca mais comum: %s (ocorrências: %d)\n", maisComum, max);
    free(lista);
}
