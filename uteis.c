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


time_t stringParaTempo(const char* data){
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    int dia, mes, ano, hora, minuto, segundo, milissegundos;

    if (sscanf(data, "%d-%d-%d %d:%d:%d.%d", &dia, &mes, &ano, &hora, &minuto, &segundo, &milissegundos) != 7) {
        printf("Erro: formato de data inválido -> %s\n", data);
        return (time_t)-1;
    }

    int scanned_items = sscanf(data, "%d-%d-%d %d:%d:%d.%d", &dia, &mes, &ano, &hora, &minuto, &segundo, &milissegundos);
    printf("sscanf returned: %d for data: %s\n", scanned_items, data);

    // Verificações de validade
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1 || dia > 31 || hora < 0 || hora > 23 || minuto < 0 || minuto > 59 || segundo < 0 || segundo > 59 || milissegundos < 0 || milissegundos > 1000) {
        printf("Erro: valores de data ou hora fora do intervalo -> %s\n", data);
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


float calcularDiferencaHoras(const char* data1, const char* data2) {
    // Implementação simplificada - assumindo formato DD-MM-AAAA HH:MM:SS.mmm
    int dia1, mes1, ano1, hora1, min1, seg1;
    int dia2, mes2, ano2, hora2, min2, seg2;
    float mili1, mili2;

    sscanf(data1, "%d-%d-%d %d:%d:%f", &dia1, &mes1, &ano1, &hora1, &min1, &mili1);
    sscanf(data2, "%d-%d-%d %d:%d:%f", &dia2, &mes2, &ano2, &hora2, &min2, &mili2);

    seg1 = (int)mili1;
    seg2 = (int)mili2;
    mili1 = mili1 - seg1;
    mili2 = mili2 - seg2;

    // Cálculo simplificado - considerando apenas horas, minutos e segundos
    float total1 = hora1 * 3600 + min1 * 60 + seg1 + mili1;
    float total2 = hora2 * 3600 + min2 * 60 + seg2 + mili2;

    return (total2 - total1) / 3600.0; // Retorna diferença em horas
}


Veiculo* encontrarVeiculoPorCodigo(Veiculo* veiculos, int codVeiculo) {
    Veiculo* atual = veiculos;
    while (atual != NULL) {
        if (atual->codVeiculo == codVeiculo) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}


Dono* encontrarDonoPorContribuinte(Dono* donos, int numContribuinte) {
    Dono* atual = donos;
    while (atual != NULL) {
        if (atual->numContribuinte == numContribuinte) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}


float encontrarDistancia(Distancia* distancias, int sensor1, int sensor2) {
    Distancia* atual = distancias;
    while (atual != NULL) {
        if ((atual->codSensor1 == sensor1 && atual->codSensor2 == sensor2) ||
            (atual->codSensor1 == sensor2 && atual->codSensor2 == sensor1)) {
            return atual->distancia;
        }
        atual = atual->prox;
    }
    return -1.0; // Retorna -1 se não encontrar
}

int compararRankingVeiculos(const void* a, const void* b) {
    Infracao* ia = (Infracao*)a;
    Infracao* ib = (Infracao*)b;
    return ib->numInfracoes - ia->numInfracoes;
}

float encontrarDistanciaEntreSensores(Sensor* sensores, int idSensor1, int idSensor2) {
    // Implementação depende de como as distâncias estão armazenadas
    // Esta é uma versão simplificada - você precisará adaptar
    Sensor* s1 = sensores;
    while (s1 != NULL && s1->codSensor != idSensor1) {
        s1 = s1->prox;
    }

    Sensor* s2 = sensores;
    while (s2 != NULL && s2->codSensor != idSensor2) {
        s2 = s2->prox;
    }

    if (s1 != NULL && s2 != NULL) {
        // Cálculo simplificado da distância (usando coordenadas GPS)
        // Você deve substituir por sua implementação real
        return 10.0; // Valor fictício
    }
    return -1.0; // Indica que a distância não foi encontrada
}

int compararRankingMarcas(const void* a, const void* b) {
    RankingMarca* ra = (RankingMarca*)a;
    RankingMarca* rb = (RankingMarca*)b;
    if (ra->kmTotal > rb->kmTotal) return -1;
    if (ra->kmTotal < rb->kmTotal) return 1;
    return 0;
}

int compararInfracoes(const void* a, const void* b) {
    Infracao* ia = (Infracao*)a;
    Infracao* ib = (Infracao*)b;
    return strcmp(ia->matricula, ib->matricula);
}

Veiculo* encontrarVeiculoPorMatricula(Veiculo* veiculos, const char* matricula) {
    Veiculo* atual = veiculos;
    while (atual != NULL) {
        if (strcmp(atual->matricula, matricula) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

int compararVelocidadesMedias(const void* a, const void* b) {
    VelocidadeMedia* va = (VelocidadeMedia*)a;
    VelocidadeMedia* vb = (VelocidadeMedia*)b;
    if (va->velocidadeMedia > vb->velocidadeMedia) return -1;
    if (va->velocidadeMedia < vb->velocidadeMedia) return 1;
    return 0;
}

