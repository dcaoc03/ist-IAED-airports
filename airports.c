/*
* Projeto 1 - IAED 2021/22
* Diogo Cabral Antunes de Oliveira Costa, ist1103179, LEIC-A
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Constantes */

#define LIM_AP 40     /* n maximo de aeroportos no sistema */
#define LIM_VOOS 30000     /* n maximo de voos no sistema */
#define LETRAS_ID 4     /* n de carateres numa string 'ID' */
#define LIM_PAIS 31     /* n maximo de carateres numa string 'pais' */
#define LIM_CIDADE 51     /* n maximo de carateres numa string 'cidade' */
#define DIG_COD 7     /* n de caracteres numa string 'codigo' */

#define LIM_BUFF 65535
#define DIG_COD_RES 10

/* Estruturas */

/* Estrutura do tipo Aeroporto: armazena os dados de um aeroporto */
typedef struct {
    char id[LETRAS_ID];     /* String ID: representa o ID unico do aeroporto */
    char pais[LIM_PAIS];     /* String pais: indica o pais onde se localiza o aeroporto */
    char cidade[LIM_CIDADE];     /* String pais: indica a cidade onde se localiza o aeroporto */
    int n_voos;     /* Inteiro n_voos: indica o numero de voos que partem desse aeroporto */
} Aeroporto;

/* Estrutura do tipo Horas: armazena informacoes relativas a uma dada hora */
typedef struct {
    int horas;     /* Inteiro horas: indica o numero de horas do tempo representado */
    int min;     /* Inteiro min: indica o numero de minutos do tempo representado */
} Horas;

/* Estrutura do tipo Data: aramazena informacoes relativas a uma dada data */
typedef struct {
    int dia;     /* Inteiro dia: indica o dia da data */
    int mes;     /* Inteiro mes: indica o numero do mes da data */
    int ano;     /* Inteiro ano: indica o ano da data */
} Data;

/* Estrutura do tipo Voo: armazena os dados de um voo */
typedef struct {
    char codigo[DIG_COD];     /* String codigo: representa o codigo do voo */
    char id_ap_p[LETRAS_ID];     /* String id_ap_p: indica o ID do aeroporto de partida */
    char id_ap_c[LETRAS_ID];     /* String id_ap_c: indica o ID do aeroporto de chegada */
    Data data_p;     /* Data data_p: indica a data de partida do voo */
    Horas hora_p;     /* Horas hora_p: indica a hora de partida do voo */
    Horas duracao;     /* Horas duracao: indica a duracao do voo */
    int capacidade;     /* inteiro capacidade: indica a lotacao do voo */
    int lugares_vagos;     /* inteiro lugares_vagos: indica o numero de lugares livres no voo */
} Voo;


/* Estruturas Projeto 2 */

/* Estrutura auto-referenciada do tipo Reserva: armazena os dados de uma reserva */
typedef struct reserva {
    char codigo_voo[DIG_COD];     /* String codigo_voo: representa o codigo do voo da reserva */
    Data data;     /* Data data: representa a data de partida do voo da reserva */
    char *codigo_res;     /* Ponteiro para char codigo_tres: representa o codigo unico da reserva, de tamanho variavel */
    int len_cod;     /* Inteiro len_cod: armazena o numero de char armazenados em codigo_res */
    int num_pass;     /* Inteiro len_cod: armazena o numero de passaegiros da reserva */
    struct reserva *prox;     /* Ponteiro para Reserva prox: ponteiro para a reserva seguinte, ordenando assim as reservas numa lista */
} Reserva;

/* Res_pont: Ponteiro para uma reserva */
typedef Reserva* Res_pont;

/* Prototipos projeto 1 */

int adiciona_aeroporto(Aeroporto lista_ap[], int indice);
void lista_aeroportos(Aeroporto lista_ap[], int indice);
int adiciona_voo(Voo lista_voos[], int indice_voo, Data data_atual, Aeroporto lista_ap[], int indice_ap);
void partidas_chegadas(Voo lista_voos[], int indice_voo, Aeroporto lista_ap[], int indice_ap, char c);
Data avanca_data(Data data_atual);

/* Prototipos projeto 2 */

int trata_reservas(Voo lista_voos[], int i_voos, Data data, Res_pont* lista_reservas);
Res_pont elimina_voo_res(Voo lista_voos[], int* i_voos, Res_pont lista_reservas);
void free_reservas(Res_pont lista_reservas);


/* Funcoes gerais (utilizadas ao longo de todo o codigo) */

