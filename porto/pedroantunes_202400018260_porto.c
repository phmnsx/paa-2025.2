#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct e1{
	char codigo[12];
	char cnpj[19];
	char cnpj2[19];
	int peso;
	int pesoDif;
	int erroPj;
	int erro;
	int pesoDifNum;
} elem;

int tonum(elem e, int i){
	return e.codigo[i] - 'A';
}

double arredondar(double x){
	if (x - (int)x > 0.5)
		return (int)x + 1;
	else
		return (int)x;
}

void compare(elem e, elem* dest){
	if(strcmp(e.cnpj, dest->cnpj)){
		strcpy(dest->cnpj2, e.cnpj);
		dest->erroPj = 1;
		dest->erro = 1;
		return;
	}
	double p1 = e.peso;
	double p2 = dest->peso;
	double res = (p1 - p2)/p2;
	if(res < 0)
		res = res*(-1);
	int fakeBool = res > 0.1;
	res = arredondar(res*100);
	
	if(fakeBool){
		dest->erro = 1;
		dest->pesoDifNum = abs(p1 - p2);
		dest->pesoDif = res;
	}
}

void copiar(elem** a, elem** b, int t){
	for(int i = 0; i < t; i++)
		a[i] = b[i];
}

int maiorIgual(elem* a, elem* b){
	if((a->erroPj > b->erroPj) || (a->erroPj && b->erroPj)) // a tem erro e b nao, ambos tem Pj
		return 1;
	if(a->erroPj < b->erroPj) // a n tem erro Pj e b tem
		return 0;
	if(a->pesoDif >= b->pesoDif) //se peso de a for maior igual a peso de b
		return 1;
	else
		return 0;
}

void merge(elem** array1, elem** array2, int i, int m, int j){
	int i1 = i, i2 = m + 1, k = i;
	while(i1 <= m && i2 <= j){
		if(maiorIgual(array2[i1],array2[i2]))
			array1[k++] = array2[i1++];
		else
			array1[k++] = array2[i2++];
	}
	if(i1 > m) copiar(&array1[k], &array2[i2], j - i2 + 1);
	else copiar(&array1[k], &array2[i1], m - i1 + 1);
	copiar(&array2[i], &array1[i], j - i + 1);
}

void mergesort(elem** array1, elem** array2, int i, int j){
	if(i < j){
		int m = i + (j - i)/2;
		mergesort(array1, array2, i, m);
		mergesort(array1, array2, m+1, j);
		merge(array1, array2, i, m, j);
	}
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");
	int tamanho;
	fscanf(input, "%d", &tamanho);
	static elem* hash[26][26][26][26][10];
	static int info[26][26][26][26];
	memset(info, 0, sizeof(info));
	elem** array1 = (elem**)malloc(tamanho*sizeof(elem*));
	elem** array2 = (elem**)malloc(tamanho*sizeof(elem*));
	for(int i = 0; i < tamanho; i++){
		elem *n = malloc(sizeof(elem));
		fscanf(input, "%s %s %d", n->codigo, n->cnpj, &n->peso);
		array1[i] = n;
		array2[i] = n;
		n->erroPj = 0;
		n->erro = 0;
		n->pesoDif = 0;
		n->pesoDifNum = 0;
		int a = tonum(*n, 0);
		int b = tonum(*n, 1);
		int c = tonum(*n, 2);
		int d = tonum(*n, 3);
		hash[a][b][c][d][info[a][b][c][d]++] = n;
	}
	int tamanho2;
	fscanf(input, "%d", &tamanho2);
	for(int i = 0; i < tamanho2; i++){
		elem n;
		fscanf(input, "%s %s %d", n.codigo, n.cnpj, &n.peso);
		int a = tonum(n, 0);
		int b = tonum(n, 1);
		int c = tonum(n, 2);
		int d = tonum(n, 3);
		int j = 0;
		while(j < info[a][b][c][d]){
			
			if(strcmp(n.codigo, hash[a][b][c][d][j]->codigo) == 0)
				break;
			j++;
		}
		compare(n,  hash[a][b][c][d][j]);
	}
	
	mergesort(array1, array2, 0, tamanho-1);
	for(int i = 0; i < tamanho; i++){
		if(array1[i]->erro){
			fprintf(output, "%s:", array1[i]->codigo);
			if(array1[i]->erroPj)
				fprintf(output, "%s<->%s\n", array1[i]->cnpj, array1[i]->cnpj2);
			else
				fprintf(output, "%dkg(%d%%)\n",array1[i]->pesoDifNum, array1[i]->pesoDif);
		}
		free(array1[i]);
	}
	free(array1);
	free(array2);
	
}
