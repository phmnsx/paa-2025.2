#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct Veiculo{
	char placa[8];
	int peso;
	int volume;
} veiculo;

typedef struct Produto{
	char codigo[14];
	float valor;
	int peso;
	int volume;
	int usado;
	int indice;
} produto;

typedef struct aux{
	int posicao;
	int valor;
} dupla;

int maior(int a, int b){
	if(a > b)
		return a;
	return b;
}

void printResultados(produto* pLista, int* pUsados, veiculo ve, int qnt , int v, int p, float max,FILE* out){
	float pp = (float)p/ve.peso * 100.0;
    float pv = (float)v/ve.volume * 100.0;
	fprintf(out, "[%s]R$%.2f,%dKG(%.0f%%),%dL(%.0f%%)->", ve.placa, max, p, pp, v, pv);
	for(int i = qnt - 1; i >= 0; i--){
		fprintf(out,"%s", pLista[pUsados[i]].codigo);
		if(i != 0)
		fprintf(out, ",");
	}
	fprintf(out, "\n");
}


void solveMochila(veiculo v, produto* pLista, int tamanhoTot, FILE* out)
{
	int t = (v.peso+1)*(v.volume+1);
    float* dp = (float*)calloc(t, sizeof(float));
    int* choice = calloc(t, sizeof(int));
    for(int i = 0; i < t; i++)
		choice[i] = -1;
    
    for(int i = 0; i < tamanhoTot; i++){
		if(pLista[i].usado == 0){
			for(int p = v.peso; p >= pLista[i].peso; p--){
				int var = p*(v.volume + 1);
				int var2 = (p-pLista[i].peso)*(v.volume + 1);
				for(int vv = v.volume; vv >= pLista[i].volume; vv--){
					float pn = dp[var2 + (vv - pLista[i].volume)] + pLista[i].valor;
					if(pn > dp[var + vv]){
						dp[var + vv] = pn;
						choice[var + vv] = i;
					}
				} 
			}
		}
	}
	int* pUsados = (int*) malloc(sizeof(int)*tamanhoTot);
	int ind = 0;
	//backtrack eba
	int i = v.peso;
	int j = v.volume;
	int idx = 0;
	while( i >= 0 && j >= 0 && idx >= 0){
		idx = choice[(i)*(v.volume+1)+(j)];
		if(idx >= 0){
			pUsados[ind++] = idx;
			pLista[idx].usado = 1;
			i -= pLista[idx].peso;
            j -= pLista[idx].volume;
		}
	}
	int pesoTotal = 0;
	int volumeTotal = 0;
	for (int i = 0; i < ind;i++){
		pesoTotal += pLista[pUsados[i]].peso;
		volumeTotal  += pLista[pUsados[i]].volume;
    }

    printResultados(pLista, pUsados, v, ind, volumeTotal, pesoTotal, dp[(v.peso)*(v.volume+1)+(v.volume)],out);
}


int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
	//FILE* input = fopen("input.txt", "r");
    //FILE* output = fopen("output.txt", "w");
    int tamanhoVeiculos, tamanhoProdutos;
    fscanf(input,"%d", &tamanhoVeiculos);
    veiculo* vLista = malloc(sizeof(veiculo)*tamanhoVeiculos);
    for(int i = 0; i < tamanhoVeiculos; i++){
		fscanf(input, "%s %d %d", vLista[i].placa, &vLista[i].peso, &vLista[i].volume);
	}
	fscanf(input,"%d", &tamanhoProdutos);
	produto* pLista = malloc(sizeof(produto)*tamanhoProdutos);
	for(int i = 0; i < tamanhoProdutos; i++){
		fscanf(input, "%s %f %d %d", pLista[i].codigo, &pLista[i].valor, &pLista[i].peso, &pLista[i].volume);
		pLista[i].usado = 0;
		pLista[i].indice = i;
	}
	for(int i = 0; i < tamanhoVeiculos; i++){
		solveMochila(vLista[i], pLista, tamanhoProdutos, output);
	}
	float precoTot = 0;
	int pesoTot = 0;
	int volTot = 0;
	produto* pListaAux = malloc(tamanhoProdutos*sizeof(produto));
	int j = 0;
	for(int i = 0; i < tamanhoProdutos; i++)
		if(pLista[i].usado == 0){
			pListaAux[j++] = pLista[i];
			precoTot += pLista[i].valor;
			pesoTot += pLista[i].peso;
			volTot += pLista[i].valor;
		}
	if(precoTot != 0){
		fprintf(output, "PENDENTE:R$%.2f,%dKG,%dL->", precoTot, pesoTot, volTot);
		for(int i = 0; i < j; i++){
			fprintf(output,"%s", pListaAux[i].codigo);
			if(i != j - 1)
				fprintf(output,",");
		}
	}
	
	free(pLista);
	free(vLista);
	fclose(input);
	fclose(output);
}