/* copia_ap: Recebe um aeroporto e retorna uma copia exata desse aeroporto,
copiando-o parametro a parametro. */
Aeroporto copia_ap(Aeroporto ap) {
    Aeroporto novo_ap;
    int i;
    for (i=0; i < LETRAS_ID; i++)
        novo_ap.id[i] = ap.id[i];
    i=0;
    while (ap.pais[i] != '\0') {
        novo_ap.pais[i] = ap.pais[i];
        i++;
    }
    novo_ap.pais[i] = '\0';
    i=0;
    while (ap.cidade[i] != '\0') {
        novo_ap.cidade[i] = ap.cidade[i];
        i++;
    }
    novo_ap.cidade[i] = '\0';
    novo_ap.n_voos = ap.n_voos;
    return novo_ap;
}

/* copia_voo: Recebe um voo e retorna uma copia exata desse voo,
   copiando-o parametro a parametro. */
Voo copia_voo(Voo v) {
    Voo novo_v;
    int i=0;
    strcpy(novo_v.codigo, v.codigo);
    for (i=0; i < LETRAS_ID; i++) {
        novo_v.id_ap_p[i] = v.id_ap_p[i];
        novo_v.id_ap_c[i] = v.id_ap_c[i];
    }
    novo_v.data_p.dia = v.data_p.dia;
    novo_v.data_p.mes = v.data_p.mes;
    novo_v.data_p.ano = v.data_p.ano;
    novo_v.hora_p.horas = v.hora_p.horas;
    novo_v.hora_p.min = v.hora_p.min;
    novo_v.duracao.horas = v.duracao.horas;
    novo_v.duracao.min = v.duracao.min;
    novo_v.capacidade = v.capacidade;
    novo_v.lugares_vagos = v.lugares_vagos;
    return novo_v;
}

/* data_anterior_posterior: recebe duas Datas d1 e d2 e retorna um de tres valores:
   -1 se d1 corresponder a uma data anterior a d2, 0 se d1 e d2 corresponderem a
   mesma data e 1 se d1 corresponde a uma data posterior a d2. */
int data_anterior_posterior(Data d1, Data d2) {
    if ((d1.dia == d2.dia) && (d1.mes == d2.mes) && (d1.ano == d2.ano))
        return 0;
    else if ((d1.ano < d2.ano) || ((d1.ano == d2.ano) && (d1.mes < d2.mes)) || ((d1.ano == d2.ano) && (d1.mes == d2.mes) && (d1.dia < d2.dia)))
        return -1;
    else
        return 1;
}

/* hora_anterior_posterior: recebe duas Horas h1 e d2 e retorna um de tres valores:
   -1 se h1 corresponder a uma hora anterior a h2, 0 se h1 e h2 corresponderem a
   mesma hora e 1 se h1 corresponde a uma hora posterior a h2. */
int hora_anterior_posterior(Horas h1, Horas h2) {
    if ((h1.horas == h2.horas) && (h1.min == h2.min))
        return 0;
    else if ((h1.horas < h2.horas) || ((h1.horas == h2.horas) && (h1.min < h2.min))) 
        return -1;
    else
        return 1;
}

/* eh_data_valida: recebe a data do sistema e outra data d1 e retorna 1 se e so se
   d1 for uma data valida, caso contrario retorna 0. Para ser valida, d1 tem de ser
   igual ou posterior a data do sistema e anterior ou igual a data limite, que e a
   mesma data que a data do sistema, mas no ano seguinte */
int eh_data_valida(Data data_atual, Data d1) {
    Data data_lim;
    data_lim.dia = data_atual.dia;
    data_lim.mes = data_atual.mes;
    data_lim.ano = data_atual.ano + 1;
    if ((data_anterior_posterior(d1, data_atual) >= 0) && (data_anterior_posterior(d1, data_lim) <= 0))
        return 1;
    return 0;
}

/* FUNCAO MAIN */

/* Funcao main: controla os comandos colocados no input. */
int main() {
    char comando;   /* controla os carateres de comando */
    int indice_ap=0, indice_voos=0;     /* controlam os tamanhos das listas de aeroportos e voos */
    Aeroporto lista_ap[LIM_AP];     /* lista de todos os aeroportos */
    Voo lista_voos[LIM_VOOS];     /* lista de todos os voos */
    Data data_atual={1, 1, 2022};     /* inicializa a data do sistema */  
    Res_pont lista_reservas=NULL;
    comando = getchar();
    while (comando != 'q') {     /* o programa para quando e usado o comando 'q' */
        if (comando == 'a') 
            indice_ap += adiciona_aeroporto(lista_ap, indice_ap);
        if (comando == 'l')
            lista_aeroportos(lista_ap, indice_ap); 
        if (comando == 'v')
            indice_voos += adiciona_voo(lista_voos, indice_voos, data_atual, lista_ap, indice_ap);
        if ((comando == 'p') || (comando == 'c'))
            partidas_chegadas(lista_voos, indice_voos, lista_ap, indice_ap, comando);
        if (comando == 't')
            data_atual = avanca_data(data_atual);
        if (comando == 'r') {
            if (trata_reservas(lista_voos, indice_voos, data_atual, &lista_reservas) < 0) {
                printf("No memory.\n");
                break;
            }
           /* if (lista_reservas == NULL) {
                free(lista_reservas);
                exit(0);
            } */
        }
        if (comando == 'e')
            lista_reservas = elimina_voo_res(lista_voos, &indice_voos, lista_reservas);
        comando = getchar();
    }
    free_reservas(lista_reservas);
    return 0;
}


