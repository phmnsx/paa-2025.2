import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

public class edugabriel_202400017719_sequenciamento {

    static class Doenca {
        String nome;
        int porcentagem;
    }
    static int[] charMap = new int[128];
    static {
        charMap['A'] = 0; charMap['C'] = 1; charMap['G'] = 2; charMap['T'] = 3;
    }

    static long[] tabelaHashChaves;
    static boolean[] tabelaHashOcupado;
    static int tamanhoTabela;

    public static void inicializarTabelaHash(int tamanhoDna) {
        tamanhoTabela = 1;
        while (tamanhoTabela < tamanhoDna * 2) {
            tamanhoTabela <<= 1;
        }
        tabelaHashChaves = new long[tamanhoTabela];
        tabelaHashOcupado = new boolean[tamanhoTabela];
    }

    public static void inserirHash(long chave) {
        int indice = (int)(chave & (tamanhoTabela - 1));

        while (tabelaHashOcupado[indice]) {
            if (tabelaHashChaves[indice] == chave) return;
            indice = (indice + 1) & (tamanhoTabela - 1);
        }

        tabelaHashOcupado[indice] = true;
        tabelaHashChaves[indice] = chave;
    }

    public static boolean contemHash(long chave) {
        int indice = (int)(chave & (tamanhoTabela - 1));

        while (tabelaHashOcupado[indice]) {
            if (tabelaHashChaves[indice] == chave) return true;
            indice = (indice + 1) & (tamanhoTabela - 1);
        }
        return false;
    }

    public static void main(String[] args) {
        if (args.length < 2) return;
        try {
            BufferedReader input = new BufferedReader(new FileReader(args[0]));
            BufferedWriter output = new BufferedWriter(new FileWriter(args[1]));

            String linha = input.readLine();
            if (linha == null) { input.close(); output.close(); return; }
            int k = Integer.parseInt(linha.trim());

            String linhaDna = input.readLine();
            while (linhaDna != null && linhaDna.trim().isEmpty()) linhaDna = input.readLine();
            if (linhaDna == null) { input.close(); output.close(); return; }
            char[] dnaPessoa = linhaDna.trim().toCharArray();

            inicializarTabelaHash(dnaPessoa.length);

            long hashDna = 0;
            long mascaraBits = 0;
            if (k < 32) {
                mascaraBits = (1L << (2 * k)) - 1;
            } else {
                mascaraBits = -1L;
            }
			System.out.println(mascaraBits);
            for (int i = 0; i < dnaPessoa.length; i++) {
                hashDna = ((hashDna << 2) | charMap[dnaPessoa[i]]) & mascaraBits;

                if (i >= k - 1) {
                    inserirHash(hashDna);
                }
            }

            Doenca[] resultados = new Doenca[2000];
            int indice = 0;

            while ((linha = input.readLine()) != null) {
                linha = linha.trim();
                if (linha.isEmpty() || linha.matches("\\d+")) continue;

                int len = linha.length();
                int pos = 0;

                int start = pos;
                while (pos < len && !Character.isWhitespace(linha.charAt(pos))) pos++;
                String nomeDoenca = linha.substring(start, pos);

                while (pos < len && Character.isWhitespace(linha.charAt(pos))) pos++;

                if (pos >= len) continue;
                start = pos;
                while (pos < len && !Character.isWhitespace(linha.charAt(pos))) pos++;
                int qtdGenes = Integer.parseInt(linha.substring(start, pos));

                int genesPresentes = 0;

                for (int g = 0; g < qtdGenes; g++) {
                    while (pos < len && Character.isWhitespace(linha.charAt(pos))) pos++;
                    if (pos >= len) break;

                    start = pos;
                    while (pos < len && !Character.isWhitespace(linha.charAt(pos))) pos++;

                    int tamGene = pos - start;

                    boolean[] coberto = new boolean[tamGene];
                    int countCoberto = 0;

                    long hashGene = 0;

                    for (int i = 0; i < tamGene; i++) {
                        char c = linha.charAt(start + i);
                        hashGene = ((hashGene << 2) | charMap[c]) & mascaraBits;

                        if (i >= k - 1) {
                            if (contemHash(hashGene)) {
                                for (int j = 0; j < k; j++) {
                                    int p = i - j;
                                    if (!coberto[p]) {
                                        coberto[p] = true;
                                        countCoberto++;
                                    }
                                }
                            }
                        }
                    }
					System.out.println(countCoberto);
                    int percent = ((countCoberto * 100) + (tamGene / 2)) / tamGene;
                    if (percent >= 90) {
                        genesPresentes++;
                    }
                }

                int porcentagemFinal = ((genesPresentes * 100) + (qtdGenes / 2)) / qtdGenes;

                if (indice == resultados.length) {
                    Doenca[] novo = new Doenca[resultados.length * 2];
                    System.arraycopy(resultados, 0, novo, 0, resultados.length);
                    resultados = novo;
                }

                Doenca d = new Doenca();
                d.nome = nomeDoenca;
                d.porcentagem = porcentagemFinal;
                resultados[indice++] = d;
            }

            countingSortDoencas(resultados, indice);

            for (int i = 0; i < indice; i++) {
                output.write(resultados[i].nome + "->" + resultados[i].porcentagem + "%");
                output.newLine();
            }

            input.close();
            output.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void countingSortDoencas(Doenca[] array, int n) {
        int[] count = new int[101];
        for (int i = 0; i < n; i++) count[array[i].porcentagem]++;
        int[] startPos = new int[101];
        int total = 0;
        for (int i = 100; i >= 0; i--) {
            startPos[i] = total;
            total += count[i];
        }
        Doenca[] output = new Doenca[n];
        for (int i = 0; i < n; i++) {
            output[startPos[array[i].porcentagem]++] = array[i];
        }
        for (int i = 0; i < n; i++) array[i] = output[i];
    }
}
