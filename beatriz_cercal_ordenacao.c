/*
Disciplina: Estrutura de Dados;
Atividade: Comparacao de desempenho entre metodos de ordenacao;
Aluna: Beatriz Cercal Cachoeira 

Conseguiu chegar nos valores de referencia?
R: Sim! Os valores foram atingidos corretamente.
Se nao, em quais metodos e por que?
R: Atingi todos os valores!
Metodos analisados:
- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Teste anterior com as 8 cartas:

int cartas[8] = {6, 8, 5, 4, 2, 10, 1, 11};
int copia[8];

for (metodo = 0; metodo < 5; metodo++) {
    copiar(copia, cartas);
    zerar();
    metodos[metodo](copia, 8);
    printf("Resultado do metodo: ");
    for (i = 0; i < 8; i++) printf("%d ", copia[i]);
    printf("\n");
}

Resultado obtido:
1 2 4 5 6 8 10 11
*/

#define TAM 10000
#define METODOS 5
#define CENARIOS 3
#define RODADAS_ALEATORIO 3

typedef struct {
    unsigned long comp, trocas, mov;
    double tempo;
} Resultado;

typedef void (*Metodo)(int *, int);
Resultado r;

/* Zera os indicadores antes de cada ordenacao. */
void zerar(void) { r.comp = r.trocas = r.mov = 0; r.tempo = 0; }

/* Troca usada por Bubble e Selection: uma troca tem tres movimentacoes. */
void troca(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
    r.trocas++; r.mov += 3;
}

