import java.io.FileReader;      
import java.io.FileWriter;            
import java.io.FileNotFoundException; 
import java.io.BufferedReader;
import java.io.BufferedWriter;

class pedroantunes_202400018260_quicksort {
	public static void main (String [] args){
		try{
			BufferedReader br = new BufferedReader(new FileReader(args[0]));
			BufferedWriter output = new BufferedWriter(new FileWriter(args[1]));
			int tamanhoLista = Integer.parseInt(br.readLine());
			SortInfo [] lista = new SortInfo[tamanhoLista];
			int tamanhoLista2;
			for(int i = 0; i < tamanhoLista; i++){
				tamanhoLista2 = Integer.parseInt(br.readLine());
				lista[i] = new SortInfo();
				lista[i].model = new int[tamanhoLista2];
				lista[i].array = new int[tamanhoLista2];
				String regex = "[\\s]";
				String [] inputarr = br.readLine().split(regex);
				
				for(int j = 0; j < tamanhoLista2; j++){
					lista[i].model[j] = Integer.parseInt(inputarr[j]);
				}
			}
			for(int i = 0; i < tamanhoLista; i++){
				runSort(lista[i]);
				sortInside(lista[i]);
				toFile(lista[i], output);
			}
			output.close();
		}
		catch (Exception e){
			e.printStackTrace();
		}
	}
	
	public static void toFile(SortInfo elem, BufferedWriter output) throws Exception{
		StringBuilder sb = new StringBuilder(256);
		sb.append("[").append(elem.array.length).append("]").append(":");
		for(int i = 0; i < 6; i++){
			sb.append(elem.numArrAux[i].toString()).append("(").append(elem.numArrAux[i].passos).append(")");
			if (i != 5){
				sb.append(",");
			}
		}
		//System.out.println(sb.toString());
		output.write(sb.toString());
		output.newLine();
	}
	
	public static void sortInside(SortInfo elem){
		elem.makeArray();
		quicksortElem(elem, 0, 5);
	}
	
	public static void quicksortElem(SortInfo elem, int i, int j){
		if (i < j){
			int p = lomuto2(elem, i, j);
			quicksortElem(elem, i, p - 1);
			quicksortElem(elem, p + 1, j);
		}
	}
	
	public static int lomuto2(SortInfo elem, int i, int j){ //passos = passos, newj = prioridade
		Dupla p = elem.numArrAux[j];
		Dupla aux;
		int x = i - 1, y = i;
		for (y = i; y < j; y++){
			if (elem.numArrAux[y].passos < (p.passos)){
				x++;
				aux = elem.numArrAux[x];
				elem.numArrAux[x] = elem.numArrAux[y];
				elem.numArrAux[y] = aux;
			}
			else if (elem.numArrAux[y].passos == p.passos){
				if(elem.numArrAux[y].newj < p.newj){
					x++;
					aux = elem.numArrAux[x];
					elem.numArrAux[x] = elem.numArrAux[y];
					elem.numArrAux[y] = aux;
				}
			}
		}
		x++;
		aux = elem.numArrAux[x];
		elem.numArrAux[x] = elem.numArrAux[y];
		elem.numArrAux[y] = aux;
		return x;
	}
	
	public static void runSort(SortInfo elem){
		elem.reset();
		quicksortL(elem, 0, elem.model.length - 1, 0);
		elem.reset();
		quicksortL(elem, 0, elem.model.length - 1, 1);
		elem.reset();
		quicksortL(elem, 0, elem.model.length - 1, 2);
		elem.reset();
		quicksortH(elem, 0, elem.model.length - 1, 0);
		elem.reset();
		quicksortH(elem, 0, elem.model.length - 1, 1);
		elem.reset();
		quicksortH(elem, 0, elem.model.length - 1, 2);
		elem.reset();
	}
	
	public static void quicksortH(SortInfo elem, int i, int j, int mode){
		int passos = 0;
		int tamanho = (j-i) + 1;
		int pivo = 0;
		int pivoI = 0;
		
		if (mode == 0)
			elem.hp++;
		else if (mode == 1)
			elem.hm++;
		else if (mode == 2)
			elem.ha++;
			
		if(i<j){
			if(mode == 0){
				pivo = elem.array[i];
			}
			else if (mode == 1){
				int a = elem.array[i + tamanho/4];
				int b = elem.array[i + tamanho/2];
				int c = elem.array[i + 3*tamanho/4];
				int d = a + b + c;
				pivo = d - Math.max(Math.max(a,b),c) - Math.min(Math.min(a,b),c);
				if (pivo == a)
					pivoI = i + tamanho/4;
				else if (pivo == b)
					pivoI = i + tamanho/2;
				else if (pivo == c)
					pivoI = i + 3*tamanho/4;
			}
			else if (mode == 2){
				pivoI = i + (Math.abs(elem.array[i]) % tamanho);
				pivo = elem.array[pivoI];
			}
			if ((mode == 1 || mode == 2)){
				int aux = elem.array[pivoI];
				elem.array[pivoI] = elem.array[i];
				elem.array[i] = aux;
				if(mode == 1)
					elem.hm++;
				else if(mode == 2)
					elem.ha++;
			}
			Dupla p = hoare(elem, i, j);
			if(mode == 0)
				elem.hp += p.passos;
			else if(mode == 1)
				elem.hm += p.passos;
			else if(mode == 2)
				elem.ha += p.passos;
			quicksortH(elem, i, p.newj, mode);
			quicksortH(elem, p.newj + 1, j, mode);
		} 
	}
	
