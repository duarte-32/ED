#include "Structs.h"
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

TempoComMs stringParaTempoComMs(const char* data) {
    struct tm tm = {0};
    int dia, mes, ano, hora, minuto, segundo, milissegundos = 0;
    TempoComMs resultado;

    int lidos =  sscanf(data, "%d-%d-%d %d:%d:%d.%d", &dia, &mes, &ano, &hora, &minuto, &segundo, &milissegundos);

    if (lidos < 6) {
        printf("Erro: Formato de data inválido: %s\n", data);
        resultado.tempoBase = (time_t)-1;
        resultado.milissegundos = -1;
        return resultado;
    }

    // Validação simples dos valores
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || hora < 0 || hora > 23 ||
        minuto < 0 || minuto > 59 || segundo < 0 || segundo > 59 || milissegundos < 0
        || milissegundos > 999) {
        fprintf(stderr, "Erro: Valores de data/hora fora do intervalo: %s\n", data);
        resultado.tempoBase = (time_t)-1;
        resultado.milissegundos = -1;
        return resultado;
    }

    tm.tm_mday = dia;
    tm.tm_mon = mes - 1;
    tm.tm_year = ano - 1900;
    tm.tm_hour = hora;
    tm.tm_min = minuto;
    tm.tm_sec = segundo;

    resultado.milissegundos = milissegundos;
    resultado.tempoBase = mktime(&tm);

    return resultado;
}


time_t stringParaTempo(const char* data){
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    int dia, mes, ano, hora, minuto, segundo;

    if (sscanf(data, "%d-%d-%d %d:%d:%d", &dia, &mes, &ano, &hora, &minuto, &segundo) != 6) {
        printf("Erro: formato de data inválido -> %s\n", data);
        return (time_t)-1;
    }

    tm.tm_mday = dia;
    tm.tm_mon = mes - 1;// Em C, janeiro = 0
    tm.tm_year = ano - 1900; // Em C, ano = anos desde 1900
    tm.tm_hour = hora;
    tm.tm_min = minuto;
    tm.tm_sec = segundo;

    return mktime(&tm);
}

