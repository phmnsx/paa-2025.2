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
	int tamanhoBin;
} no;

typedef struct arvore{
	int tamanho;
	int capacidade;
	no** array;
} arvore;

static char binToHexTable[16] = "0123456789ABCDEF";

void binToHex(char* C, char* hex){
    int len = strlen(C);
    int hexLen = (len + 3) / 4;
    
    for(int i = 0; i < hexLen; i++){
        int val = 0;
        int start = i * 4;
        for(int j = 0; j < 4; j++){
            if(start + j < len){
                val = (val << 1) | (C[start + j] - '0');
            } else {
                val <<= 1;
            }
        }
        hex[i] = binToHexTable[val];
    }
    hex[hexLen] = '\0';
}

static char hexTable[256][3] = {0};
/*void printBits(bit* entrada, int tamanho, FILE* saida){
	for(int i = 0; i < tamanho; i++){
		fprintf(saida, "%c%c ", entrada[i].s1, entrada[i].s2);
	}
	fprintf(saida, "\n");
}*/

int igual(char* a, char* b){
	return (a[0] == b[0] && a[1] == b[1]);
}

bit toBit(int num){
	bit resp;
	if(num == -1){
		resp.s1 = 'G';
		resp.s2 = 'G';
		return resp;
	}
	resp.s1 = hexTable[num][0];
	resp.s2 = hexTable[num][1];
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

int comprimeRLE(char** entrada, int tamanho, char** saida){
	char* string = *entrada;
	char* resultado = (char*)malloc(sizeof(char)*2*tamanho + 1);
	int i = 0, j = 0; //i percorre entrada e j percorre resultado
	int contagem = 1;
	while(i <= tamanho){
		if(i == tamanho){
			free(*entrada);
			resultado[j] = '\0';
			*saida = resultado;
			return j;
		}

		if(	string[i] == string[i+2] && //AA AA
			string[i+1] == string [i+3] && i + 3 < tamanho){
			contagem++;
			i+=2;
		}
		else{
			resultado[j] = hexTable[contagem][0]; //adicione a contagem ao espaço j
			resultado[j+1] = hexTable[contagem][1];
			resultado[j+2] = string[i]; //adicione o bit contado j vezes em j + 1
			resultado[j+3] = string[i+1];
			j += 4; //coloque j pra depois do bit
			contagem = 1;
			i += 2;
		}
	}
	return -1;
}

void swap(no** a, no** b){
	no* t = *a;
	*a = *b;
	*b = t;
}

void siftUp(arvore* h, int i){
	while(i>0){
        int p=(i-1)/2;
        if(h->array[i]->freq >= h->array[p]->freq) break;
        swap(&h->array[i], &h->array[p]);
        i=p;
    }
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
	int i = heap->tamanho++;
    heap->array[i] = novoNo;
}

void inserirOrd(arvore* heap, no* novoNo){
	heap->array[heap->tamanho] = novoNo;
	siftUp(heap,heap->tamanho);
	heap->tamanho++;
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
	newnode->tamanhoBin = 0;
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

void gerarTabela(no* raiz, char* codigo, int prof, char T[][256], int tamanhos[]){
	if(!raiz) return;
	
	if(!(raiz->E) && !(raiz->D)){
		if(!(raiz->s.s1 == 'G')){
			if(prof == 0){
				codigo[0] = '0';
				codigo[1] = '\0';
				prof = 1;
			}
			else
				codigo[prof]='\0';
			int idx = toInt(raiz->s);
			tamanhos[idx] = prof;
			strcpy(T[idx], codigo);
		}
		return;
	}
	if(raiz->E){
		codigo[prof] = '0';
		
		gerarTabela(raiz->E, codigo, prof+1, T, tamanhos);
	}
	if(raiz->D){
		codigo[prof] = '1';
		
		gerarTabela(raiz->D, codigo, prof+1, T, tamanhos);
	}
}
void freeTree(no* t){
	if(t->E)
		freeTree(t->E);
	if(t->D)
		freeTree(t->D);
	free(t);
}


int comprimeHUF(char** entrada, int tamanho, char** saida){
	char* string = *entrada;
	int H[256] = {0};
	int tamanhos[256] = {0};
	for(int i = 0; i < tamanho; i++){
		H[hexToInt(string[i*2])*16 + hexToInt(string[i*2+1])]++;
	}

	arvore* fpm = criarHeap(256);
	no* tmp;
	
	for(int i = 0;i < 256; i++){
		if(H[i] > 0){
			tmp = novoNo(toBit(i), H[i]);
			inserir(fpm, tmp);
		}
	}
	//heapify(fpm, 0);
	construirHeap(fpm);
	while(fpm->tamanho > 1){
		no* x = extrairMin(fpm);
		no* y = extrairMin(fpm);
		tmp = novoNo(toBit(-1),x->freq + y->freq);
		tmp->E = x;
		tmp->D = y;
		inserirOrd(fpm, tmp);
		//siftUp(fpm, fpm->tamanho);
	}
	tmp = extrairMin(fpm); //raiz
	static char T[256][256];
	char cod[256];
	gerarTabela(tmp, cod, 0, T, tamanhos);
	char* C = malloc(sizeof(char)*tamanho*256+1);
	char* hex = malloc(sizeof(char)*tamanho*256+1);
	memset(C, 'a', sizeof(char)*tamanho*256);
	C[0] = '\0';
	int endPos = 0;
	for(int i = 0; i < tamanho; i++){
		int idx = hexToInt(string[i*2])*16 + hexToInt(string[i*2+1]);
		char* codigo = T[idx];
		//int x = strlen(codigo);
		int x = tamanhos[idx];
		memcpy(&C[endPos], codigo, x);
		endPos += x;
	}
	C[endPos] = '\0';
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
	free(C);
	aux = strlen(hex);
	return aux;
}

void initHexTable() {
    static int initialized = 0;
    if (!initialized) {
        for(int i = 0; i < 256; i++){
            hexTable[i][0] = "0123456789ABCDEF"[(i >> 4) & 0xF];
            hexTable[i][1] = "0123456789ABCDEF"[i & 0xF];
            hexTable[i][2] = '\0';
        }
        initialized = 1;
    }
}

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    int qntLinhas;
    fscanf(input, "%d", &qntLinhas);
    //printf("Inicio\n");
     char buffer[65536];
    initHexTable();
    int qntBits;
    char* rle;
    char* huf;
    char* string;
	char* string2;
	//int c;
    for(int i = 0; i < qntLinhas; i++){
		fscanf(input, "%d", &qntBits);
		fgetc(input); //tira o whitespace, sobrando apenas os byte
		rle = (char*)malloc(sizeof(char)*qntBits*2 + 1); //AA(whitespace)*3 + 1
		huf = (char*)malloc(sizeof(char)*qntBits*2 + 1);
		fgets(buffer, sizeof(buffer), input);
		char* src = buffer;
        char* dst = rle;
        int count = 0;
         while(*src && count < qntBits*2){
            if(*src != ' '){
                *dst++ = *src;
                count++;
            }
            src++;
        }
		rle[qntBits*2] = '\0';
		//printf("%s\n", rle);
		memcpy(huf, rle, sizeof(char)*qntBits*2 + 1);
		int rleLen = comprimeRLE(&rle, qntBits*2, &string2);
		int hufLen = comprimeHUF(&huf, qntBits, &string);
		qntBits = qntBits * 2;
		float rlePorc = ((float)rleLen)/qntBits * 100.0;
		float hufPorc = ((float)hufLen)/qntBits * 100.0;
		if(hufPorc <= rlePorc)
			fprintf(output, "%d->HUF(%.2f%%)=%s\n", i, hufPorc, string);
		if(rlePorc <= hufPorc)
			fprintf(output, "%d->RLE(%.2f%%)=%s\n", i, rlePorc, string2);
		free(string);
		free(string2);
	}
	
}
