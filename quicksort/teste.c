#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void trocar(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* ===========================================================
   Seletores de Pivô
   =========================================================== */

// 1. Pivô Padrão → usa posição low
int pivo_padrao(int V[], int low, int high) {
    return low;
}

// 2. Pivô Aleatório (Modular) baseado no valor da posição low
int pivo_aleatorio_mod(int V[], int low, int high) {
    int ValorIni = V[low];
    int Desloc = abs(ValorIni);
    int Va = low + (Desloc % (high - low + 1));
    return Va;
}

// Função auxiliar para mediana de três valores
int encontrar_mediana(int a, int b, int c) {
    if ((a > b && a < c) || (a < b && a > c))
        return a;
    else if ((b > a && b < c) || (b < a && b > c))
        return b;
    else
        return c;
}

// 3. Pivô Mediana de 3 baseado em n/4, n/2, 3n/4
int pivo_mediana(int V[], int low, int high) {
    int n = high - low + 1;

    if (n < 3)
        return low;

    int i1 = low + n / 4;
    int i2 = low + n / 2;
    int i3 = low + (3 * n) / 4;

    int V1 = V[i1];
    int V2 = V[i2];
    int V3 = V[i3];

    int med = encontrar_mediana(V1, V2, V3);

    if (med == V1) return i1;
    if (med == V2) return i2;
    return i3;
}

/* ===========================================================
   Particionamentos
   =========================================================== */

// Particionamento de Lomuto
int particionar_lomuto(int V[], int low, int high, int(*pivo_idx)(int[], int, int)) {
    int p_idx = pivo_idx(V, low, high);
    trocar(&V[p_idx], &V[high]);

    int pivo = V[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (V[j] <= pivo) {
            i++;
            trocar(&V[i], &V[j]);
        }
    }

    trocar(&V[i + 1], &V[high]);
    return i + 1;
}

// Particionamento de Hoare
int particionar_hoare(int V[], int low, int high, int(*pivo_idx)(int[], int, int)) {
    int p_idx = pivo_idx(V, low, high);
    trocar(&V[p_idx], &V[low]);

    int pivo = V[low];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do { i++; } while (V[i] < pivo);
        do { j--; } while (V[j] > pivo);

        if (i >= j)
            return j;

        trocar(&V[i], &V[j]);
    }
}

/* ===========================================================
   Quicksort — 6 Métodos
   =========================================================== */

// 1. Lomuto Padrão
void quick_sort_lomuto_padrao(int V[], int low, int high) {
    if (low < high) {
        int pi = particionar_lomuto(V, low, high, pivo_padrao);
        quick_sort_lomuto_padrao(V, low, pi - 1);
        quick_sort_lomuto_padrao(V, pi + 1, high);
    }
}

// 2. Lomuto Aleatório (Modular)
void quick_sort_lomuto_aleatorio(int V[], int low, int high) {
    if (low < high) {
        int pi = particionar_lomuto(V, low, high, pivo_aleatorio_mod);
        quick_sort_lomuto_aleatorio(V, low, pi - 1);
        quick_sort_lomuto_aleatorio(V, pi + 1, high);
    }
}

// 3. Lomuto Mediana de 3
void quick_sort_lomuto_mediana(int V[], int low, int high) {
    if (low < high) {
        int pi = particionar_lomuto(V, low, high, pivo_mediana);
        quick_sort_lomuto_mediana(V, low, pi - 1);
        quick_sort_lomuto_mediana(V, pi + 1, high);
    }
}

// 4. Hoare Padrão
void quick_sort_hoare_padrao(int V[], int low, int high) {
    if (low < high) {
        int pi = particionar_hoare(V, low, high, pivo_padrao);
        quick_sort_hoare_padrao(V, low, pi);
        quick_sort_hoare_padrao(V, pi + 1, high);
    }
}

// 5. Hoare Aleatório (Modular)
void quick_sort_hoare_aleatorio(int V[], int low, int high) {
    if (low < high) {
        int pi = particionar_hoare(V, low, high, pivo_aleatorio_mod);
        quick_sort_hoare_aleatorio(V, low, pi);
        quick_sort_hoare_aleatorio(V, pi + 1, high);
    }
}

// 6. Hoare Mediana de 3
void quick_sort_hoare_mediana(int V[], int low, int high) {
    if (low < high) {
        int pi = particionar_hoare(V, low, high, pivo_mediana);
        quick_sort_hoare_mediana(V, low, pi);
        quick_sort_hoare_mediana(V, pi + 1, high);
    }
}

int main(int argc, char* argv[]){
	int v [1842];
	FILE *input = fopen(argv[1], "r");
	for(int i = 0; i < 1842; i++){
		fscanf(input, "%d ", &v[i]);
	}
	
	quick_sort_hoare_aleatorio(v, 0, 1842);
	
	for(int i = 0; i < 1842; i++){
		printf("%d\n", v[i]);
	}
    
}

