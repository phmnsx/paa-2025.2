
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct a{
	char* cod;
	int percen;
} nome;

typedef struct no{
	int i;
	struct no* proxNo;
} no;

int myPow(int base, int expoente){
	int r = 1;
	for(int i = 0; i < expoente; i++){
		r = r * base;
	}
	return r;
}

int base3ToInt(char* s, int tam){
	int tot = 0;
	for(int i = 0; i < tam; i++){
		tot += (s[i] - 48) * (myPow(4, tam - (i + 1)));
	}
	return tot;
}

int getIdx(char* s, int tam){
	char* tmp = malloc(sizeof(char)*tam + 1);
	for(int i = 0; i < tam; i++){
		if(s[i] == 'A')
			tmp[i] = '0';
		else if (s[i] == 'C')
			tmp[i] = '1';
		else if (s[i] == 'T')
			tmp[i] = '2';
		else if (s[i] == 'G')
			tmp[i] = '3';
	}
	int resp = base3ToInt(tmp, tam);
	free(tmp);
	return resp;
}

int max(int a, int b){
	return (a > b) ? a : b;
}

no* novoNo(int ind, no* pn){
	no* new = malloc(sizeof(no));
	new->proxNo = pn;
	new->i = ind;
	return new;
}
int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    printf("k = %d\n", matchSize);
    char buffer[4096];
    char* entrada;
	fscanf(input, "%s", buffer);
	entrada = malloc(sizeof(char)*strlen(buffer)+1);
	strcpy(entrada, buffer);
	char* tmp = malloc(sizeof(char)*matchSize + 2);
	tmp[0] = '3';
	for(int i = 1; i <= matchSize; i++){
		tmp[i] = '0';
	}
	int tamanhoCadeia = strlen(buffer);
	no** tabelaHash = malloc(sizeof(no*)*base3ToInt(tmp, matchSize + 1));
	int* usado = malloc(sizeof(int)*base3ToInt(tmp, matchSize + 1));
	
	//preencher tabela
	for(int i = 0; i < base3ToInt(tmp, matchSize + 1); i++){
		tabelaHash[i] = NULL;
	}
	for(int i = 0; i + matchSize - 1 < tamanhoCadeia; i++){
		if(tabelaHash[getIdx(&entrada[i], matchSize)] == NULL){
			tabelaHash[getIdx(&entrada[i], matchSize)] = novoNo(i , NULL);
		}
		else{
			no* ultno = tabelaHash[getIdx(&entrada[i], matchSize)];
			while(ultno->proxNo != NULL) 
				ultno = ultno->proxNo;
			ultno->proxNo = novoNo(i, NULL);
		}
	}

	int tamanho2;
	//printf("indice:%d", tabelaHash[getIdx("AAA", matchSize)]->proxNo->proxNo->i);
	int tamanho;
	fscanf(input, "%d", &tamanho);
	nome* list = malloc(sizeof(nome)*tamanho);
	for(int i = 0; i < tamanho; i++){
		fscanf(input, "%s", buffer);
		list[i].cod = malloc(sizeof(char)*strlen(buffer)+1);
		strcpy(list[i].cod, buffer);
		fscanf(input, "%d", &tamanho2);
		for(int j = 0; j < tamanho2; j++){
			fscanf(input, "%s", buffer); // buffer = gene
			memset(usado, 0, sizeof(int)*base3ToInt(tmp, matchSize + 1));
			int idxMax = 0;
			int idx = 0;
			int ultimoMatch = 0;
			no* cur;
			int tamanhoGene = strlen(buffer);
			while(idxMax < tamanhoCadeia && ultimoMatch + matchSize <= tamanhoGene && idx + matchSize <= tamanhoGene){
				//printf("Indice no gene:%d\n", idx);
				cur = tabelaHash[getIdx(&buffer[idx], matchSize)];
				int pulos = usado[getIdx(&buffer[idx], matchSize)];
				if(cur == NULL)
					break;
				while(pulos > 0 && cur->proxNo != NULL && cur->i < idxMax){
					cur = cur->proxNo;
					pulos--;
				}
				
				int tmp = cur->i;
				while(entrada[tmp] == buffer[idx] && idx < tamanhoGene){
					tmp++;
					idx++;
					ultimoMatch++;
				}
					
				idxMax = tmp;
				usado[getIdx(&buffer[idx], matchSize)]++;
			}
			//printf("%d\n", ultimoMatch);
		}
	}
}
