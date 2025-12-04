#include <stdio.h>
#include <stdlib.h>

typedef struct pac {
	int prioridade;
	char* conteudo;
} pacote;

void trocar(pacote* a, pacote* b){
	pacote c = *a;
	*a = *b;
	*b = c; 
}

void heapify(pacote* lista, int n, int i){
	int maior = i;
	int l = (2 * i) + 1;
	int r = (2 * i) + 2;
	if((l < n) && (lista[l].prioridade < lista[maior].prioridade)){
		maior = l;
	}
		
	
	if((r < n) && (lista[r].prioridade < lista[maior].prioridade)){
		maior = r;
	}
	if (maior != i){
		trocar(&lista[i], &lista[maior]);
		heapify(lista, n, maior);
	}
}

void construirHeap(pacote* lista, int j){
	int ini = (j/2) - 1;
	for(int i = ini; i >= 0; i--){
		heapify(lista, j, i);
	}
}

void heapsort(pacote* lista, int j){
	construirHeap(lista, j);
	for(int i = j - 1; i > 0; i--){
		trocar(&lista[0], &lista[i]);
		heapify(lista, i, 0);
	}
}

void out(pacote* lista, int j, FILE* output){
	fprintf(output,"|");
	for(int i = 0; i <= j; i++){
		fprintf(output, "%s|", lista[i].conteudo);
	}
	fprintf(output,"\n");
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int qntP;
    int maxByt;
    fscanf(input,"%d %d", &qntP, &maxByt);
    pacote* lista = (pacote*) malloc(sizeof(pacote)*maxByt);
    int tamanho;
    int tamanhoTotal = 0;
    int j = 0;
    int buffer;
    for(int i = 0; i < qntP; i++, j++){ //mandar todos os pacotes "no meio"
		fscanf(input,"%d %d ", &buffer, &tamanho);
		if (tamanho + tamanhoTotal > maxByt){ 
			heapsort(lista, j);
			out(lista, j - 1, output);
			for(int k = 0; k < j; k++)
				free(lista[k].conteudo);
			j = 0;
			tamanhoTotal = 0;
		}
		tamanhoTotal += tamanho;
		lista[j].prioridade = buffer;
		lista[j].conteudo = (char*)malloc(sizeof(char)*3*tamanho + 1);//dois caracteres e espaço
		for(int k = 0; k < tamanho*3; k += 3){
			fscanf(input," %c%c", &lista[j].conteudo[k], &lista[j].conteudo[k+1]);
			lista[j].conteudo[k+2] = ',';
		}
		lista[j].conteudo[3*tamanho - 1] = '\0';
	}
	//manda o ultimo pacote
	heapsort(lista, j);
	out(lista, j - 1, output);
}
