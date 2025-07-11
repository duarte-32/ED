#include "structs.h"
#include <time.h>

Dono *donos = NULL;

void CarregarDonos(const char* arquivo) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return;
    }

    Dono *nova, *ultima = NULL;
        char linha[200];

        while (fgets(linha, sizeof(linha), f)) {

            // Remove o \n no final se existir
            linha[strcspn(linha, "\n")] = 0;

            nova = (Dono*)malloc(sizeof(Dono));

            // Lê o nome até detetar num numero(que corresponde ao código postal)
            // Isto evita a não leitura do código postal.
            if (sscanf(linha, "%d %99[^0-9] %19s",
                      &nova->numContribuinte,
                      nova->nome,
                      nova->codPostal) != 3) {
                free(nova);
                printf("Formato inválido na linha: %s\n", linha);
                continue;
            }


            nova->prox = NULL;

            if (donos == NULL) {
                donos = nova;
            } else {
                ultima->prox = nova;
            }
            ultima = nova;
        }


    fclose(f);
}

void ListarDonos() {
    Dono *atual = donos;
    if (atual == NULL) {
        printf("Não há donos registados.\n");
        return;
    }
    printf("\n--- Lista de Donos ---\n");
    while (atual != NULL) {
        printf("NIF: %d\n", atual->numContribuinte);
        printf("Nome: %s\n", atual->nome);
        printf("Codigo Postal: %s\n", atual->codPostal);
        printf("--------------------\n");
        atual = atual->prox;
    }
}

