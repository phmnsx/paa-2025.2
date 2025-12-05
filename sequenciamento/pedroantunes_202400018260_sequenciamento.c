#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct a{
	char* cod;
	int percen;
} nome;

void calcularTabela(int* k, char* P){
	for (int i = 1, j = -1; i < strlen(P); i++){
		while (j >= 0 && P[j + 1] != P[i])
			j = k[j];
		if(P[j+1] == P[i])
			j++;
		k[i] = j;
	}
}

int max(int a, int b){
	return (a > b) ? a : b;
}

int kmpMod(int* k, char* T, char* P, int matchSize){
    int n = strlen(T);
    int m = strlen(P);

    int ind = -1, i = 0, j = -1, passos = 0;   

    while (i < n) {
		printf("caracter: %c, j = %d, tabela = %d ind = %d\n", T[i], j + 1, k[j], ind);
		if (ind >= m - 1)
			return m;
		if(T[i] == P[j + 1]){
			passos++;
			j++;
		}
		//houve mismatch
		else{
			if(passos >= matchSize)
				ind += passos;
			j = max(k[j], ind);
			//j = ind; // j = indice maior entre: indice "normal" ou indice minimo permitido (se ind avançar, alguma parte ja teve match valido antes);
			passos = 0;
		}
		i++;
    }

    return ind + 1;	
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    char buffer[4096];
    char* entrada;
	fscanf(input, "%s", buffer);
	entrada = malloc(sizeof(char)*strlen(buffer) + 1);
	strcpy(entrada, buffer);
	int tamanho;
	int tamanho2;
	float valid;
	char* cadeia;
	int tabela[4096];
	fscanf(input, "%d", &tamanho);
	nome* list = malloc(sizeof(nome)*tamanho);
	for(int i = 0; i < tamanho; i++){
		valid = 0.0;
		fscanf(input, "%s", buffer);
		list[i].cod = malloc(sizeof(char)*strlen(buffer) + 1);
		strcpy(list[i].cod, buffer);
		fscanf(input, "%d", &tamanho2);
		for(int j = 0; j < tamanho2; j++){
			fscanf(input, "%s", buffer);
			cadeia = malloc(sizeof(char)*strlen(buffer) + 1);
			strcpy(cadeia, buffer);
			for(int k = 0; k < strlen(cadeia); k++){
				tabela[k] = -1;
			}
			calcularTabela(tabela, cadeia);
			printf("cad: %s, %d\n", cadeia, strlen(cadeia));
			printf("tabela:");
			for(int k = 0; k < strlen(cadeia); k++){
				printf("%i, ", tabela[k]);
			}
			printf("\n");
			int resps = kmpMod(tabela, entrada, cadeia, matchSize);
			printf("resp:%d\n\n", resps);
		}
	}
}