	public static Dupla hoare(SortInfo elem, int i, int j){
		int x = i - 1, y = j + 1, passos = 0;
		int pivo = elem.array[i];
		while(true){
			while(elem.array[--y] > pivo);
			while(elem.array[++x] < pivo);
			if (x < y){
				passos++;
				int aux = elem.array[x];
				elem.array[x] = elem.array[y];
				elem.array[y] = aux;
			}
			else return new Dupla(passos, y);
		}
	}	
	
	public static void quicksortL(SortInfo elem, int i, int j, int mode){
		int passos = 0;
		int tamanho = (j-i) + 1;
		int pivo = 0;
		int pivoI = 0;
		
		if (mode == 0)
		elem.lp++;
		else if (mode == 1)
		elem.lm++;
		else if (mode == 2)
		elem.la++;
		if(i<j){
			if(mode == 0){
				pivo = elem.array[j];
				pivoI = j;
			}
			else if (mode == 1){
				int a = elem.array[i + (tamanho/4)];
				int b = elem.array[i + (tamanho/2)];
				int c = elem.array[i + (3*tamanho/4)];
				int d = a + b + c;
				pivo = d - (Math.max(Math.max(a,b),c) + Math.min(Math.min(a,b),c));
				if(pivo == a)
					pivoI = (tamanho/4) + i;
				else if(pivo == b)
					pivoI = (tamanho/2) + i;
				else if(pivo == c)
					pivoI = (3*tamanho/4) + i;
			}
			else if (mode == 2){
				pivoI = i + (Math.abs(elem.array[i]) % tamanho);
				pivo = elem.array[pivoI];
			}

			if ((mode == 1 || mode == 2)){
				int aux = elem.array[pivoI];
				elem.array[pivoI] = elem.array[j];
				elem.array[j] = aux;
				if(mode == 1)
					elem.lm++;
				else if(mode == 2)
					elem.la++;
			}
			Dupla p = lomuto(elem, i, j);
			if(mode == 0)
				elem.lp += p.passos;
			else if(mode == 1)
				elem.lm += p.passos;
			else if(mode == 2)
				elem.la += p.passos;
			quicksortL(elem, i, p.newj - 1, mode);
			quicksortL(elem, p.newj + 1, j, mode);
		} 
	}
	
	public static Dupla lomuto(SortInfo elem, int i, int j){
		int x = i - 1, y = i, passos = 0;
		int pivo = elem.array[j];
		for (y = i; y < j; y++){               
			if (elem.array[y] <= pivo){         					   
				x++;
				passos++;
				int aux = elem.array[x];
				elem.array[x] = elem.array[y];
				elem.array[y] = aux;
			}
		}
		x++;
		passos++;
		int aux = elem.array[x];
		elem.array[x] = elem.array[j];
		elem.array[j] = aux;
		return new Dupla(passos, x);
	}
	
	public static class Dupla {
		int passos = 0;
		int newj;
		public Dupla(int passos, int newj){
			this.passos = passos;
			this.newj = newj;
		}
		
		public String toString(){
			if(newj == 0){
				return "LP";
			}
			else if (newj == 1){
				return "LM";
			}
			else if (newj == 2){
				return "LA";
			}
			else if (newj == 3){
				return "HP";
			}
			else if (newj == 4){
				return "HM";
			}
			else if (newj == 5){
				return "HA";
			}
			return "";
		}
	}
	
	public static class SortInfo {
		int [] model;
		int [] array;
		int [] numArr = new int[6];
		Dupla [] numArrAux = new Dupla[6];
		int lp = 0; //lomuto padrao
		int lm = 0; //lomuto por mediana de 3
		int la = 0; //lomuto por pivo aleatorio
		int hp = 0; //hoare padrao
		int hm = 0; //hoare por mediana de 3
		int ha = 0; //hoare por pivo aleatorio
		
		public void makeArray(){
			numArrAux[0] = new Dupla(lp,0);
			numArrAux[1] = new Dupla(lm,1);
			numArrAux[2] = new Dupla(la,2);
			numArrAux[3] = new Dupla(hp,3);
			numArrAux[4] = new Dupla(hm,4);
			numArrAux[5] = new Dupla(ha,5);
		}
		
		public void reset(){
			this.array = model.clone();
		}
		public void print(){
			System.out.print("[");
			for(int i = 0; i < this.array.length; i ++){
				System.out.print(" ");
				System.out.print(this.array[i]);
			}
			System.out.println("]");
		}
	}
}
