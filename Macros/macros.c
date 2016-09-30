#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	Esta função retorna um ponteiro para caracteres alocado dinâmicamente com o nome do arquivo gerado após as expansões das macros.
	O arquivo de entrada possui como primeiro elemento da linha o número da linha no arquivo original. 
*/
char *macros_expandidas(char *arq_pre_processado){
	FILE *fp, *final;
	char *nome_arqfinal, nome[16] = "saidaMacros.asm"; /*Nome do arquivo de saída: saidaMacros.asm*/

	fp = fopen(arq_pre_processado, "r");
	if(!fp)
		exit(1); /* Sai se o arquivo pre-processado não for aberto.*/

	final = fopen(nome, "w+");
	if(!final)
		exit(1);/* Sai se o arquivo de saída da expansao de macros não for aberto.*/


	nome_arqfinal = (char *)malloc(16*sizeof(char));
	if(!nome_arqfinal)
		exit(1); /* Sai do programa se não for possível alocar memória para o nome do arquivo de saída. */
	strcpy(nome_arqfinal, nome);


	busca_macros(fp);

	if(expande_macros(fp, final))
		exit(1); /* Se a função retornar algo diferente de 0, houve erro. */





	return nome_arqfinal;
	/* Depois lembrar de desalocar a memória desse ponteiro.*/
}

void busca_macros(FILE *fp){

}

int expande_macros(FILE *fp, FILE *final){

	return 0;
}


int main(){
	return 0;
}