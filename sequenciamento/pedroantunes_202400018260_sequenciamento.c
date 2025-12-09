#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct a{
	char* cod;
	int percen;
} nome;

void calcularTabela(int* k, char* P, int offset){
	int n = strlen(P);
	for (int i = 0; i < n; i++)
		k[i + offset] = -1;
	for (int i = 1, j = -1; i < n; i++){
		while (j >= 0 && P[j + 1] != P[i])
			j = k[j + offset];
		if(P[j+1] == P[i])
			j++;
		k[i + offset] = j + offset;
	}
}

int max(int a, int b){
	return (a > b) ? a : b;
}

int kmpMod(int* tabela, char* T, char* P, int matchSize, int n){
	int m = strlen(P);
    int nov = (int)round(m*0.9);
    int* k = tabela;
    int ind = -1, i = 0, j = -1, passos = 0;   

    while (i < n) {
		//printf("caracter: %c, j = %d, tabela = %d ind = %d\n", T[i], j + 1, k[j], ind);
		if (ind >= nov || ind == m - 1)
			return ind + 1;
		/*if (ind+ (n - i) < nov)
			return ind + 1;*/
		if(T[i] == P[j + 1]){
			passos++;
			i++;
			j++;
		}
		//houve mismatch
		else{
			if(passos >= matchSize)
				ind += passos;

			if (j == -1 ||j == ind)
				i++;
			if ((j >= ind + 1) && j >= 0){
				if(k[j] != -1 && k[j] < ind){
					calcularTabela(k, &P[ind+1], ind+1);
					//printf("recalculo: %s\n", &P[ind+1]);
				}
				if(k[j] < ind)
					j = ind;
				else
					j = k[j];
			}
			//printf("%d\n", j);
			//j = ind; // j = indice maior entre: indice "normal" ou indice minimo permitido (se ind avançar, alguma parte ja teve match valido antes);
			passos = j + 1 - (ind + 1);
			
		}
		//printf("\n i, j, ind: %d %d %d", i, j, ind);
    }

    return ind + 1;	
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    //FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    char buffer[100000];
    char* entrada;
	fscanf(input, "%s", buffer);
	int n = strlen(buffer);
	entrada = malloc(sizeof(char)*strlen(buffer) + 1);
	strcpy(entrada, buffer);
	int tamanho;
	int tamanho2;
	//float valid;
	int* tabela = (int*) malloc(sizeof(int)*4096);
	fscanf(input, "%d", &tamanho);
	nome* list = malloc(sizeof(nome)*tamanho);
	for(int i = 0; i < tamanho; i++){
		fscanf(input, "%s", buffer);
		int m = strlen(buffer);
		list[i].cod = malloc(sizeof(char)*m + 1);
		strcpy(list[i].cod, buffer);
		//printf("doenca: %s\n", list[i].cod);
		fscanf(input, "%d", &tamanho2);
		for(int j = 0; j < tamanho2; j++){
			fscanf(input, "%s", buffer);
			calcularTabela(tabela, buffer, 0);
			//printf("kmp de: %s\n", buffer);
			int resps = kmpMod(tabela, entrada, buffer, matchSize, n);
			//printf("%d\n", resps);
		}
	}
}