/* Funcoes do comando 'a' */

/* verifica_erros: recebe um array de aeroportos, o seu tamanho e um novo
   aeroporto a adicionar, verifica se o ID do novo aeroporto e valido, ou
   seja composto por 3 letras maiusculas e unico, e que o numero de aeroportos
   na lista nao excede o limite. Se ocorreu um desses erros, retorna 1, caso
   contrario retorna 0. Se ocorrer mais de um erro, apenas e apresentado o
   primeiro. */
int verifica_erros(Aeroporto lista_ap[], Aeroporto novo_ap, int indice) {
    int i;
    for (i=0; i < LETRAS_ID - 1; i++) 
        if ((novo_ap.id[i] < 'A') || (novo_ap.id[i] > 'Z')) {
            printf("invalid airport ID\n");
            return 0;
        }
    if (indice >= LIM_AP) {
        printf("too many airports\n");
        return 0;
    }
    for (i=0; i < indice; i++)
        if (!strcmp(lista_ap[i].id, novo_ap.id)) {
            printf("duplicate airport\n");
            return 0;
        }
    return 1;
}

/* adiciona_aeroporto: recebe um array de aeroportos e o seu tamanho, le o
   input e adiciona um aeroporto a esse array com as informacoes do input.
   Da print da expressao 'airport' seguida do ID do aeroporto adicionado e
   retorna 1, indicando que o tamanho do array foi incrementado em 1 unidade
   se nao tiver ocorrido qualquer erro. */
int adiciona_aeroporto(Aeroporto lista_ap[], int indice) {
    Aeroporto novo_ap;
    char carater;
    int indice_cidade=0, exec;      /*variavel exec: controla a execucao do programa, 
    0 se ocorreu um erro, 1 se esta tudo normal; a variavel exec e utilizada varias
    vezes ao longo de todo o codigo com o mesmo objetivo. */
    scanf("%s%s", novo_ap.id, novo_ap.pais);
    carater = getchar();
    while ((carater == ' ') || (carater == '\t'))
        carater = getchar();      /* avanca carateres brancos entre o pais e a cidade */
    while (carater != '\n') {
        novo_ap.cidade[indice_cidade] = carater;
        indice_cidade++;
        carater = getchar();
    }
    novo_ap.cidade[indice_cidade] = '\0';
    novo_ap.n_voos = 0;
    exec = verifica_erros(lista_ap, novo_ap, indice);
    if (exec) {
        lista_ap[indice] = copia_ap(novo_ap);
        printf("airport %s\n", novo_ap.id);
        return 1;
    }
    return 0;
}



/* Funcoes do comando 'l' */

/* ordena_ap: recebe um array de aeroportos e o seu tamanho, modifica
   destrutivamente o array dado segundo a ordem alfabetica dos ID's dos
   seus constituintes */
void ordena_ap(Aeroporto ap_ordenado[], int indice) {
    Aeroporto aux;
    int i_ap=0;
    while (i_ap < (indice - 1)) {
        if (strcmp(ap_ordenado[i_ap].id, ap_ordenado[i_ap + 1].id) > 0) {
            aux = copia_ap(ap_ordenado[i_ap + 1]);
            ap_ordenado[i_ap + 1] = copia_ap(ap_ordenado[i_ap]);
            ap_ordenado[i_ap] = copia_ap(aux);
            i_ap = 0;
        }
        else 
            i_ap++;
    }
}

/* lista_aeroportos: recebe um array aeroprto e o seu tamanho, podendo
   comportar-se de duas maneiras diferentes: se nao tiver nenhum parametro,
   lista todos os aeroportos do sistema por ordem alfabetica dos seus ID's;
   se for invocado com uma lista de ID's de aeroportos, sao listados apenas
   os aeroportos indicados pela ordem pela qual foram invocados */
