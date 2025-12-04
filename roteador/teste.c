#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** valores;
    int quantVal;
    int prioridade;
} pacote;

typedef struct {
    pacote* packges;
    int maxAgrupa;
} lista;

void trocar(pacote* a, pacote* b) {
    pacote temp = *a;
    *a = *b;
    *b = temp;
}

int esquerdo(int i) { return (2 * i) + 1; }
int direito(int i) { return (2 * i) + 2; }

void heapify(pacote* V, int T, int i) {
        int menor = i;
        int E = esquerdo(i);
        int D = direito(i);

        if (E < T && V[E].prioridade < V[menor].prioridade)
            menor = E;
        if (D < T && V[D].prioridade < V[menor].prioridade)
            menor = D;

        if (menor != i){
        trocar(&V[i], &V[menor]);
        heapify(V, T, menor);
	}

}

void construir_heap(pacote* pack, int x) {
    int a;
    for (a = (x / 2) - 1; a >= 0; a--) {
        heapify(pack, x, a);
    }
}

void heapOrd(pacote* pack, int x) {
	printf("%d",x);
    if (x <= 1)
        return;
    construir_heap(pack, x);
    int i;
    for (i = x - 1; i > 0; i--) {
        trocar(&pack[0], &pack[i]);
        heapify(pack, i, 0);
    }
}

pacote* criaPacotes(FILE* arquivo, int x) {
    int n, q, i, j;
    char l[3];
    pacote* a = (pacote*) calloc(x, sizeof(pacote));

    for (i = 0; i < x; i++) {
        if (fscanf(arquivo, "%d", &q) != 1) q = 0;
        if (q > 63){
			q=63;
		}
        a[i].prioridade = q;

        if (fscanf(arquivo, "%d", &n) != 1) n = 0;
        a[i].quantVal = n;

        a[i].valores = (char**) malloc(n * sizeof(char*));

        for (j = 0; j < n; j++) {
            if (fscanf(arquivo, "%s", l) != 1) l[0] = '\0';
            a[i].valores[j] = malloc(strlen(l) + 1);
            strcpy(a[i].valores[j], l);
        }
    }

    return a;
}

void imprimirPacotes(FILE* out, pacote* pack, int tam) {
    if (tam <= 0) return;
    fputc('|', out);
    int i, j;
    for (i = 0; i < tam; i++) {
        for (j = 0; j < pack[i].quantVal; j++) {
            fputs(pack[i].valores[j], out);
            if (j != pack[i].quantVal - 1)
                fputc(',', out);
        }
        fputc('|', out);
    }
    fputc('\n', out);
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int n, m;
    if (fscanf(input, "%d", &n) != 1) n = 0; // quantPacotes
    if (fscanf(input, "%d", &m) != 1) m = 0; // maxAgrupa
    if (m >512){
		m=512;
	}

    lista* principalList = (lista*) malloc(sizeof(lista));
    principalList->maxAgrupa = m;
    principalList->packges = criaPacotes(input, n);

    pacote* printPackList = (pacote*) calloc(m,sizeof(pacote));
    int cont = 0, p = 0;
    int i;

    for (i = 0; i < n; i++) {
        int atual = principalList->packges[i].quantVal;

        if (p + atual < principalList->maxAgrupa) {
            printPackList[cont++] = principalList->packges[i];
            p += atual;
        }

        else if (p + atual == principalList->maxAgrupa) {
            printPackList[cont++] = principalList->packges[i];
            if (cont > 1) heapOrd(printPackList, cont);
            imprimirPacotes(output, printPackList, cont);
            cont = 0;
            p = 0;
        }

        else {
            if (cont > 0) {
                if (cont > 1) heapOrd(printPackList, cont);
                imprimirPacotes(output, printPackList, cont);
            }
            cont = 0;
            p = 0;
            i--; 
        }
    }

    if (cont > 0) {
        if (cont > 1) heapOrd(printPackList, cont);
        imprimirPacotes(output, printPackList, cont);
    }

    free(printPackList);
    for (i = 0; i < n; i++) {
        int j;
        for (j = 0; j < principalList->packges[i].quantVal; j++) {
            free(principalList->packges[i].valores[j]);
        }
        free(principalList->packges[i].valores);
    }
    free(principalList->packges);
    free(principalList);

    fclose(input);
    fclose(output);
    return 0;
}
