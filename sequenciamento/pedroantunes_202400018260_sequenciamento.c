#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct a{
	char* cod;
	int percen;
} nome;

typedef struct b{
	int len, link;
	//int maxN; // 2n - 1, n sendo o tamanho da string
 	int next[4]; //C A T G
} state;

int max(int a, int b){
	return (a > b) ? a : b;
}

/*state* sa_init(int s, state* t0){
	t0->maxN = (2*s) - 1;
	t0->len = malloc(sizeof(int)*maxN);
	t0->link = malloc(sizeof(int)*maxN);
	t0->len[0] =  
}*/

int charToNum(char c){ // seja c C/A/T/G
	if(c == 'C')
		return 0;
	if(c == 'A')
		return 1;
	if(c == 'T')
		return 2;
	if(c == 'G')
		return 3;
	return -1;
}

void generateSAM(char* string, int stringSize, state** t0){
	*t0 = malloc(sizeof(state)*stringSize*2);
	state* t = *t0;
	memset(t, 0, sizeof(state) * stringSize * 2);
	memset(t[0].next, -1, sizeof(t[0].next));
	t[0].len = 0;
	t[0].link = -1;
	int sz = 1; // 1 ou 0?
	int last = 0;
	for(int i = 0; i < stringSize; i++){
		int cur = sz++;
		t[cur].len = t[last].len + 1;
		memset(t[cur].next, -1, sizeof(t[cur].next));
		int p = last;
		while(p != -1 && t[p].next[charToNum(string[i])] == -1){
			t[p].next[charToNum(string[i])] = cur;
			p = t[p].link;
		}
		if (p == -1)
			t[cur].link = 0;
		else{
			int q = t[p].next[charToNum(string[i])];
			if(t[p].len + 1 == t[q].len)
				t[cur].link = q;
			else{
				int clone = sz++;
				t[clone] = t[q];
				t[clone].len = t[p].len + 1;
				
				while(p != -1 && t[p].next[charToNum(string[i])] == q){
					t[p].next[charToNum(string[i])] = clone;
					p = t[p].link;
				}
				t[q].link = t[cur].link = clone;
			}
		}
		last = cur;
	}
}

int solveSAM(char* entrada, int matchSize, state* sam) {

}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int matchSize;
    fscanf(input, "%d", &matchSize);
    char buffer[100000];
    char* entrada;
	fscanf(input, "%s", buffer);
	entrada = malloc(sizeof(char)*strlen(buffer) + 1);
	strcpy(entrada, buffer);
	int tamanho;
	int tamanho2;
	//float valid;
	fscanf(input, "%d", &tamanho);
	state* t = 0;
	generateSAM(entrada, strlen(entrada), &t);
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
			printf("\n //new//\n");
			//test_sam(buffer, t);
			int resps = solveSAM(entrada, matchSize, t);
			printf("%d\n", resps);
		}
	}
}
