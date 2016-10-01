#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"


/*
	Esta função retorna um ponteiro para caracteres alocado dinâmicamente com o nome do arquivo gerado após as expansões das macros.
	O arquivo de entrada possui como primeiro elemento da linha o número da linha no arquivo original. 
*/
char *macros_expandidas(char *arq_pre_processado){
	FILE *fp, *final;
	char *nome_arqfinal, nome[16] = "saidaMacros.asm"; /*Nome do arquivo de saída: saidaMacros.asm*/
	lMacro *listaMacros;

	fp = fopen(arq_pre_processado, "r");
	if(!fp){
		printf("Erro ao abrir o arquivo 'arq_pre_processado'.\n");
		exit(1); /* Sai se o arquivo pre-processado não for aberto.*/
	}

	final = fopen(nome, "w+");
	if(!final){
		printf("Erro ao abrir o arquivo 'final'.\n");
		exit(1);/* Sai se o arquivo de saída da expansao de macros não for aberto.*/
	}


	nome_arqfinal = (char *)malloc(16*sizeof(char));
	if(!nome_arqfinal){
		printf("Erro ao alocar memória para o 'nome_arqfinal'.\n");
		exit(1); /* Sai do programa se não for possível alocar memória para o nome do arquivo de saída. */
	}
	strcpy(nome_arqfinal, nome);


	listaMacros = busca_macros(fp);

	if(expande_macros(fp, final)){
		printf("Erro no retorno da função 'expande_macros'.\n");
		exit(1); /* Se a função retornar algo diferente de 0, houve erro. */
	}





	return nome_arqfinal;
	/* Depois lembrar de desalocar a memória desse ponteiro.*/
}

lMacro *busca_macros(FILE *fp){
	char bufferLinha[300], **buffers;
	FILE *auxFile;
	lMacro *lista, *auxMacro;
	int linha, i, tokensNaLinha, auxInt;


	if(!fp){
		printf("Arquivo na função \"busca_macros\" inválido.\n");
		exit(1); /* Sai se o arquivo passado não for válido. */
	}

	buffers = (char **)malloc(7*sizeof(char *));
	if(!buffers){
		printf("Erro ao alocar memória na funcao 'busca_macros' em 'buffers'.\n");
		exit(1);
	}

	for(i = 0; i < 7; ++i){
		buffers[i] = (char *)malloc(60*sizeof(char));
	}


	auxFile = fp;
	while(fgets(bufferLinha, 300, fp) != NULL){
		/* TokensNaLinha é o número de strings efetivamente lidas.*/
		tokensNaLinha = sscanf(bufferLinha, "%d %s %s %s %s %s %s %s", &linha, buffers[0], buffers[1], buffers[2], 
								buffers[3], buffers[4], buffers[5], buffers[6]);

		to_upcase(buffers, 1, strlen(buffers[1]));

		if(isLabel(buffers[0]) == 0 && strcmp(buffers[1], "MACRO") == 0)
			fp = registra_macro(auxFile, &lista); /* Encontrou a label de uma macro e começa o processo de registá-la.*/

	}

	
	for(i = 0; i < 7; ++i){
		free(buffers[i]);
	}
	free(buffers);

	return lista;
}


FILE *registra_macro(FILE *fp, lMacro **lista){
	char **buffers, bufferLinha[300];
	int linha, i, tokensLidos;
	lMacro *new;

	buffers = (char **)malloc(7*sizeof(char *));
	if(!buffers){
		printf("Erro ao alocar memória na funcao 'registra_macro' em 'buffers'.\n");
		exit(1);
	}
	for(i = 0; i < 7; ++i)
		buffers[i] = (char *)malloc(60*sizeof(char));


	new = createLMacro();

	fgets(bufferLinha, 300, fp);
	tokensLidos = sscanf(bufferLinha, "%d %s %s %s %s %s %s %s", &linha, buffers[0], buffers[1], buffers[2], 
								buffers[3], buffers[4], buffers[5], buffers[6]);
	new->linha = linha;
	buffers[0][strlen(buffers[0])-1] = '\0';
	new->def = (char *)malloc((strlen(buffers[0]))*sizeof(char));
	strcpy(new->def, buffers[0]);

	/*

	-> É preciso pegar os argumentos das macros.
	-> Caso uma linha esteja vazia, é só ver a variável tokensLidos, que irá ser igual a 1.

	*/

	while(fgets(bufferLinha, 300, fp) != NULL){
		/* TokensNaLinha é o número de strings efetivamente lidas.*/
		tokensLidos = sscanf(bufferLinha, "%d %s %s %s %s %s %s %s", &linha, buffers[0], buffers[1], buffers[2], 
								buffers[3], buffers[4], buffers[5], buffers[6]);

	}


	for(i = 0; i < 7; ++i){
		free(buffers[i]);
	}
	free(buffers);
	return fp;
}

int expande_macros(FILE *fp, FILE *final){

	return 0;
}

lMacro *createLMacro(){
	lMacro *new = (lMacro *)malloc(sizeof(lMacro));

	if(!new){
		printf("Erro ao alocar memória na funcao 'createLMacro'. \n");
		exit(1);
	}
	return new;
}

lMacro *addMacro(char *label, char *def, int linha, lMacro **lista){
	lMacro *new = createLMacro();

	new->label = label;
	new->def = def;
	new->linha = linha;

	if((*lista) == NULL)
		new->prox = NULL;
	else
		new->prox = (*lista)->prox;

	(*lista) = new;

	return (*lista);
}

int liberaLMacro(lMacro **lista){
	lMacro *aux1 = (*lista), *aux2;

	while(aux1 != NULL){
		aux2 = aux1->prox;

		if(aux1->label != NULL)
			free(aux1->def);
		if(aux1->def != NULL)
			free(aux1->def);

		free(aux1);

		aux1 = aux2;
	}

	return 0;
}


/* Retorna 0 caso str for um numero.*/
int isNumber(char *str){
	int len = strlen(str), ret = 0;

	for(; len >= 0 ; --len){
		if(str[len] < '0' || str[len] > '9')
			++ret;
	}

	return ret;
}

/* Retorna 0 caso a string recebida for um label.*/
int isLabel(char *str){

	if(!str){
		printf("Parâmetro na funcao 'isLabel' recebido nulo.\n");
		exit(1);
	}

	if(str[strlen(str)-1] == ':')
		return 0;
	else
		return 1;
}

void to_upcase(char **buffers, int word, int len){
	int i;

	for(i = 0; i < len; ++i){
		if(buffers[word][i] < 'A' || buffers[word][i] > 'Z')
			buffers[word][i] -= 32;
	}

}

int main(){
	return 0;
}