 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct e2{
	int num;
	int indice;
} med;

typedef struct e1{
	int** arrays;
	med resultados[6]; //lp, lm, la, hp, hm, ha
} elem;

void sortAll(elem* V, int tamanhoArray);
void quicksortL(elem* V, int i, int j, int mode);
int max(int x, int y);
int min(int x, int y);
int abs(int x);
void trocar(elem* V, int* x, int* y, int mode);
int lomuto(elem* V, int i, int j, int mode);
int hoare(elem* V, int i, int j, int mode);
void quicksortH(elem* V, int i, int j, int mode);
void insertionSort(med* array, int size);
void saveToFile(elem* V, FILE* output, int tamanhoArray);
char* intToString(int num);
void freeArr(elem* V);

int main(int argc, char* argv[]){
	FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    elem* V = malloc(sizeof(elem));
    
    
	int tamanho; 
    fscanf(input, "%d ", &tamanho);// quantas arrays
    int tamanhoArray;
    for(int i = 0; i < tamanho; i++){
		fscanf(input, "%d ", &tamanhoArray); // tamanho da array
		V->arrays = malloc(sizeof(int*)*6);
		if (V->arrays){
			for (int j = 0; j < 6; j++){
				V->arrays[j] = malloc(sizeof(int)*tamanhoArray);
				V->resultados[j].indice = j; //0 a 5
				V->resultados[j].num = 0;
			}
		}
		for(int j = 0; j < tamanhoArray; j++){
			int num;
			fscanf(input, "%d ", &num);
			for(int k = 0; k < 6; k++){
				V->arrays[k][j] = num;
			}
		}
		sortAll(V, tamanhoArray);
		insertionSort(V->resultados, 6);
		saveToFile(V, output, tamanhoArray);
		freeArr(V);
}
    free(V);
    fclose(input);
    fclose(output);
}

void freeArr(elem* V){
	for(int i = 0; i < 6; i++){
		free(V->arrays[i]);
	}
}

void saveToFile(elem* V, FILE* output, int tamanhoArray){
	fprintf(output, "[%d]:", tamanhoArray);
	for(int i = 0; i < 6; i++){
		fprintf(output,"%s(%d)", intToString(V->resultados[i].indice), V->resultados[i].num);
		if (i != 5){
			fprintf(output,",");
		}
	}
	fprintf(output,"\n");
}

char* intToString(int num){
	switch (num){
		case 0:
		return "LP"; break;
		case 1:
		return "LM"; break;
		case 2:
		return "LA"; break;
		case 3:
		return "HP"; break;
		case 4:
		return "HM"; break;
		case 5:
		return "HA"; break;
		default: return "Help";
	}
}

void sortAll(elem* V, int tamanhoArray){
	quicksortL(V, 0, tamanhoArray - 1, 0);
	quicksortL(V, 0, tamanhoArray - 1, 1);
	quicksortL(V, 0, tamanhoArray - 1, 2);
	
	quicksortH(V, 0, tamanhoArray - 1, 3);
	quicksortH(V, 0, tamanhoArray - 1, 4);
	quicksortH(V, 0, tamanhoArray - 1, 5);
}

void quicksortL(elem* V, int i, int j, int mode){
	V->resultados[mode].num++;
	int tamanho = (j-i) + 1;
	int indicePivo = j;
	if(i<j){
		if (mode == 1){ //TODO: fazer mediana estavel la
			med a;
			med b;
			med c;
			a.indice = i+(tamanho/4);
			a.num = V->arrays[mode][a.indice]; 
			b.indice = i+(tamanho/2);
			b.num = V->arrays[mode][b.indice]; 
			c.indice =  i+(3*tamanho/4);
			c.num = V->arrays[mode][c.indice];
			med arr[3];
			arr[0] = a;
			arr[1] = b;
			arr[2] = c;
			insertionSort(arr, 3);
			
			indicePivo = arr[1].indice;
		}
		else if (mode == 2){
			indicePivo = i + abs(V->arrays[mode][i]) % tamanho;
		}
		
		if(mode > 0){
			trocar(V, &V->arrays[mode][indicePivo], &V->arrays[mode][j], mode);
		}
		int p = lomuto(V, i, j, mode);
		
		quicksortL(V, i, p-1, mode);
		quicksortL(V, p+1, j, mode);
	}
}

int lomuto(elem* V, int i, int j, int mode){
	int p = V->arrays[mode][j], x = i - 1;
	for(int y = i; y < j; y++){
		if(V->arrays[mode][y] <= p)
			trocar(V, &V->arrays[mode][++x], &V->arrays[mode][y], mode);
	}
	trocar(V, &V->arrays[mode][++x], &V->arrays[mode][j], mode);
	return x;
}

void quicksortH(elem* V, int i, int j, int mode){
	V->resultados[mode].num++;
	int tamanho = (j-i) + 1;
	int indicePivo = j;
	if(i<j){
		if (mode == 4){
			med a;
			med b;
			med c;
			a.indice = i+(tamanho/4);
			a.num = V->arrays[mode][a.indice]; 
			b.indice = i+(tamanho/2);
			b.num = V->arrays[mode][b.indice]; 
			c.indice =  i+(3*tamanho/4);
			c.num = V->arrays[mode][c.indice];
			med arr[3];
			arr[0] = a;
			arr[1] = b;
			arr[2] = c;
			insertionSort(arr, 3);
			
			indicePivo = arr[1].indice;
		}
		else if (mode == 5){
			indicePivo = i + abs(V->arrays[mode][i]) % tamanho;
		}
		
		if(mode > 3){
			trocar(V, &V->arrays[mode][indicePivo], &V->arrays[mode][i], mode);
		}
		int p = hoare(V, i, j, mode);
		
		quicksortH(V, i, p, mode);
		quicksortH(V, p+1, j, mode);
	}
}

int hoare(elem* V, int i, int j, int mode){
	int x = i - 1, y = j + 1, p = V->arrays[mode][i];
	while(1){
			while(V->arrays[mode][--y] > p);
			while(V->arrays[mode][++x] < p);
			if (x < y){
				trocar(V, &V->arrays[mode][y], &V->arrays[mode][x], mode);
			}
			else return y;
		}
}

void insertionSort(med* array, int size){
	int i, j;
	med key;
	for(i = 1; i < size; i++){
		key = array[i];
		j = i - 1;
		while (j >= 0 && array[j].num > key.num){
			array[j + 1] = array[j];
			j = j - 1;
		}
		array[j + 1] = key;
	}
	
}

void trocar(elem* V, int* x, int* y, int mode){
	int aux = *x;
	*x = *y;
	*y = aux;
	V->resultados[mode].num++;
}

void trocar2(int** x, int** y){
	int* aux = *x;
	*x = *y;
	*y = aux;
}

int max(int x, int y){
	if (x > y)
		return x;
	else
		return y;
}

int min(int x, int y){
	if (x < y)
		return x;
	else
		return y;
}

int abs(int x){
	if (x > 0)
		return x;
	else
		return (-1)*x;
}
