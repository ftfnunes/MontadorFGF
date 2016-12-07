#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregador.h"
#include "simulador.h"


int *relocador(FILE *fp, chunk *c, int n_chunks){
	char buffer[200], *mapa_bits;
	int tam_prog, i, *prog, codigo_a_carregar, pos_prog, *enderecos_prog;

	fseek(fp, 3, SEEK_SET);

	fgets(buffer, 200, fp); /* Pega "H: " mais o nome do arquivo.*/
	printf("nomearq: %s\n", buffer);
	
	fseek(fp, 3, SEEK_CUR); /* Avança por "H: ".*/
	fscanf(fp,"%d", &tam_prog);/* Pega o tamanho do programa.*/
	printf("tamprog: %d\n", tam_prog);


	if(fgetc(fp) == '\n'){
		printf("É um barra n. \n");
	}

	prog = (int *)malloc((tam_prog)*sizeof(int));/* Aloca memória para o array de int que armazenará o código. */
	enderecos_prog = (int *)malloc((tam_prog)*sizeof(int)); /* Aloca memória para o array de int que armazenará os endereços do código. */ 
	if (!prog || !enderecos_prog)	{
		printf("Memória para alocar o programa não alocada.\n");
		exit(1);
	}

	mapa_bits = (char *)malloc((tam_prog)*sizeof(char));/* Aloca memória para o array de char que armazenará o mapa de bits.*/
	if (!prog)	{
		printf("Memória para alocar o programa não alocada.\n");
		exit(1);
	}

	fseek(fp, 4, SEEK_CUR); /* Avança por "H: ".*/
	for (i = 0; i < tam_prog; ++i){
		mapa_bits[i] = fgetc(fp);
		printf("pegando mapa_bits[%d]: %c\n", i, mapa_bits[i]);
	}

	


	fgetc(fp); /* Pega o '\n' no final da linha. */


	fseek(fp, 3, SEEK_CUR); /* Avança por "T:".*/


	for (i = 0; i < tam_prog; ++i)
		fscanf(fp, "%d", &prog[i]);/* Adquire uma unidade do código, seja instrução ou endereço. */
	

	fgetc(fp); /* Pega o '\n' final ou o EOF.*/


	/* Mapa de bits, estrutura de relocação criada e vetor com o programa criados, falta relocar.*/


	codigo_a_carregar = tam_prog;

	for(i = 0, pos_prog = 0; codigo_a_carregar != 0; ++i){	/* Laço que define os endereços das partes do programa. */

		pos_prog = define_enderecos(enderecos_prog, tam_prog, pos_prog, c[i].endereco, (c[i].endereco+c[i].tam));
		codigo_a_carregar = (((codigo_a_carregar-c[i].tam)<=0)? 0:(codigo_a_carregar-c[i].tam));	
	}

	for (i = 0; i < tam_prog; ++i){
		printf("antes reloca mapa_bits[%d]: %c\n", i, mapa_bits[i]);
	}
	
	reloca(prog, enderecos_prog, mapa_bits, tam_prog);

	for (i = 0; i < tam_prog; ++i){
		printf("enderecos_prog[%d]: %d\n", i, enderecos_prog[i]);
		printf("prog[%d]: %d\n", i, prog[i]);
		printf("mapa_bits[%d]: %c\n", i, mapa_bits[i]);
	}

	printf("Linha 66.\n");
	free(enderecos_prog);
	printf("Linha 68.\n");
	free(mapa_bits);
	return prog;
}

int define_enderecos(int *enderecos_prog, int tam_prog, int pos_prog, int init_chunk, int fim_chunk){
	int i, p_prog = pos_prog;

	for(i = init_chunk; i < fim_chunk && p_prog != tam_prog; ++i){
		enderecos_prog[p_prog] = i;
		++p_prog;
	}

	return p_prog;
}

void reloca(int *prog, int *enderecos_prog, char *mapa_bits, int tam_prog){
	int i;

	for(i = 0; i < tam_prog; ++i){
		if(mapa_bits[i] == '1')
			prog[i] = enderecos_prog[prog[i]];		
	}

}

void imagem_memoria(FILE *saida, int *prog, int tam_prog){
	int i;

	for (i = 0; i < tam_prog; ++i)
		if(i == tam_prog-1)
			fprintf(saida, "%d", prog[i]);
		else
			fprintf(saida, "%d ", prog[i]);
}



int main(int argc, char *argv[]){
	char buffer[200];
	FILE *fp, *saida;
	int n_chunks, i, tam_prog, tamTotalChunks = 0, *prog;
	chunk *c;


	simulacao(argv[1]);


	fp = fopen(argv[1], "r");
	if(!fp){
		printf("Arquivo do ligador não foi aberto.\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_SET);

	fgets(buffer, 200, fp); /* Pega o nome do arquivo.*/

	fseek(fp, 3, SEEK_CUR);/* Avança por "H: ". */
	fgets(buffer, 200, fp); /* Pega o tamanho do programa.*/
	tam_prog = atoi(buffer);

	n_chunks = atoi(argv[2]);
	c = (chunk *)malloc(n_chunks*sizeof(chunk));
	if (!c)	{
		printf("Memória para registro de chunks não alocada.\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_SET);

	for(i = 0; i < n_chunks; ++i){
		c[i].tam = atoi(argv[3+i]);
		c[i].endereco = atoi(argv[3+i+n_chunks]);
		tamTotalChunks += c[i].tam;
	}

	if(tam_prog > tamTotalChunks){
		printf("OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED\n");
		return 0;
	}

	prog = relocador(fp, c, n_chunks);

	saida = fopen("memoria.im", "w+");
	if(!saida){
		printf("Problema na abertura do arquivo de saída.\n");
		exit(1);
	}
	imagem_memoria(saida, prog, tam_prog);


	fclose(fp);
	fclose(saida);
	printf("Linha 162.\n");
	free(c);
	printf("Linha 164.\n");
	free(prog);
	return 0;
}