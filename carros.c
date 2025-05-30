
#include "structs.h"

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

    printf("Veículo registado com sucesso! Código: %d\n", novo->codVeiculo);
}


int compararPorMatricula(const void* a, const void* b) {
    return strcmp((*(Veiculo**)a)->matricula, (*(Veiculo**)b)->matricula);
}

int compararPorMarca(const void* a, const void* b) {
    return strcmp((*(Veiculo**)a)->marca, (*(Veiculo**)b)->marca);
}

int compararPorModelo(const void* a, const void* b) {
    return strcmp((*(Veiculo**)a)->modelo, (*(Veiculo**)b)->modelo);
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

