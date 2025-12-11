#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct a{
	char* cod;
	int percen;
} nome;

int max(int a, int b){
	return (a > b) ? a : b;
}

int charToNum(char c){ // seja c C/A/T/G
	if(c == 'A')
		return 0;
	if(c == 'C')
		return 1;
	if(c == 'G')
		return 2;
	if(c == 'T')
		return 3;
	return -1;
}

int temGene(int* chaves, int* encontrado, int hash, int var1){
	int idx = hash & (var1 - 1);
	while(encontrado[idx] == 1){
		if (chaves[idx] == hash)
			return 1;
		idx = (idx + 1) & (var1 - 1);
	}
	return 0;
}

/*void insereHash(int** chaves, int** encontrados, int var1, int key){
	int* chave = *chaves;
	int* encontrado = *encontrados;
	
	int idx = key & (var1 - 1);
	
	while(encontrado[idx] == 1){
		if(chave[idx] == key) 
			return;
		idx = (idx + 1) & (var1 - 1);
	}
	encontrado[idx] = 1;
	chave[idx] = key;
}*/

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    char buffer[100000];
    char* entrada;
	fscanf(input, "%s", buffer);
	int var1 = strlen(buffer);
	entrada = malloc(sizeof(char)*var1 + 1);
	int* entradaHash = (int*) malloc(sizeof(int)*var1*2);
	int* chaves = (int*) malloc(sizeof(int)*var1*2);
	int* encontrado = (int*) malloc(sizeof(int)*var1*2);
	strcpy(entrada, buffer);
	memset(entradaHash, 1, var1*2*sizeof(int));
	memset(encontrado, 0, var1*2*sizeof(int));
	int hdna = 0;
	int rlhash = 0;
	if (matchSize < 32)
		rlhash = (1 << (2*matchSize)) - 1;
	else
		rlhash = -1;
	for(int i = 0; i < var1; i++){
		hdna = ((hdna << 2) | charToNum(entrada[i])) & rlhash;
		if (i >= matchSize - 1)
			insereHash(&chaves, &encontrado, var1, hdna);
			
	}
	int tamanho;
	int tamanho2;
	fscanf(input, "%d", &tamanho);
	nome* list = malloc(sizeof(nome)*tamanho);
	for(int i = 0; i < tamanho; i++){
		fscanf(input, "%s", buffer);
		int m = strlen(buffer);
		list[i].cod = malloc(sizeof(char)*m + 1);
		strcpy(list[i].cod, buffer);
		fscanf(input, "%d", &tamanho2);
		for(int j = 0; j < tamanho2; j++){
			fscanf(input, "%s", buffer);
			int bufferSize = strlen(buffer);
			printf("tamanho: %d ", bufferSize);
			int hash = 0;
			int resp = 0;
			int* lido = malloc(sizeof(int)*bufferSize);
			memset(lido, 0, bufferSize*sizeof(int));
			for(int i = 0; i < bufferSize; i++){
				char c = buffer[i];
				hash = ((hash << 2) | charToNum(c)) & rlhash;
				if(i >= matchSize - 1){
					if(temGene(chaves, encontrado, hash, var1)){
						for(int j = 0; j < matchSize; j++){
							int p = i - j;
							if(lido[p] == 0){
								lido[p] = 1;
								resp++;
							}
						}
					}
				}
			}
			printf("%s: ", buffer);
			printf("%d\n", resp);
		}
	}
}