void lista_aeroportos(Aeroporto lista_ap[], int indice) {
    int i, exec;
    char c, id_ap[LETRAS_ID];
    Aeroporto ap_ord[LIM_AP];
    c = getchar();
    if (c == '\n') {
        for (i=0; i < indice; i++)
            ap_ord[i] = copia_ap(lista_ap[i]);
        ordena_ap(ap_ord, indice);
        for (i=0; i < indice; i++) 
            printf("%s %s %s %d\n", ap_ord[i].id, ap_ord[i].cidade, ap_ord[i].pais, ap_ord[i].n_voos);        
    } 
    else {
        while ((c != '\n') && (c != EOF)) {
            exec = 0;
            scanf("%s", id_ap);
            for (i=0; (i < indice) && (!exec); i++) {
                if (!strcmp(lista_ap[i].id, id_ap)) {
                    printf("%s %s %s %d\n", lista_ap[i].id, lista_ap[i].cidade, lista_ap[i].pais, lista_ap[i].n_voos);
                    exec = 1;
                }
            }
            if (!exec)
                printf("%s: no such airport ID\n", id_ap);
            c = getchar();
        }
    } 
}



/* Funcoes do comando 'v' */

/* verifica_erros_voo: recebe um array de voos, um voo novo, o tamanho do array,
   a data do sistema, um array de aeroportos e o tamanho desse array, retorna 1
   ou 0, consoante ocorreu um erro ou nao. O voo tem de ter um codigo comporto
   por duas letras maiusculas seguidas de um numero entre 1 e 9999, unico para
   o dia do voo, tem de partir e chegar a um aeroporto que existe, tem de ter
   data, duracao e capacidade validas e o numero de voos definido para o sistema
   nao pode ser excedido. Se ocorrer mais de um erro, apenas e apresentado o
   primeiro. */
int verifica_erros_voo(Voo lista_voos[], Voo novo_voo, int indice_voo, Data data_atual, Aeroporto lista_ap[], int indice_ap) {
    int i=0, i_cod=0, part=0, cheg=0;
    while ((novo_voo.codigo[i_cod] != '\0') && (i_cod < DIG_COD)) {
        if ((i_cod < 2) && ((novo_voo.codigo[i_cod] < 'A') || (novo_voo.codigo[i_cod] > 'Z'))) {
            printf("invalid flight code\n");
            return 0;
        }
        else if ((i_cod >= 2) && ((novo_voo.codigo[i_cod] < '0') || (novo_voo.codigo[i_cod] > '9'))) {
            printf("invalid flight code\n");
            return 0;
        }
        i_cod++;
    } 
    if (i_cod < 3) {
        printf("invalid flight code\n");
        return 0;
    }
    for (i=0; i < indice_voo; i++) {
        if (!strcmp(novo_voo.codigo, lista_voos[i].codigo) && !data_anterior_posterior(novo_voo.data_p, lista_voos[i].data_p)) {
            printf("flight already exists\n");
            return 0;
        }
    }
    for (i=0; i < indice_ap; i++) {
        if (!strcmp(novo_voo.id_ap_p, lista_ap[i].id)) {
            part++;
            lista_ap[i].n_voos++;
        }
        if (!strcmp(novo_voo.id_ap_c, lista_ap[i].id))
            cheg++;
    }
    if (part != 1) {
        printf("%s: no such airport ID\n", novo_voo.id_ap_p);
        return 0;
    }
    if (cheg != 1) {
        printf("%s: no such airport ID\n", novo_voo.id_ap_c);
        return 0; 
    } 
    if (indice_voo >= LIM_VOOS) {
        printf("too many flights\n");
        return 0;
    }
    if (!eh_data_valida(data_atual, novo_voo.data_p)) {
        printf("invalid date\n");
        return 0;
    }
    if ((novo_voo.duracao.horas > 12) || (novo_voo.duracao.horas == 12 && novo_voo.duracao.min > 0)) {
        printf("invalid duration\n");
        return 0;
    }
    if ((novo_voo.capacidade > 100) || (novo_voo.capacidade < 10)) {
        printf("invalid capacity\n");
        return 0;
    }
    return 1;
}

/* adiciona_voo: recebe um array de voos e o seu tamanho, um array de aeroportos e o
   seu tamanho e a data atual do sistema, pode comportar-se de duas maneiras diferentes:
   se nao tiver mais parametros, lista pela ordem de criacao os voos ja registados
   no sistema; se tiver mais parametros, adiciona o voo com as caracteristicas ai
   indicadas ao sistema caso nao tenha ocorrido nenhum erro, retornando 1, indicando
   que a lista de voos foi incrementada em uma unidade, caso contrario retorna 0. */
