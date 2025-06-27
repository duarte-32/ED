#include "structs.h"
#include <time.h>

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int compararNomes (const void* a, const void* b){
    char* nomeA = *(char**)a;
    char* nomeB = *(char**)b;
    return strcmp(nomeA, nomeB);
}

int compararContribuintes(const void* a, const void* b){
    Dono* da = *(Dono**)a;
    Dono* db = *(Dono**)b;
    return da->numContribuinte - db->numContribuinte;
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

time_t stringParaTempo(const char* data) {
    struct tm tm = {0};
    int dia, mes, ano, hora, minuto, segundo;

    if (sscanf(data, "%2d-%2d-%4d %2d:%2d:%2d", &dia, &mes, &ano, &hora, &minuto, &segundo) != 6) {
        fprintf(stderr, "Erro: Formato de data inválido: %s\n", data);
        return -1;
    }

    // Validação simples dos valores
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || hora < 0 || hora > 23 ||
        minuto < 0 || minuto > 59 || segundo < 0 || segundo > 59) {
        fprintf(stderr, "Erro: Valores de data/hora fora do intervalo: %s\n", data);
        return -1;
    }

    tm.tm_mday = dia;
    tm.tm_mon = mes - 1;
    tm.tm_year = ano - 1900;
    tm.tm_hour = hora;
    tm.tm_min = minuto;
    tm.tm_sec = segundo;

    return mktime(&tm);
}

