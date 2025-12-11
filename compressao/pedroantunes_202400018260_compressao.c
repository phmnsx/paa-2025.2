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

void binToHex(char* C, char* hex){
	int len = strlen(C);
	int hexLen = (len+3)/4;
	for(int i = 0; i < hexLen; i++){
		int val = 0;
		for(int j = 0; j < 4; j++){
			int bitPos = i * 4 + j;
			if(bitPos < len)
				val = (val << 1) | (C[bitPos] - '0');
			else
				val = val << 1;
		}
		if (val < 10)
			hex[i] = '0' + val;
		else
			hex[i] = 'A' + (val - 10);
	}
	hex[hexLen] = '\0';
}

/*void printBits(bit* entrada, int tamanho, FILE* saida){
	for(int i = 0; i < tamanho; i++){
		fprintf(saida, "%c%c ", entrada[i].s1, entrada[i].s2);
	}
	fprintf(saida, "\n");
}*/

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

char* bitToString(bit* bits, int j){
	char* string = malloc(sizeof(char)*j + 1);
	string[j] = '\0';
	for(int i = 0; i < j/2; i++){
		string[2*i] = bits[i].s1;
		string[2*i + 1] = bits[i].s2;
	}
	return string;
}

int comprimeRLE(bit** entrada, int tamanho, char** saida){
	bit* string = *entrada;
	bit* resultado = (bit*)malloc(sizeof(bit)*2*tamanho);
	int i = 0, j = 0; //i percorre entrada e j percorre resultado
	int contagem = 1;
	while(i <= tamanho){
		if(i == tamanho){
			free(*entrada);
			*saida = bitToString(resultado, j*2);
			return j*2;
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

int toInt(bit b){
	char string[3];
	string[0] = b.s1;
	string[1] = b.s2;
	string[2] = '\0';
	int res = (int)strtol(string, NULL, 16);
	return res;
}

void gerarTabela(no* raiz, char* codigo, int prof, char T[][256]){
	if(!raiz) return;
	
	if(!(raiz->E) && !(raiz->D)){
		if(!(raiz->s.s1 == 'G')){
			if(prof == 0){
				codigo[0] = '0';
				codigo[1] = '\0';
			}
			else
				codigo[prof]='\0';
			int idx = toInt(raiz->s);
			strcpy(T[idx], codigo);
		}
		return;
	}
	if(raiz->E){
		codigo[prof] = '0';
		gerarTabela(raiz->E, codigo, prof+1, T);
	}
	if(raiz->D){
		codigo[prof] = '1';
		gerarTabela(raiz->D, codigo, prof+1, T);
	}
}
void freeTree(no* t){
	if(t->E)
		freeTree(t->E);
	if(t->D)
		freeTree(t->D);
	free(t);
}
int comprimeHUF(bit** entrada, int tamanho, char** saida){
	bit* string = *entrada;
	int H[256] = {0};
	for(int i = 0; i < tamanho; i++){
		H[toInt(string[i])]++;
	}

	arvore* fpm = criarHeap(256);
	no* tmp;
	
	for(int i = 0;i < 256; i++){
		if(H[i] > 0){
			tmp = novoNo(toBit(i), H[i]);
			inserir(fpm, tmp);
		}
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
	static char T[256][256];
	char cod[256];
	gerarTabela(tmp, cod, 0, T);
	char* C = malloc(sizeof(char)*256+1);
	char* hex = malloc(sizeof(char)*255+1);
	memset(C, 'a', sizeof(char)*256+1);
	C[0] = '\0';
	for(int i = 0; i < tamanho; i++){
		strcat(C, T[toInt(string[i])]);
	}
	binToHex(C, hex);
	int aux = strlen(hex);
	if(aux % 2){
		hex[aux] = '0';
		hex[aux + 1] = '\0';
	}
	*saida = hex;
	free(*entrada);
	//printf("%s\n", C);
	freeTree(fpm->array[0]);
	free(fpm->array);
	free(fpm);
	aux = strlen(hex);
	return aux;
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int qntLinhas;
    fscanf(input, "%d", &qntLinhas);
    
    int qntBits;
    bit* rle;
    bit* huf;
    char* string;
    char* string2;
    for(int i = 0; i < qntLinhas; i++){
		fscanf(input, "%d", &qntBits);
		rle = (bit*)malloc(sizeof(bit)*qntBits);
		huf = (bit*)malloc(sizeof(bit)*qntBits);
		for(int j = 0; j < qntBits; j++){
			fscanf(input, " %c%c", &rle[j].s1, &rle[j].s2);
			huf[j].s1 = rle[j].s1;
			huf[j].s2 = rle[j].s2;
		}
		int rleLen = comprimeRLE(&rle, qntBits, &string2);
		int hufLen = comprimeHUF(&huf, qntBits, &string);
		qntBits = qntBits * 2;
		float rlePorc = ((float)rleLen)/qntBits * 100.0;
		float hufPorc = ((float)hufLen)/qntBits * 100.0;
		//printf("huflen: %d rlelen: %d\n", hufLen, rleLen);
		if(hufPorc <= rlePorc)
			fprintf(output, "%d->HUF(%.2f%%)=%s\n", i, hufPorc, string);
		if(rlePorc <= hufPorc)
			fprintf(output, "%d->RLE(%.2f%%)=%s\n", i, rlePorc, string2);
	}
	free(string);
	free(string2);
}