int adiciona_voo(Voo lista_voos[], int indice_voo, Data data_atual, Aeroporto lista_ap[], int indice_ap) {
    char c;
    int i, exec;
    Voo novo_voo;
    c = getchar();
    if (c == '\n') {
        for (i=0; i < indice_voo; i++) {
            printf("%s ", lista_voos[i].codigo);
            printf("%s %s ", lista_voos[i].id_ap_p, lista_voos[i].id_ap_c);
            printf("%.2d-%.2d-%.4d ", lista_voos[i].data_p.dia, lista_voos[i].data_p.mes, lista_voos[i].data_p.ano);
            printf("%.2d:%.2d\n", lista_voos[i].hora_p.horas, lista_voos[i].hora_p.min);
        }
        return 0;
    }
    else {
        scanf("%s", novo_voo.codigo);
        scanf("%s %s", novo_voo.id_ap_p, novo_voo.id_ap_c);
        scanf("%d-%d-%d", &novo_voo.data_p.dia, &novo_voo.data_p.mes, &novo_voo.data_p.ano);
        scanf("%d:%d %d:%d", &novo_voo.hora_p.horas, &novo_voo.hora_p.min, &novo_voo.duracao.horas, &novo_voo.duracao.min);
        scanf("%d", &novo_voo.capacidade);
        novo_voo.lugares_vagos = novo_voo.capacidade;
        exec = verifica_erros_voo(lista_voos, novo_voo, indice_voo, data_atual, lista_ap, indice_ap);
        if (exec) {
            lista_voos[indice_voo] = copia_voo(novo_voo);
            return 1;
        }
        else
            return 0;
    }
}


/* Funcoes do comando 'p' e 'c' */

/* tempos_chegada: recebe um voo e modifica-o destrutivamente, somando a sua hora
   de partida a sua duracao, a hora de chegada. A funcao tambem corrige em caso de 
   o voo aterrar no dia a seguir ao partir, corrigindo o dia. */
Voo tempos_chegada(Voo v) {
    v.hora_p.min += v.duracao.min;
    v.hora_p.horas += v.duracao.horas;
    while (v.hora_p.min >= 60) {
        v.hora_p.horas++;
        v.hora_p.min -= 60;
    }
    while (v.hora_p.horas >= 24) {
        v.data_p.dia++; 
        v.hora_p.horas -= 24;
    }
    if ((v.data_p.mes == 2) && (v.data_p.dia > 28)) {
        v.data_p.mes++;
        v.data_p.dia -= 28; 
    }
    else if (((v.data_p.mes == 4) || (v.data_p.mes == 6) || (v.data_p.mes == 9) || (v.data_p.mes == 11)) && (v.data_p.dia > 30)) {
        v.data_p.mes++;
        v.data_p.dia -= 30; 
    }
    else if (v.data_p.dia > 31) {
        if (v.data_p.mes == 12) {
            v.data_p.ano++;
            v.data_p.mes = 1;
        }
        else
            v.data_p.mes++;

        v.data_p.dia -= 31;
    }
    return v;
}

/* ordena_voos: recebe um array de voos e o seu tamanho, modificando-o
   destrutivamente de modo a que os voos fiquem ordenados por ordem de
   acordo com as suas datas e horas de partida/chegada */
void ordena_voos(Voo lista_voos[], int indice) {
    Voo aux;
    int i=0;
    int comparacao;
    while (i < (indice - 1)) {
        comparacao = data_anterior_posterior(lista_voos[i].data_p, lista_voos[i+1].data_p);
        if ((comparacao > 0) ||
            (comparacao == 0 && hora_anterior_posterior(lista_voos[i].hora_p, lista_voos[i+1].hora_p) > 0)) {
            aux = copia_voo(lista_voos[i+1]);
            lista_voos[i+1] = copia_voo(lista_voos[i]);
            lista_voos[i] = copia_voo(aux);
            i = 0;
        }
        else
            i++;
    }
}


/* partidas_chegadas: recebe um array de voos e o seu tamanho, um array de
   aeroportos e o seu tamanho e um carater c, igual a 'p' ou 'c'. Se c for 'p',
   lista todos os voos com origem no aeroporto cujo ID foi colocado a frente do
   comando por ordem de partida, do mais antigo para o mais recente. Se c for
   'c', sao listados os voos com chegada ao aeroporto cujo ID foi colocado a
   frente do comando por ordem de chegada, do mais antigo para o mais recente. */
