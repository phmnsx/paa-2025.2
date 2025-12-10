#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct bit{
	char s1;
	char s2;
} bit;

typedef struct no{
	int freq;
	bit s;
	struct no* D;
	struct no* E;
} no;

typedef struct arvore{
	int tamanho;
	int capacidade;
	no** array;
} arvore;

void printBits(bit* entrada, int tamanho, FILE* saida){
	for(int i = 0; i < tamanho; i++){
		fprintf(saida, "%c%c ", entrada[i].s1, entrada[i].s2);
	}
	fprintf(saida, "\n");
}

int igual(bit a, bit b){
	return (a.s1 == b.s1 && a.s2 == b.s2);
}

bit toBit(int num){
	bit resp;
	char string[3];
	if(num == -1){
		resp.s1 = 'G';
		resp.s2 = 'G';
		return resp;
	}
	sprintf(string, "%02X", num);
	resp.s1 = string[0];
	resp.s2 = string[1];
	return resp;
}

int hexToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

int bitToIndex(bit b) {
    return (hexToInt(b.s1) << 4) | hexToInt(b.s2);
}

int comprimeRLE(bit** entrada, int tamanho){
	bit* string = *entrada;
	bit* resultado = (bit*)malloc(sizeof(bit)*tamanho);
	int i = 0, j = 0; //i percorre entrada e j percorre resultado
	int contagem = 1;
	while(i <= tamanho){
		if(i == tamanho){
			*entrada = resultado;
			free(string);
			return j;
		}
		if(igual(string[i], string[i+1])){
			contagem++;
			i++;
		}
		else{
			resultado[j] = toBit(contagem); //adicione a contagem ao espaço j
			resultado[j+1] = string[i]; //adicione o bit contado j vezes em j + 1
			j += 2; //coloque j pra depois do bit
			contagem = 1;
			i++;
		}
	}
	return -1;
}

void swap(no** a, no** b){
	no* t = *a;
	*a = *b;
	*b = t;
}

void heapify(arvore* heap, int idx){
	int menor = idx;
	int esq = 2 * idx + 1;
	int dir = 2 * idx + 2;
	
	if(esq < heap->tamanho && heap->array[esq]->freq < heap->array[menor]->freq)
		menor = esq;
	if(dir < heap->tamanho && heap->array[dir]->freq < heap->array[menor]->freq)
		menor = dir;
	if(menor != idx){
		swap(&heap->array[menor], &heap->array[idx]);
		heapify(heap, menor);
	}
}

void inserir(arvore* heap, no* novoNo){
	heap->tamanho++;
	int i = heap->tamanho - 1;
	while(i && novoNo->freq < heap->array[(i - 1) / 2]->freq){
		heap->array[i] = heap->array[(i-1)/2];
		i = (i-1) / 2;
	}
	heap->array[i] = novoNo;
}

void construirHeap(arvore* heap){
	int n = heap->tamanho - 1;
	int i;
	for(i = (n-1)/2 ; i >= 0; i--)
		heapify(heap, i);
}

no* extrairMin(arvore* fpm){
	no* tmp = fpm->array[0];
	fpm->array[0] = fpm->array[fpm->tamanho - 1];
	fpm->tamanho--;
	heapify(fpm, 0);
	return tmp;
}

no* novoNo(bit dado, int freq){
	no* newnode = (no*)malloc(sizeof(no));
	newnode->E = newnode->D = NULL;
	newnode->freq = freq;
	newnode->s = dado;
	return newnode;
}

arvore* criarHeap(int tam){
	arvore* g = (arvore*)malloc(sizeof(arvore));
	g->tamanho = 0;
	g->capacidade = tam;
	g->array = (no**)malloc(g->capacidade*sizeof(no*));
	return g;
}

void gerarTabela(no* raiz, char* codigo, int prof, char T[][256]){
	if(!raiz)
		return;
	if (!raiz->E && !raiz->D){
		codigo[prof] = '\0';
		strcpy(T[raiz->s.s1*256+raiz->s.s2], codigo);
		return;
	}
	codigo[prof] = '0';
	gerarTabela(raiz->E, codigo, prof + 1, T);
	
	codigo[prof] = '1';
	gerarTabela(raiz->D, codigo, prof + 1, T);
}

int comprimeHUF(bit** entrada, int tamanho){
	bit* string = *entrada;
	no H[256];
	for(int i = 0; i < 256; i++){
		H[i].freq = 0;
		H[i].s = toBit(-1);
	}
	int bitsDif = 0; //quantos bits diferentes tem
	int j = 0;
	int i = 0;
	while(i < tamanho){
		if(igual(H[j].s, toBit(-1))){
			H[j].s = string[i];
			H[j].freq++;
			bitsDif++;
			i++;
		}
		else if(igual(string[i], H[j].s)){
			H[j].freq++;
			j = 0;
			i++;
		}
		else
			j++;
	}
	arvore* fpm = criarHeap(bitsDif*10);
	no* tmp;
	
	for(int i = 0; i < bitsDif; i++){
		tmp = novoNo(H[i].s, H[i].freq);
		inserir(fpm, tmp);
	}
	while(fpm->tamanho > 1){
		no* x = extrairMin(fpm);
		no* y = extrairMin(fpm);
		tmp = novoNo(toBit(-1),x->freq + y->freq);
		tmp->E = x;
		tmp->D = y;
		inserir(fpm, tmp);
	}
	tmp = extrairMin(fpm); //raiz
	static char T[65536][256];
	char cod[256];
	
	gerarTabela(tmp, cod, 0, T);
	
	char* C = malloc(sizeof(char)*tamanho*20);
	memset(C, 'a', sizeof(char)*tamanho*20);
	C[0] = '\0';
	for(int i = 0; i < tamanho; i++){
		strcat(C, T[string[i].s1*256+string[i].s2]);
	}
	printf("%s\n", C);
	free(fpm->array);
	free(fpm);
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int qntLinhas;
    fscanf(input, "%d", &qntLinhas);
    
    int qntBits;
    bit* rle;
    bit* huf;
    for(int i = 0; i < qntLinhas; i++){
		fscanf(input, "%d", &qntBits);
		rle = (bit*)malloc(sizeof(bit)*qntBits);
		huf = (bit*)malloc(sizeof(bit)*qntBits);
		for(int j = 0; j < qntBits; j++){
			fscanf(input, " %c%c", &rle[j].s1, &rle[j].s2);
			huf[j].s1 = rle[j].s1;
			huf[j].s2 = rle[j].s2;
		}
		int rleLen = comprimeRLE(&rle, qntBits);
		int hufLen = comprimeHUF(&huf, qntBits);
	}
}
