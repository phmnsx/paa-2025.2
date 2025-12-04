import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

class pedroantunes_202400018260_porto {
	public static void main(String [] args){
		try{
			BufferedReader br = new BufferedReader(new FileReader(args[0]));
			BufferedWriter output = new BufferedWriter(new FileWriter(args[1]));
			String linha;
			Container [] lista;
			Container [] check;
			Container [][][][][] hashArr = new Container[26][26][26][26][10]; //4 letras, e caso repita numero tem o array de 10 ali
			
			int tamanhoLista = Integer.parseInt(br.readLine()); //primeira linha é um número
			int bufferSize = tamanhoLista; 
			String str;
			Container ctn;
			Container [] buffer = new Container[tamanhoLista];
			for(int i = 0; i < tamanhoLista; i++){
				str = br.readLine();
				ctn = stringTcontainer(str);
				buffer[i] = ctn;
				for (int j = 0; j < 10; j++){
					if (hashArr[hash(str.charAt(0))][hash(str.charAt(1))][hash(str.charAt(2))][hash(str.charAt(3))][j] == null)
						hashArr[hash(str.charAt(0))][hash(str.charAt(1))][hash(str.charAt(2))][hash(str.charAt(3))][j] = ctn;
				}
			}
			lista = buffer;
			tamanhoLista = Integer.parseInt(br.readLine());
			buffer = new Container[tamanhoLista];
			for(int i = 0; i < tamanhoLista; i++){
				buffer[i] = stringTcontainer(br.readLine());
			}
			check = buffer;
			buffer = lista.clone();
			busca(hashArr, check);
			mergesort(lista, buffer, 0, bufferSize-1);
			salvar(lista, output, bufferSize);
			output.close();
		}
		catch (Exception e){
			e.printStackTrace();
		}
	}
	
	public static int hash(char input){
		int a = input - 65;
		return a;
	}
	
	public static void salvar(Container [] lista, BufferedWriter output, int bufferSize) throws Exception {	
		StringBuilder sb = new StringBuilder(256);
		for(int i = 0; i < bufferSize; i++){
			sb.setLength(0);
				if (lista[i].printable){
					if(lista[i].erroPJ){
						sb.append(lista[i].id).append(":").append(lista[i].cnpj).append("<->").append(lista[i].newcnpj);
						output.write(sb.toString());
						output.newLine();
						
					}
					else{
						sb.append(lista[i].id).append(":").append(lista[i].erroPeso).append("kg").append("(").append(lista[i].erroPesoPerc).append("%)");
						output.write(sb.toString());
						output.newLine();
					}
					
				}
			}
	}
	public static void mergesort(Container [] lista, Container [] buffer, int i, int j){
		int meio = i + ((j - i) / 2);
		if (j>i){
			mergesort(lista, buffer, meio+1, j);
			mergesort(lista, buffer, i, meio);
			juntar(lista, buffer, i, meio, j);
		}
	}
	
	public static void juntar(Container [] lista, Container [] buffer, int i, int m, int j){
		int i1 = i, i2 = m + 1, k = i;
		while (i1 <= m && i2 <= j){
			if (compare(buffer[i1], buffer[i2]))
				lista[k++] = buffer[i1++];
			else
				lista[k++] = buffer[i2++];
		}
		if(i1 == m + 1){
			while(i2<=j){
				lista[k++] = buffer[i2++];
			}
		}
		else if (i2 == j + 1){
			while(i1<=m){
				lista[k++] = buffer[i1++];
			}
		}
		for(int ii = i; ii <= j; ii++){
			buffer[ii] = lista[ii];
		}
		//buffer = lista.clone();
	}
	public static boolean compare(Container a, Container b){
		if(!a.erroPJ && !b.erroPJ) //se os dois n sao erro de pj
			return a.erroPesoPerc >= b.erroPesoPerc;
		else if (!a.erroPJ && b.erroPJ) // false true
			return false;
		else 							// true ture | true false
			return true;
	} 
	
	public static void busca(Container [][][][][] hashArr, Container [] lista2){
		Container ctn;
		for (Container elem2 : lista2){
			for(int i = 0; i < 10; i++){
				ctn = hashArr[hash(elem2.id.charAt(0))][hash(elem2.id.charAt(1))][hash(elem2.id.charAt(2))][hash(elem2.id.charAt(3))][i];
				if (ctn != null && ctn.id.equals(elem2.id));
					check(ctn, elem2);
			}
		}
	}
	
	public static void check(Container alvo, Container c){
		if (!((alvo.cnpj).equals(c.cnpj))){
			alvo.printable = true;
			alvo.erroPJ = true;
			alvo.newcnpj = c.cnpj;

		}
		else if (Math.abs(((alvo.peso - c.peso)/alvo.peso)) > 0.1){
			alvo.printable = true;
			alvo.erroPesoPerc = (int) Math.round(Math.abs(((alvo.peso - c.peso)/alvo.peso))*100);
			alvo.erroPJ = false;
			alvo.erroPeso = Math.abs((int)(alvo.peso - c.peso));
			
		}
	}
	
	public static Container stringTcontainer(String input){
		Container c = new Container();
		String regex = "[\\s]";
		String [] inputarr = input.split(regex);
		c.id = inputarr[0];
		c.cnpj = inputarr[1];
		c.peso = Integer.parseInt(inputarr[2]);
		c.printable = false;
		c.erroPeso = 0;
		c.erroPJ = false;
		return c;
	}
	public static class Container {
		String id;
		String cnpj;
		String newcnpj;
		double peso;
		boolean printable = false;
		int erroPesoPerc;
		int erroPeso;
		boolean erroPJ = false;
	}
}