void partidas_chegadas(Voo lista_voos[], int indice_voo, Aeroporto lista_ap[], int indice_ap, char c) {
    int i_ap, i_voo, i_voo_or=0, exec=0;
    char id[LETRAS_ID];
    Voo lista_or_voos[LIM_VOOS];
    scanf("%s", id);
    for (i_ap=0; (i_ap < indice_ap) && (exec != 1); i_ap++)
        if (!strcmp(id, lista_ap[i_ap].id)) 
            exec = 1;
    if (!exec)
        printf("%s: no such airport ID\n", id);
    else {
        if (c == 'p') {
            for (i_voo=0; i_voo < indice_voo; i_voo++) {
                if (!strcmp(id, lista_voos[i_voo].id_ap_p)) {
                    lista_or_voos[i_voo_or] = copia_voo(lista_voos[i_voo]);
                    i_voo_or++;
                }
            } 
        }
        else if (c == 'c') {
            for (i_voo=0; i_voo < indice_voo; i_voo++) {
                if (!strcmp(id, lista_voos[i_voo].id_ap_c)) {
                    lista_or_voos[i_voo_or] = tempos_chegada(lista_voos[i_voo]);
                    i_voo_or++;
                }
            } 
        }
        ordena_voos(lista_or_voos, i_voo_or);
        for (i_voo=0; i_voo < i_voo_or; i_voo++) {
            if (c == 'p')
                printf("%s %s ", lista_or_voos[i_voo].codigo, lista_or_voos[i_voo].id_ap_c);
            if (c == 'c')
                printf("%s %s ", lista_or_voos[i_voo].codigo, lista_or_voos[i_voo].id_ap_p);
            printf("%.2d-%.2d-%.4d ", lista_or_voos[i_voo].data_p.dia, lista_or_voos[i_voo].data_p.mes, lista_or_voos[i_voo].data_p.ano);
            printf("%.2d:%.2d\n", lista_or_voos[i_voo].hora_p.horas, lista_or_voos[i_voo].hora_p.min);
        }
    }
}


/* Funcoes do comando 't' */

/* le_data: le o input sob a forma DD-MM-AAAA e retorna a data lida correspondente
   sob a forma de uma estrutura Data. */
Data le_data() {
    char c;
    int i=0, data[3], num=0;
    Data nova_data;
    getchar();
    while ((c = getchar()) != '\n') {
        if ((c >= '0') && (c <= '9')) 
            num = num*10 + (c - '0');
        else if (c == '-') {
            data[i] = num;
            num = 0;
            i++;
        }
    }
    data[i] = num;
    nova_data.dia = data[0];
    nova_data.mes = data[1];
    nova_data.ano = data[2];
    return nova_data;
}

/* verifica_data: recebe a data atual e uma nova candidata a data atual,
   retorna 1 caso seja uma data valida, 0 em caso contrario. Para a nova data
   ser valida, nao pode ser antes da data atual, nem mais de um ano no futuro,
   nem pode ser de alem de 2023. */
int verifica_data(Data data_atual, Data nova_data) {
    if (eh_data_valida(data_atual, nova_data) && (nova_data.ano < 2024))
        return 1;
    else
        return 0; 
}

/* avanca_data: recebe a data_atual e altera-a para o valor inserido no input
   se essa nova data for valida (se exec=1), dando print a data inserida. */
Data avanca_data(Data data_atual) {
    int exec;
    Data nova_data;
    nova_data = le_data();
    exec = verifica_data(data_atual, nova_data);
    if (exec) {
        printf("%.2d-%.2d-%.4d\n", nova_data.dia, nova_data.mes, nova_data.ano);
        return nova_data;
    }
    else {
        printf("invalid date\n");
        return data_atual;
    }
}




/* PROJETO 2 */

/* Funcoes do comando 'r' */

/* 1- Funcoes de controlo */

/* existe_voo: recebe um array de voos, o seu tamanho, a data da reserva a criar e
   o codigo do voo dessa reserva e retorna o indice desse voo no array de voos se este
   existir, caso contrario retorna -1. No fundo, verifica se o voo para o qual se esta
   a fazer uma resrva existe. */
int existe_voo(Voo lista_voos[], int i_voos, Data data_res, char cod_voo[]) {
    int i;
    for (i=0; i < i_voos; i++) {
        if (!strcmp(lista_voos[i].codigo, cod_voo) &&
        !data_anterior_posterior(lista_voos[i].data_p, data_res))
            return i;
    }
    return -1;
}

/* erros_reservas: recebe uma lista de reservas, uma nova reserva, a data do sistema,
   o indice do voo da reserva e a nova capacidade desse voo e verifica se ha algum
   erro associado a criacao da nova reserva, retornando 0 se houver e 1 se esta tudo
   bem. Todas as resrvas devem cumprir os seguintes criterios:
   o seu codigo deve ter pelo menos 10 carateres e ser constituido exclusivamente
   por maiusculas e numeros. o voo tem que existir, a reserva nao pode fazer exceder
   a capacidade do voo, a data tem de ser valida e o numero de passageiros da reserva
   tem de ser um inteiro positivo. */
