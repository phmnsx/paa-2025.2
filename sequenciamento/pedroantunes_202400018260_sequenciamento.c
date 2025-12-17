
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
//5387
int const PRIME_NUMBER = 5387;
int const HIGHEST_CHARACTER_VALUE = 4;

void countingSort(nome* list, int n){
	int count[101] = {0};
	for(int i = 0; i < n; i++){
		int p = list[i].percen;
		count[p]++;
	}
	for (int i = 99; i >= 0; i--) count[i] += count[i + 1];
	nome* saida = malloc(sizeof(nome)*n);
	for (int i = n - 1; i >= 0; i--) {
        int p = list[i].percen;
        int pos = count[p] - 1;
        saida[pos] = list[i];
        count[p]--;
    }
    for (int i = 0; i < n; i++) list[i] = saida[i];
    list = saida;
}

long long myPowMod(int base, int exp, int mod) {
    long long r = 1;
    for(int i = 0; i < exp; i++) {
        r = (r * base) % mod;
    }
    return r;
}

int base4ToInt(char* s, int tam){
	int tot = 0;
	for(int i = 0; i < tam; i++){
		tot += (s[i] - 48) * (myPowMod(4, tam - (i + 1), PRIME_NUMBER));
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
	int resp = base4ToInt(tmp, tam) % PRIME_NUMBER;
	free(tmp);
	return resp;
}

int max(int a, int b){
	return (a > b) ? a : b;
}

no* novoNo(int ind, no* pn){
	no* new = malloc(sizeof(no));
	if (!new) {
		printf("foi aq?");
		perror("malloc");
		exit(1);
	}
	new->proxNo = pn;
	new->i = ind;
	return new;
}

int acgtToInt(char s) {
	if(s == 'A')
		return 1;
	else if (s == 'C')
		return 2;
	else if (s == 'G')
		return 3;
	else if (s == 'T')
		return 4;
		
	//printf("acgtToInt recebeu uma letra invalida!\n");
	return 0;
}

int rollingHash(char* s, int tam) {
	int hash = 0;
	for(int i = 0; i < tam; i++) {
		int posFromRight = (tam-1) - i;
		hash += acgtToInt(s[i]) * myPowMod(HIGHEST_CHARACTER_VALUE,posFromRight,PRIME_NUMBER);
		hash %= PRIME_NUMBER;
	}																	
	return hash;
}
	
int rehashRollingHash(int oldHash, char first, char next, int tam) {
	int newHash = oldHash;
	
	newHash -= (acgtToInt(first) * myPowMod(HIGHEST_CHARACTER_VALUE, tam-1,PRIME_NUMBER)) % PRIME_NUMBER;
	if(newHash < 0) newHash += PRIME_NUMBER;
	newHash *= HIGHEST_CHARACTER_VALUE;
	newHash += (acgtToInt(next));
	
	newHash %= PRIME_NUMBER;
	return newHash;
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    int c;
    while ((c = fgetc(input)) != EOF && c != '\n');
    //printf("k = %d\n", matchSize);
    char buffer[204800];
    char* entrada;
	//fscanf(input, "%20479s", buffer);
	
	fgets(buffer, sizeof(buffer), input);
	buffer[strcspn(buffer, "\n")] = '\0';
	//fprintf(output, "sucesso em ler buffer\n");
	entrada = malloc(sizeof(char)*strlen(buffer)+1);
	strcpy(entrada, buffer);
	//fprintf(output, "sucesso em salvar buffer em entrada\n");
	//fprintf(output, "%s\n", entrada);
	//fprintf(output, "tamanho da entrada: %d\n", strlen(entrada));
	int tamanhoCadeia = strlen(buffer);
	no** tabelaHash = malloc(sizeof(no*)*PRIME_NUMBER);
	//preencher tabela
	for(int i = 0; i < PRIME_NUMBER; i++){
		tabelaHash[i] = NULL;
	}
	

	 //primeira sequência de tamanho matchSize da entrada que será convertida para um valor hash
	char* sequenciaInicial= malloc(sizeof(char)*matchSize+1);
	for(int i = 0; i < matchSize; i++) sequenciaInicial[i] = entrada[i];
	sequenciaInicial[matchSize] = '\0';
	
	//Define o hash da primeira sequencia
	int hashAtual = rollingHash(sequenciaInicial, matchSize); 
	tabelaHash[hashAtual] = novoNo(0 , NULL);
	//printf("N: %d <- %s\n", hashAtual, sequenciaInicial);
	free(sequenciaInicial);
	for(int i = matchSize; i < tamanhoCadeia; i++) {
		hashAtual = rehashRollingHash(hashAtual,entrada[i-matchSize], entrada[i],matchSize);
		
		if(tabelaHash[hashAtual] == NULL){
			tabelaHash[hashAtual] = novoNo((i-matchSize+1) , NULL);
			//printf("N: %d <- ", hashAtual);
		}
		else{
			no* ultno = tabelaHash[hashAtual];
			//printf("V: %d <- ", hashAtual);
			while(ultno->proxNo != NULL) 
			
				ultno = ultno->proxNo;
			ultno->proxNo = novoNo((i-matchSize+1), NULL);
		}
		
		//printf("%c%c%c\n", entrada[i-matchSize+1], entrada[i-matchSize+2], entrada[i-matchSize+3]);
	}
	//fprintf(output, "Tabela hash construida com sucesso\n");
	int tamanho2;
	int tamanho;
	//fscanf(input, "%d", &tamanho);
	int teste;
	while ((c = fgetc(input)) != EOF && (c == ' ' || c == '\t' || c == '\n'));
    ungetc(c, input);
    teste = fscanf(input, "%d", &tamanho);
    if (teste != 1) {
        fprintf(output, "Erro ao ler o número de doenças: %d\n", teste);
    }
	//fgetc(input);
	//tamanho = fgetc(input);
	//fprintf(output, "qnt doencas: %d\n", tamanho);
	nome* list = malloc(sizeof(nome)*tamanho);
	for(int i = 0; i < tamanho; i++){
		fscanf(input, "%s", buffer);
		list[i].cod = malloc(sizeof(char)*strlen(buffer)+1);
		strcpy(list[i].cod, buffer);
		//fprintf(output, "Lendo: %s\n", list[i].cod);
		fscanf(input, "%d", &tamanho2);
		//fprintf(output, "tamanho: %d\n", tamanho2);
		int sucessos = 0;
		for(int j = 0; j < tamanho2; j++){
			fscanf(input, "%s", buffer); // buffer = gene
			int idxMax = 0;
			int idx = 0;
			int ultimoMatch = 0;
			no* cur;
			int tamanhoGene = strlen(buffer);				
			//printf("%s\n", buffer);		
			while(idxMax < tamanhoCadeia && ultimoMatch + matchSize <= tamanhoGene && idx + matchSize <= tamanhoGene){
				//printf("Indice no gene:%d\n", idx);
				if(idx + matchSize > tamanhoGene) break;
				
				int h = rollingHash(&buffer[idx], matchSize);
				if (h < 0 || h >= PRIME_NUMBER) break;
				cur = tabelaHash[h];
				
				
				if(cur == NULL) break;
				while(cur->proxNo != NULL && cur->i < idxMax) cur = cur->proxNo;

				int tmp = cur->i;
				
				int equals;
				int existe = 0;
				do {
					equals = 0;
					for(int i = 0; i < matchSize; i++) {
						//printf("%d - %c -%d- : %c -%d-\n", i, entrada[tmp+i], tmp+i, buffer[i+idx], i+idx);
						if((tmp+i) < idxMax) break;
						if(entrada[tmp+i] == buffer[i+idx]) equals++;
					}
					
					if(equals < matchSize && cur->proxNo == NULL) break;
					
					if(equals >= matchSize) {
						 existe = 1;
					} else { 
						cur = cur->proxNo;
						if(cur == NULL) break;
						tmp = cur->i;
					}
				} while(!existe);
				
				if(!existe) {
					//printf("existe = 0\n");
					break;
				} else {
					//printf("existe = 1\n");
				}
				
				while(idx < tamanhoGene && tmp < tamanhoCadeia && entrada[tmp] == buffer[idx]){
					tmp++;
					idx++;
					ultimoMatch++;
				}

				idxMax = tmp;
			}
			//fprintf(output, "%d <- %s\n", ultimoMatch, buffer);
			if((float)ultimoMatch >= 0.9*tamanhoGene)
				sucessos++;
		}
		list[i].percen = round(sucessos / (float)tamanho2 * 100.0);
		//printf("%d\n", list[i].percen);
	}
	countingSort(list, tamanho);
	for(int i = 0; i < tamanho; i++){
		fprintf(output, "%s->%d%%\n", list[i].cod, list[i].percen);
	}
}
