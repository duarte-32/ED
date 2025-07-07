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
/*
void pesquisarMatricula() {

    char matricula[10];

    printf("Insira a matricula a pesquisar: \n");
    scanf("%s", matricula);


    Veiculo *atual = veiculos;
    while (atual != NULL) {
        if (strcmp(atual->matricula, matricula) == 0) {
            printf("Matrícula: %s\n", atual->matricula);
            printf("Marca: %s\n", atual->marca);
            printf("Modelo: %s\n", atual->modelo);
            printf("Ano: %d\n", atual->ano);
            printf("Dono (NIF): %d\n", atual->dono);
            printf("Código Veículo: %d\n", atual->codVeiculo);
            printf("-----------------------\n");

            return;
        }

        atual = atual->prox;
    }

}
*/
void listarVeiculosPorMatriculaPeriodo(Passagem* passagens, Veiculo* veiculos, const char* dataInicio, const char* dataFim){

   Veiculo** veiculosEncontrados = NULL;
   int tamanho = 0;
   int capacidade = 10;
   veiculosEncontrados = malloc(capacidade*sizeof(Veiculo*));

   TempoComMs inicio = stringParaTempoComMs(dataInicio);
   TempoComMs fim = stringParaTempoComMs(dataFim);

   //Evitar duplicados - lista de codVeiculos já adicionados
   int* codsAdicionados = malloc(capacidade*sizeof(int));
   int numCods = 0;

   for(Passagem* p = passagens; p!=NULL; p=p->prox){
        TempoComMs tempoPassagem = stringParaTempoComMs(p->data);

        if(tempoPassagem.tempoBase >= inicio.tempoBase && tempoPassagem.tempoBase <= fim.tempoBase){
            //Verificar se o veiculo atual já foi adicionado
            int jaAdicionado = 0;
            for(int i = 0; i< numCods; i++){
                    if(codsAdicionados[i] == p->codVeiculo){
                        jaAdicionado = 1;
                        break;
                    }
            }
            if(!jaAdicionado){
                //Obter o veiculo correspondente
                Veiculo* v = obterVeiculo(veiculos, p->codVeiculo);
                //Guardar ponteiros para veiculos e realocar memória para os arrays
                if(v!=NULL){
                    if(tamanho == capacidade){
                        capacidade *=2;
                        veiculosEncontrados = realloc(veiculosEncontrados, capacidade*sizeof(Veiculo*));
                        codsAdicionados = realloc(codsAdicionados, capacidade*sizeof(int));
                    }
                    veiculosEncontrados[tamanho++] = v;
                    codsAdicionados[numCods++] = p->codVeiculo;
                }
            }
        }
   }

   qsort(veiculosEncontrados, tamanho, sizeof(Veiculo*), compararPorMatricula);

   //Listar
   printf("\n--- Veículos que circularam entre %s e %s (ordenados por matrícula) ---\n");
   for(int i =0; i<tamanho; i++){
    printf("Matrícula: %s | MArca: %s | Modelo: %s | Ano: %d | Código: %d | NIF: %d\n",
            veiculosEncontrados[i]->matricula,
            veiculosEncontrados[i]->marca,
            veiculosEncontrados[i]->modelo,
            veiculosEncontrados[i]->ano,
            veiculosEncontrados[i]->codVeiculo,
            veiculosEncontrados[i]->dono);
   }
   free(veiculosEncontrados);
   free(codsAdicionados);
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

    novo->codVeiculo = proximoCodVeiculo;

    novo->prox = veiculos;
    veiculos = novo;

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
        float vel = velocidades[i].totalKm/ velocidades[i].totalHoras;
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