int erros_reservas(Res_pont lista_reservas, Res_pont nova_res, int comp_datas, int n_voo, Voo lista_voos[]) {
    int i, nova_cap;
    Res_pont i_pont;
    if (nova_res->len_cod < DIG_COD_RES) {
        printf("invalid reservation code\n");
        return 0;
    }
    for (i=0; i < (nova_res->len_cod); i++) {
        if (!((nova_res->codigo_res)[i] >= 'A' && (nova_res->codigo_res)[i] <= 'Z') && !((nova_res->codigo_res)[i] >= '0' && (nova_res->codigo_res)[i] <= '9')) {
            printf("invalid reservation code\n");
            return 0;
        }
    }
    if (n_voo < 0) {
        printf("%s: flight does not exist\n", nova_res->codigo_voo);
        return 0;
    }
    for (i_pont=lista_reservas; i_pont != NULL; i_pont = i_pont->prox) {
        if (!strcmp(i_pont->codigo_res, nova_res->codigo_res)) {
            printf("%s: flight reservation already used\n", nova_res->codigo_res);
            return 0;
        }
    }
    nova_cap = lista_voos[n_voo].lugares_vagos - nova_res->num_pass;
    if (nova_cap < 0) {
        printf("too many reservations\n");
        return 0;
    }
    if (!comp_datas) {
        printf("invalid date\n");
        return 0;
    }
    if (nova_res->num_pass <= 0) {
        printf("invalid passenger number\n");
        return 0;
    }
    lista_voos[n_voo].lugares_vagos = nova_cap;
    return 1;
}

/* 2- Funcoes de execucao */

/* cria_reserva: recebe um codigo do voo onde se vai realizar uma reserva
   e a data desse voo, de seguida recebe do input o codigo da reserva (uma
   string sem limite maximo de carateres, alocando apenas a memoria necessaria)
   e o numero de passageiros associados a reserva. Retorna um ponteiro para
   a noa reserva criada. Se nao houver mais memoria, retorna NULL */
Res_pont cria_reserva(char cod_voo[], Data data_res) {
    Res_pont nova_res;
    char buffer[LIM_BUFF];
    int len;
    nova_res = (Res_pont) malloc(sizeof(Reserva));
    if (nova_res != NULL) {
        strcpy(nova_res->codigo_voo, cod_voo);
        nova_res->data.dia = data_res.dia;
        nova_res->data.mes = data_res.mes;
        nova_res->data.ano = data_res.ano;
        scanf("%s", buffer);
        len = strlen(buffer);
        nova_res->codigo_res = (char *) malloc(sizeof(char) * (len + 1));
        if (nova_res->codigo_res != NULL) {
            strcpy(nova_res->codigo_res, buffer);
            nova_res->len_cod = len;
            scanf("%d", &nova_res->num_pass);
            nova_res->prox = NULL;
        }
        else {
            free(nova_res);
            nova_res = NULL;
        }
    }
    return nova_res;
}

/* insere_ordenado: recebe um ponteiro para o primeiro elemento da lista 
   de reservas e a nova reserva a inserir, inserindo a nova reserva na
   lista segundo a ordem alfabetica dos codigos das reservas. */
void insere_ordenado(Res_pont *lista_reservas, Res_pont nova_res) {
    Res_pont ind;
    if (*lista_reservas == NULL) {
        *lista_reservas = nova_res;
        return;
    }
    for (ind = *lista_reservas; (ind->prox != NULL) && (strcmp(nova_res->codigo_res, (ind->prox)->codigo_res) > 0); ind = ind->prox);     /* percorre a lista ate chegar a posicao certa */
    if (ind == *lista_reservas) {
        if (strcmp(nova_res->codigo_res, ind->codigo_res) < 0) {
            nova_res->prox = ind;
            *lista_reservas = nova_res;
        }
        else {
            nova_res->prox = ind->prox;
            ind->prox = nova_res; 
        }
    }
    else {
        nova_res->prox = ind->prox;
        ind->prox = nova_res;
    }
}

/* print_reservas: recebe o primeiro elemento da lista de reservas, o inteiro
   n_voo, correspondente ao indice do voo da reserva na lista de voos, o codigo do voo,
   um inteiro comp_datas e a data do voo. Caso n_voo seja negativo ou comp_datas igual
   a 0, dah print das correspondentes mensagens de erro e sai, caso contrario dah
   print a todas as reservas pertencentes ao voo indicado.  */
void print_reservas(Res_pont lista_reservas, int n_voo, char cod_voo[], int comp_datas, Voo lista_voos[]) {
    Res_pont p;
    if (n_voo < 0) {
        printf("%s: flight does not exist\n", cod_voo);
        return;
    }
    if (!comp_datas) {
        printf("invalid date\n");
        return;
    }
    for (p=lista_reservas; p != NULL; p=p->prox) 
        if (!strcmp(p->codigo_voo, cod_voo) && !data_anterior_posterior(p->data, lista_voos[n_voo].data_p))
          printf("%s %d\n", p->codigo_res, p->num_pass);
}

