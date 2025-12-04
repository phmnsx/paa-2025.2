#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct a{
	char* nome;
	int percen;
} nome;

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    char buffer[4096];
    char* entrada;
	fscanf(input, "%s", buffer);
	entrada = malloc(sizeof(char)*strlen(buffer));
	strcpy(entrada, buffer);
	int tamanho;
	fscanf(input, "%d", &tamanho);
	nome* list = malloc(sizeof(nome)*tamanho);
	for(int i = 0; i < tamanho; i++){
	}
}
