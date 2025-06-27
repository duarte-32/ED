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