/* trata_reservas: funcao controlo do comando 'r'. Recebe a lista dos vooos e
   o seu tamanho, a data atual do sistema e a lista das reservas. Le o input
   pelo codigo do voo e pela sua data, podendo depois comportar-se de duas
   maneiras: se nao tiver mais parametros, lista as reservas do voo indicado;
   caso contrario adiciona uma nova reserva desse voo a lista se este nao
   provocar quaisquer erros. */
int trata_reservas(Voo lista_voos[], int i_voos, Data data_atual, Res_pont* lista_reservas) {
    char c, cod_voo[DIG_COD];
    int exec, n_voo, comp_datas;
    Data data_res;
    Res_pont nova_res;
    scanf("%s", cod_voo);
    scanf("%d-%d-%d", &data_res.dia, &data_res.mes, &data_res.ano);
    n_voo = existe_voo(lista_voos, i_voos, data_res, cod_voo);    /* verifica se o voo com o codigo dado existe na data dada */
    comp_datas = eh_data_valida(data_atual, data_res);    /* verifica se a data do voo eh valida */
    c = getchar();
    if (c == '\n')
        print_reservas(*lista_reservas, n_voo, cod_voo, comp_datas, lista_voos);
    else {
        nova_res = cria_reserva(cod_voo, data_res);
        if (nova_res == NULL) 
            return -1;    /* Se nao for possivel alocar memoria */
        exec = erros_reservas(*lista_reservas, nova_res, comp_datas, n_voo, lista_voos);
        if (exec) 
            insere_ordenado(lista_reservas, nova_res);
        else {
            free(nova_res->codigo_res);
            free(nova_res);
        }
    }
    return 1;     /* Correu tudo bem */
}


/* Funcoes do comando 'e' */

Res_pont elimina_reserva(Res_pont lista_reservas, char* codigo, char modo, Voo lista_voos[], int i_voos) {
    Res_pont p_res = lista_reservas, p_res2 = NULL;
    int exec=0, n_voo;
    char* v_ou_r;
    while (p_res != NULL) {
        if (modo == 'r')
            v_ou_r = p_res->codigo_res;
        else
            v_ou_r = p_res->codigo_voo;
        if (!strcmp(v_ou_r, codigo)) {
            if ((n_voo = existe_voo(lista_voos, i_voos, p_res->data, p_res->codigo_voo)) < 0)
                break;
            lista_voos[n_voo].lugares_vagos += p_res->num_pass;
            if (p_res == lista_reservas) {
                lista_reservas = p_res->prox;
                free(p_res->codigo_res);
                free(p_res);
                p_res = lista_reservas;
            }
            else {
                p_res2->prox = p_res->prox;
                free(p_res->codigo_res);
                free(p_res);
                p_res = p_res2->prox;
            }
            exec = 1;
            if (modo == 'r')
                break;
        }
        else {
            p_res2 = p_res;
            p_res = p_res->prox;
        }
    }
    if (!exec && (modo == 'r'))
        printf("not found\n");
    return lista_reservas;
}

void elimina_voo(Voo lista_voos[], int* i_voos, char* codigo) {
    int i=0, n_ocor=0, exec=0;
    while (i < *i_voos) {       
        while (!strcmp(lista_voos[i+n_ocor].codigo, codigo)) {
            n_ocor++;
            (*i_voos)--;
            exec = 1;
        }
        lista_voos[i] = copia_voo(lista_voos[i+n_ocor]);
        i++;
    }
    if (!exec)
        printf("not found\n");
}

Res_pont elimina_voo_res(Voo lista_voos[], int* i_voos, Res_pont lista_reservas) {
    char* codigo;
    char buffer[LIM_BUFF];
    int len;
    scanf("%s", buffer);
    len = strlen(buffer);
    codigo = (char*) malloc(sizeof(char)*(len + 1));
    strcpy(codigo, buffer);
    if (len >= DIG_COD_RES)
        lista_reservas = elimina_reserva(lista_reservas, codigo, 'r', lista_voos, *i_voos);
    else if (len <= (DIG_COD - 1)) {
        lista_reservas = elimina_reserva(lista_reservas, codigo, 'v', lista_voos, *i_voos);
        elimina_voo(lista_voos, i_voos, codigo);
    }
    else
        printf("not found\n");
    free(codigo);
    return lista_reservas;
}

/* Libertacao da memoria alocada */

void free_reservas(Res_pont lista_reservas) {
    Res_pont i=lista_reservas, temp;
    while (i != NULL) {
        temp = i;
        i = i->prox;
        free(temp->codigo_res);
        free(temp);
    }
}