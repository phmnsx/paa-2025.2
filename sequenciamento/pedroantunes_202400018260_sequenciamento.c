#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct a{
	char* cod;
	int percen;
} nome;

void calcularTabela(int* k, char* P, int offset){
	for (int i = 0; i < strlen(P); i++)
		k[i + offset] = -1;
	for (int i = 1, j = -1; i < strlen(P); i++){
		while (j >= 0 && P[j + 1] != P[i])
			j = k[j + offset];
		if(P[j+1] == P[i])
			j++;
		k[i + offset] = j;
	}
}

int max(int a, int b){
	return (a > b) ? a : b;
}

int kmpMod(int* k, char* T, char* P, int matchSize){
    int n = strlen(T);
    int m = strlen(P);
    int nov = (int)round(m*0.9);
    int ind = -1, i = 0, j = -1, passos = 0;   

    while (i < n) {
		//printf("caracter: %c, j = %d, tabela = %d ind = %d\n", T[i], j + 1, k[j], ind);
		if (ind >= nov)
			return ind + 1;
		if (ind+ (n - i) < nov)
			return ind + 1;
		if(T[i] == P[j + 1]){
			passos++;
			i++;
			j++;
		}
		//houve mismatch
		else{
			if(passos >= matchSize){
				ind += passos;
				if(ind < m - 1)
					calcularTabela(k, &P[ind+1], ind+1);
			}

			if (j == -1 ||j == ind)
				i++;
			if ((j >= ind + 1) && j >= 0)
				j = ind + 1 + k[j];
			//printf("%d\n", j);
			//j = ind; // j = indice maior entre: indice "normal" ou indice minimo permitido (se ind avançar, alguma parte ja teve match valido antes);
			passos = j + 1 - (ind + 1);
		}
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
	printf("%ld\n", n);
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
		list[i].cod = malloc(sizeof(char)*strlen(buffer) + 1);
		strcpy(list[i].cod, buffer);
		fscanf(input, "%d", &tamanho2);
		for(int j = 0; j < tamanho2; j++){
			fscanf(input, "%s", buffer);
			calcularTabela(tabela, buffer, 0);
			int resps = kmpMod(tabela, entrada, buffer, matchSize);
			printf("%d\n", resps);
		}
	}
}