void registarDono(){
    Dono* novo = malloc(sizeof(Dono));
    if(!novo){
        printf("Erro de alocação de memória.\n");
        return;
    }

    printf("Número de Contribuinte: ");
    scanf("%d", &novo->numContribuinte);
    getchar(); //Limpar buffer

    printf("Nome: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0'; //Remover \n

    novo->prox = donos;
    donos = novo;

    //Escrever no ficheiro
    FILE* f = fopen("donos.txt", "a");
    if(f==NULL){
        printf("\nErro ao abrir o ficheiro para escrita.\n");
        return;
    }

    fprintf(f, "%d\t %s\t %19s\n", novo->numContribuinte, novo->nome, novo->codPostal);
    fclose(f);
    printf("Dono registado com sucesso.\n");
}


void listarCondutoresOrdemAlfa(Dono* listaDonos){
    int count = 0;
    Dono* atual = listaDonos;

    while(atual != NULL){
        count++;
        atual = atual->prox;
    }

    //Array de ponteiros para os nomes
    char** nomes = (char**)malloc(count * sizeof(char*));
    if(nomes == NULL){
        printf("Erro de alocação de memória.\n");
        return;
    }

    //Preencher array
    atual = listaDonos;
    for(int i=0; i < count; i++){
        nomes[i] = strdup(atual->nome); //Copiar nome
        atual = atual->prox;
    }

    qsort(nomes, count, sizeof(char*), compararNomes);

    printf("Nomes dos Condutores (ordenados alfabeticamente):\n");
    for(int i = 0; i < count; i++){
        printf("%s\n", nomes[i]);
        free(nomes[i]); //Libertar memória do nome
    }

    free(nomes); //Libertar memória do array
}



void listarCondutoresPorContribuinte(Dono* listaDonos){
    if (listaDonos == NULL) {
        printf("Lista de donos está vazia.\n");
        return;
    }

    int count = 0;
    Dono* temp = listaDonos;
    while (temp != NULL) {
        count++;
        temp = temp->prox;
    }

    Dono** arrayDonos = malloc(count * sizeof(Dono*));
    if (arrayDonos == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    temp = listaDonos;
    for (int i = 0; i < count; i++) {
        arrayDonos[i] = temp;
        temp = temp->prox;
    }

    qsort(arrayDonos, count, sizeof(Dono*), compararContribuintes);

    printf("\n--- Condutores ordenados por Número de Contribuinte ---\n");
    for (int i = 0; i < count; i++) {
        printf("NIF: %d | Nome: %s\n", arrayDonos[i]->numContribuinte, arrayDonos[i]->nome);
    }

    free(arrayDonos);
}

void exportarParaCSV(Dono* donos, Veiculo* veiculos, Sensor* sensores, Passagem* passagens) {
    FILE* f = fopen("base_dados.csv", "w");
    if (!f) {
        printf("Erro ao criar ficheiro CSV.\n");
        return;
    }

    fprintf(f, "Dono,NIF,Código Postal\n");
    for (Dono* d = donos; d; d = d->prox)
        fprintf(f, "%s,%d,%s\n", d->nome, d->numContribuinte, d->codPostal);

    fprintf(f, "\nMatrícula,Marca,Modelo,Ano,NIF Dono,Código Veículo\n");
    for (Veiculo* v = veiculos; v; v = v->prox)
        fprintf(f, "%s,%s,%s,%d,%d,%d\n", v->matricula, v->marca, v->modelo, v->ano, v->dono, v->codVeiculo);

    fprintf(f, "\nSensor,Código,Latitude,Longitude\n");
    for (Sensor* s = sensores; s; s = s->prox)
        fprintf(f, "%s,%d,%s,%s\n", s->designacao, s->codSensor, s->latitude, s->longitude);

    fprintf(f, "\nSensor,Veículo,Data,Tipo\n");
    for (Passagem* p = passagens; p; p = p->prox)
        fprintf(f, "%d,%d,%s,%d\n", p->idSensor, p->codVeiculo, p->data, p->tipoRegisto);

    fclose(f);
    printf("Base de dados exportada para base_dados.csv\n");
}

void exportarParaXML(Dono* donos, Veiculo* veiculos) {
    FILE* f = fopen("base_dados.xml", "w");
    if (!f) {
        printf("Erro ao criar ficheiro XML.\n");
        return;
    }

    fprintf(f, "<BaseDeDados>\n");

    fprintf(f, "  <Donos>\n");
    for (Dono* d = donos; d; d = d->prox) {
        fprintf(f, "    <Dono>\n");
        fprintf(f, "      <Nome>%s</Nome>\n", d->nome);
        fprintf(f, "      <NIF>%d</NIF>\n", d->numContribuinte);
        fprintf(f, "      <CodigoPostal>%s</CodigoPostal>\n", d->codPostal);
        fprintf(f, "    </Dono>\n");
    }
    fprintf(f, "  </Donos>\n");

    fprintf(f, "  <Veiculos>\n");
    for (Veiculo* v = veiculos; v; v = v->prox) {
        fprintf(f, "    <Veiculo>\n");
        fprintf(f, "      <Matricula>%s</Matricula>\n", v->matricula);
        fprintf(f, "      <Marca>%s</Marca>\n", v->marca);
        fprintf(f, "      <Modelo>%s</Modelo>\n", v->modelo);
        fprintf(f, "      <Ano>%d</Ano>\n", v->ano);
        fprintf(f, "      <Dono>%d</Dono>\n", v->dono);
        fprintf(f, "    </Veiculo>\n");
    }
    fprintf(f, "  </Veiculos>\n");

    fprintf(f, "</BaseDeDados>\n");
    fclose(f);
    printf("Base de dados exportada para base_dados.xml\n");
}

void subnomeMaisComum(Dono* donos){
    SubnomeContador* lista = NULL;
    int tamanho = 0, capacidade = 10;
    lista = malloc(capacidade * sizeof(SubnomeContador));

    Dono* atual = donos;
    while(atual!=NULL){
        char* nome = strdup(atual->nome);//Duplica string
        char* token = strtok(nome, " "); //dividir a string em partes, pelo espaço
        char* ultimo = token;

        while (token!=NULL){
            ultimo = token;
            token = strtok(NULL, " ");
        }

        int encontrado = 0;
        //Procurar o subnome mais comum
        for(int i =0; i<tamanho; i++){
            if(strcmp(lista[i].subnome, ultimo)== 0){
                lista[i].contador++;
                encontrado = i;
                break;
            }
        }
        //Cada subnome é guardado apenas uma vez
        if(!encontrado){
            if(tamanho == capacidade){
                capacidade *=2;
                lista = realloc(lista, capacidade*sizeof(SubnomeContador));
            }
            strcpy(lista[tamanho].subnome, ultimo);
            lista[tamanho].contador = 1;
            tamanho++;
        }
        free(nome);
        atual = atual->prox;
    }
    //Encontrar e imprimir o subnome mais comum
    int max = 0;
    char maisComum[50] = ""; //Conteudo inicial vazio
    for(int i = 0; i<tamanho; i++){
        if(lista[i].contador > max){
            max = lista[i].contador;
            strcpy(maisComum, lista[i].subnome);
        }
    }

    printf("Subnome mais comum: %s (ocorrências: %d\n", maisComum, max);
    free(lista);
}

void velocidadeMediaPorCodigoPostal(Dono* donos, Veiculo* veiculos, Passagem* passagens, Distancia* distancias,
const char* codPostal, const char* dataInicio, const char* dataFim) {
    // Funções auxiliares necessárias
    float encontrarDistancia(Distancia* lista, int sensor1, int sensor2);
    float calcularDiferencaHoras(const char* data1, const char* data2);

    // Primeiro, contar o número total de donos para alocação
    int totalDonos = 0;
    Dono* tempD = donos;
    while (tempD != NULL) {
        totalDonos++;
        tempD = tempD->prox;
    }

    // Encontrar todos os donos com o código postal especificado
    int* contribuintes = malloc(totalDonos * sizeof(int));
    int numDonos = 0;

    Dono* d = donos;
    while (d != NULL) {
        if (strcmp(d->codPostal, codPostal) == 0) {
            contribuintes[numDonos++] = d->numContribuinte;
        }
        d = d->prox;
    }

    if (numDonos == 0) {
        printf("Nao foram encontrados condutores com codigo postal %s.\n", codPostal);
        free(contribuintes);
        return;
    }

    // Contar o número total de veículos para alocação
    int totalVeiculos = 0;
    Veiculo* tempV = veiculos;
    while (tempV != NULL) {
        totalVeiculos++;
        tempV = tempV->prox;
    }

    // Encontrar todos os veículos desses donos
    int* codigosVeiculos = malloc(totalVeiculos * sizeof(int));
    int numVeiculos = 0;

    Veiculo* v = veiculos;
    while (v != NULL) {
        for (int i = 0; i < numDonos; i++) {
            if (v->dono == contribuintes[i]) {
                codigosVeiculos[numVeiculos++] = v->codVeiculo;
                break;
            }
        }
        v = v->prox;
    }

    if (numVeiculos == 0) {
        printf("Nao foram encontrados veiculos de condutores com codigo postal %s.\n", codPostal);
        free(contribuintes);
        free(codigosVeiculos);
        return;
    }

    // Calcular velocidade média para esses veículos no período
    float totalKm = 0.0;
    float totalHoras = 0.0;
    int contagem = 0;

    Passagem* entrada = NULL;
    Passagem* atual = passagens;

    while (atual != NULL) {
        if (strcmp(atual->data, dataInicio) >= 0 && strcmp(atual->data, dataFim) <= 0) {
            // Verificar se o veículo está na lista
            int veiculoValido = 0;
            for (int i = 0; i < numVeiculos; i++) {
                if (codigosVeiculos[i] == atual->codVeiculo) {
                    veiculoValido = 1;
                    break;
                }
            }

            if (veiculoValido) {
                if (atual->tipoRegisto == 0) { // Entrada
                    entrada = atual;
                } else if (atual->tipoRegisto == 1 && entrada != NULL) { // Saída
                    float distancia = encontrarDistancia(distancias, entrada->idSensor, atual->idSensor);
                    float horas = calcularDiferencaHoras(entrada->data, atual->data);

                    if (distancia > 0 && horas > 0) {
                        totalKm += distancia;
                        totalHoras += horas;
                        contagem++;
                    }
                    entrada = NULL;
                }
            }
        }
        atual = atual->prox;
    }

    // Calcular e imprimir velocidade média
    if (contagem > 0) {
        float velocidadeMedia = totalKm / totalHoras;
        printf("A velocidade media dos condutores com codigo postal %s entre %s e %s foi: %.2f km/h\n",
               codPostal, dataInicio, dataFim, velocidadeMedia);
        printf("Baseado em %d trajetos analisados.\n", contagem);
    } else {
        printf("Nao foram encontrados trajetos para veiculos de condutores com codigo postal %s no periodo especificado.\n", codPostal);
    }

    free(contribuintes);
    free(codigosVeiculos);
}