/* Troca interna do Quick, contada separadamente pelas regras do trabalho. */
void trocaQuick(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/*  BUBBLE SORT  */
void bubble(int *v, int n) {
    int i, passo = 1, mudou = 1, t;
    /* Percorre o vetor ate nao encontrar mais elementos fora de ordem. */
    while (passo <= n && mudou) {
        mudou = 0;
        for (i = 0; i < n - 1; i++) {
            r.comp++;
            if (v[i] > v[i + 1]) {
                t = v[i]; v[i] = v[i + 1]; v[i + 1] = t;
                r.trocas++; r.mov += 3; mudou = 1;
            }
        }
        passo++;
    }
}

/*  INSERTION SORT  */
void insertion(int *v, int n) {
    int i, j, chave;
    /* Insere cada elemento na posicao correta da parte ja ordenada. */
    for (i = 1; i < n; i++) {
        chave = v[i]; r.mov++; j = i - 1;
        while (j >= 0) {
            r.comp++;
            if (v[j] <= chave) break;
            v[j + 1] = v[j]; r.mov++; j--;
        }
        v[j + 1] = chave; r.mov++;
    }
}

/*  SELECTION SORT  */
void selection(int *v, int n) {
    int i, j, menor;
    /* Procura o menor elemento e coloca-o na proxima posicao. */
    for (i = 0; i < n - 1; i++) {
        menor = i;
        for (j = i + 1; j < n; j++) {
            r.comp++;
            if (v[j] < v[menor]) menor = j;
        }
        if (menor != i) troca(&v[i], &v[menor]);
    }
}

/*  MERGE SORT  */
/* Funcao auxiliar que intercala duas partes ordenadas. */
void merge(int *v, int ini, int meio, int fim) {
    int i = ini, j = meio + 1, k = 0, n = fim - ini + 1;
    /* Junta duas partes ordenadas em um vetor temporario. */
    int *temp = (int *)malloc(n * sizeof(int));
    if (!temp) exit(EXIT_FAILURE);
    while (i <= meio && j <= fim) {
        r.comp++;
        if (v[i] <= v[j]) temp[k++] = v[i++];
        else temp[k++] = v[j++];
        r.mov++;
    }
    while (i <= meio) { temp[k++] = v[i++]; r.mov++; }
    while (j <= fim) { temp[k++] = v[j++]; r.mov++; }
    for (i = ini, k = 0; i <= fim; i++, k++) { v[i] = temp[k]; r.mov++; }
    free(temp);
}

/* Funcao recursiva do Merge Sort. */
void mergeSort(int *v, int ini, int fim) {
    int meio;
    /* Divide o vetor e ordena as metades recursivamente. */
    if (ini < fim) {
        meio = (ini + fim) / 2;
        mergeSort(v, ini, meio);
        mergeSort(v, meio + 1, fim);
        merge(v, ini, meio, fim);
    }
}

/*  QUICK SORT  */
/* Funcao auxiliar de particionamento do Quick Sort. */
int particiona(int *v, int ini, int fim) {
    int i = ini - 1, j, pivo = v[fim];
    /* Usa o ultimo elemento como pivo, conforme o material de apoio. */
    for (j = ini; j < fim; j++) {
        r.comp++;
        if (v[j] < pivo) {
            i++;
            if (i != j) {
                r.trocas++;
                r.mov += 3;
            }
            trocaQuick(&v[i], &v[j]);
        }
    }
    r.mov++;
    if (i + 1 != fim) {
        r.trocas++;
        r.mov += 3;
    }
    trocaQuick(&v[i + 1], &v[fim]);
    return i + 1;
}

/* Funcao recursiva do Quick Sort. */
void quickSort(int *v, int ini, int fim) {
    int p;
    /* Ordena recursivamente os lados esquerdo e direito do pivo. */
    if (ini < fim) {
        p = particiona(v, ini, fim);
        quickSort(v, ini, p - 1);
        quickSort(v, p + 1, fim);
    }
}

void mergeMetodo(int *v, int n) { mergeSort(v, 0, n - 1); }
void quickMetodo(int *v, int n) {
    quickSort(v, 0, n - 1);
}

void gerarNumero(int *v, int op) {
    int i;
    /* op 1: crescente, op 2: decrescente, op 3: valores aleatorios. */
    for (i = 0; i < TAM; i++) {
        if (op == 1) v[i] = i + 1;
        else if (op == 2) v[i] = TAM - i;
        else v[i] = rand() % TAM;
    }
}

void copiar(int *dest, int *orig) {
    int i;
    /* Cada metodo recebe uma copia do mesmo vetor-base. */
    for (i = 0; i < TAM; i++) dest[i] = orig[i];
}

void executar(Metodo metodo, int *base, int *teste, Resultado *resultado) {
    clock_t inicio;
    /* A copia e os contadores ficam fora da medicao de tempo. */
    copiar(teste, base);
    zerar();
    inicio = clock();
    metodo(teste, TAM);
    r.tempo = 1000.0 * (clock() - inicio) / CLOCKS_PER_SEC;
    *resultado = r;
}

void imprimirTabela(const char *nome, const char *nomes[], Resultado resultados[]) {
    int i;
    printf("\n=============================================================\n");
    printf("%s - %d ELEMENTOS\n", nome, TAM);
    if (nome[8] == '3') printf("Media de 3 rodadas\n");
    printf("=============================================================\n");
    printf("%-12s %16s %12s %18s %12s\n", "Metodo", "Comparacoes", "Trocas", "Movimentacoes", "Tempo(ms)");
    printf("%-12s %16s %12s %18s %12s\n", "------------", "----------------", "------------", "------------------", "------------");
    for (i = 0; i < METODOS; i++)
        printf("%-12s %16lu %12lu %18lu %12.3f\n", nomes[i], resultados[i].comp, resultados[i].trocas, resultados[i].mov, resultados[i].tempo);
}

void imprimirEmpates(const char *nomes[], Resultado resultados[], int indicador) {
    int i, primeiro = 1;
    double menor;
    /* Encontra e imprime todos os metodos empatados no menor resultado. */
    menor = indicador == 0 ? (double)resultados[0].comp : indicador == 1 ? (double)resultados[0].mov : resultados[0].tempo;
    for (i = 1; i < METODOS; i++) {
        if ((indicador == 0 && resultados[i].comp < menor) ||
            (indicador == 1 && resultados[i].mov < menor) ||
            (indicador == 2 && resultados[i].tempo < menor))
            menor = indicador == 0 ? (double)resultados[i].comp : indicador == 1 ? (double)resultados[i].mov : resultados[i].tempo;
    }
    for (i = 0; i < METODOS; i++) {
        if ((indicador == 0 && resultados[i].comp == (unsigned long)menor) ||
            (indicador == 1 && resultados[i].mov == (unsigned long)menor) ||
            (indicador == 2 && resultados[i].tempo == menor)) {
            if (!primeiro) printf(" e ");
            printf("%s", nomes[i]);
            primeiro = 0;
        }
    }
}

int main(void) {
    const char *nomes[] = {"Bubble", "Selection", "Insertion", "Merge", "Quick"};
    const char *cenarios[] = {"CENARIO 1 - DADOS ORDENADOS", "CENARIO 2 - DADOS INVERTIDOS", "CENARIO 3 - DADOS ALEATORIOS"};
    Metodo metodos[] = {bubble, selection, insertion, mergeMetodo, quickMetodo};
    Resultado resultados[CENARIOS][METODOS];
    int *vetorBase = (int *)malloc(TAM * sizeof(int));
    int *vetorTeste = (int *)malloc(TAM * sizeof(int));
    int c, m, rodada, quantidade;
    Resultado parcial;

    if (!vetorBase || !vetorTeste) return EXIT_FAILURE;
    srand((unsigned)time(NULL));

    /* Os cenarios 1 e 2 rodam uma vez; o aleatorio roda tres vezes. */
    for (c = 0; c < CENARIOS; c++) {
        quantidade = c == 2 ? RODADAS_ALEATORIO : 1;
        for (m = 0; m < METODOS; m++) {
            resultados[c][m].comp = resultados[c][m].trocas = resultados[c][m].mov = 0;
            resultados[c][m].tempo = 0;
        }
        for (rodada = 0; rodada < quantidade; rodada++) {
            /* Todos os metodos desta rodada usam exatamente o mesmo vetor. */
            gerarNumero(vetorBase, c + 1);
            for (m = 0; m < METODOS; m++) {
                executar(metodos[m], vetorBase, vetorTeste, &parcial);
                resultados[c][m].comp += parcial.comp;
                resultados[c][m].trocas += parcial.trocas;
                resultados[c][m].mov += parcial.mov;
                resultados[c][m].tempo += parcial.tempo;
            }
        }
        for (m = 0; m < METODOS; m++) {
            /* Divide os resultados acumulados pela quantidade de rodadas. */
            resultados[c][m].comp /= quantidade;
            resultados[c][m].trocas /= quantidade;
            resultados[c][m].mov /= quantidade;
            resultados[c][m].tempo /= quantidade;
        }
        imprimirTabela(cenarios[c], nomes, resultados[c]);
    }

    printf("\n=============================================================\nRESUMO\n=============================================================\n");
    printf("%-14s | %-22s | %-22s | %-22s\n", "Cenario", "Menos comparacoes", "Menos movimentacoes", "Menor tempo");
    for (c = 0; c < CENARIOS; c++) {
        printf("%-14s | ", c == 0 ? "Ordenados" : c == 1 ? "Invertidos" : "Aleatorios");
        imprimirEmpates(nomes, resultados[c], 0); printf(" | ");
        imprimirEmpates(nomes, resultados[c], 1); printf(" | ");
        imprimirEmpates(nomes, resultados[c], 2); printf("\n");
    }
    free(vetorBase);
    free(vetorTeste);
    return 0;
}
