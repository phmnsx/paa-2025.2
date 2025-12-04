#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Veiculo{
	char placa[8];
	int peso;
	int volume;
} veiculo;

typedef struct Produto{
	char codigo[14];
	double valor;
	double peso;
	double volume;
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
void trocar(produto* x, produto* y){
	produto aux = *x;
	*x = *y;
	*y = aux;
}
int lomuto(produto* pLista, int i, int j){
	int p = pLista[j].indice;
	int x = i - 1;
	for(int y = i; y < j; y++){
		if((pLista[y].indice > p || !(pLista[y].usado))){
			x++;
			trocar(&pLista[x], &pLista[y]);
		}
	}
	trocar(&pLista[x+1], &pLista[j]);
	return x + 1;
}

void quicksort(produto* pLista, int i, int j){
	if(i<j){
		int p = lomuto(pLista, i, j);
		quicksort(pLista, i, p-1);
		quicksort(pLista, p+1, j);
	}
}

int solveMochila(veiculo v, produto* pLista, int tamanhoProdutos){ 
	//a matriz vai ser de tamanho 100 x 100 x tamanhoProdutos
	int ***dp = (int***)malloc((tamanhoProdutos + 1) * sizeof(int**));
	if(dp == NULL) {
    printf("Erro de alocação de memória\n");
    return 0;
	}
    for(int i = 0; i <= tamanhoProdutos; i++){
        dp[i] = (int**)malloc(101 * sizeof(int*));
        if(dp[i] == NULL) {
			printf("Erro de alocação de memória\n");
			return 0;
		}
        for(int w = 0; w <= 100; w++){
            dp[i][w] = (int*)calloc(101, sizeof(int));
            if(dp[i][w] == NULL) {
			printf("Erro de alocação de memória\n");
			return 0;
		}
        }
    }

	//cada i vai ser o peso/volume disponivel, sendo n com i/100 * peso/volume do veiculo

	for(int i = 0; i <= tamanhoProdutos; i++){ // i = w
		for(int wp = 0; wp <= 100; wp++){
			for(int wv = 0; wv <= 100; wv++){
				double cp = wp * v.peso / 100.0;
				double cv = wv * v.volume / 100.0;
				//se nao cabe
				if (i == 0 || wv == 0 || wp == 0)
					dp[i][wp][wv] = 0;
				else if (pLista[i].peso > cp || pLista[i].volume > cv)
					dp[i][wp][wv] = dp[i-1][wp][wv];
				else{
					int nwp = wp - (int)(pLista[i].peso * 100.0/v.peso);
					int nwv = wv - (int)(pLista[i].volume * 100.0/v.volume);
					if(nwp < 0) nwp = 0;
					if(nwv < 0) nwv = 0;
					dp[i][wp][wv] = maior(dp[i][wp][wv], dp[i][nwp][nwv] + pLista[i].valor);
				}
			}
		}
	}
	//considerando a matriz correta, selecionar os itens adicionados, e tambem remover o item da lista geral
	int i = tamanhoProdutos;
	int wp = 100;
	int wv = 100;
	int escolhidos = 0;

	while(i > 0 && (wv > 0 || wp > 0)){
		if(dp[i][wp][wv] != dp[i-1][wp][wv]){ //se for diferente, o item foi adicionado
			//printf("%d, %d, diferenca: %d", dp[i][w], dp[i-1][w], dp[i][w] - dp[i-1][w]);
			//printf("added: %s\n ", pLista[i].codigo);
			escolhidos++;
			pLista[i].usado = 1; //marca como usado
			
			wp = wp - (int)(pLista[i].peso*100.0/v.peso);
			wv = wv - (int)(pLista[i].volume*100.0/v.volume);
			i = i - 1;
		}
		else{
			i = i - 1;
		}
	}
	
	for(int i = 0; i <= tamanhoProdutos; i++) {
        for(int w = 0; w <= 100; w++) {
            free(dp[i][w]);
        }
        free(dp[i]);
    }
    free(dp);

	return escolhidos;
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
		fscanf(input, "%s %lf %lf %lf", pLista[i].codigo, &pLista[i].valor, &pLista[i].peso, &pLista[i].volume);
		pLista[i].usado = 0;
		pLista[i].indice = i;
	}
	int e = 0;
	for(int i = 0; i < tamanhoVeiculos; i++){
		e += solveMochila(vLista[i], pLista, tamanhoProdutos - e - 1);
		//printf("\n");
		quicksort(pLista, 0, tamanhoProdutos - 1);
	}
	free(pLista);
	free(vLista);
	fclose(input);
	fclose(output);
}

