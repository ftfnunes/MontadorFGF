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

	if(expande_macros(fp, final, &listaMacros)){
		printf("Erro no retorno da função 'expande_macros'.\n");
		exit(1); /* Se a função retornar algo diferente de 0, houve erro. */
	}



	fclose(fp);
	fclose(final);
	return nome_arqfinal;
	/* Depois lembrar de desalocar a memória desse ponteiro.*/
}

lMacro *busca_macros(FILE *fp){
	char bufferLinha[300], **buffers;
	FILE *auxFile;
	lMacro *lista = NULL, *new;
	int linha, i, tokensLidos;


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
		tokensLidos = sscanf(bufferLinha, "%d %s %s %s %s %s %s %s", &linha, buffers[0], buffers[1], buffers[2], 
								buffers[3], buffers[4], buffers[5], buffers[6]);

		if(tokensLidos == 1){
			for(i = 0; i < 7; ++i)
				strcpy(buffers[i], "-1");
			continue;
		}

		to_upcase(buffers, 1, strlen(buffers[1]));

		if(isLabel(buffers[0]) == 0 && strcmp(buffers[1], "MACRO") == 0){
			new = createLMacro();
			new->prox = lista;
			lista = new;

			/* Coloca no nó o nome da label da macro.*/
			buffers[0][strlen(buffers[0])-1] = '\0';
			new->label = (char *)malloc((strlen(buffers[0]))*sizeof(char));
			strcpy(new->label, buffers[0]);

			/* Adiciona os argumentos da função no nó*/
			for(i = 0; i < (tokensLidos-3); ++i)
				addArg(buffers[i+2], &new);


			fp = registra_macro(auxFile, &lista); /* Encontrou a label de uma macro e começa o processo de registá-la.*/

		}

		for(i = 0; i < 7; ++i)
			strcpy(buffers[i], "-1");

		auxFile = fp;
	}

	
	for(i = 0; i < 7; ++i){
		free(buffers[i]);
	}
	free(buffers);

	return lista;
}


FILE *registra_macro(FILE *fp, lMacro **lista){
	char **buffers, bufferLinha[300], bufferDef[300];
	int linha, i, tokensLidos;

	buffers = (char **)malloc(7*sizeof(char *));
	if(!buffers){
		printf("Erro ao alocar memória na funcao 'registra_macro' em 'buffers'.\n");
		exit(1);
	}
	for(i = 0; i < 7; ++i)
		buffers[i] = (char *)malloc(60*sizeof(char));

	for(i = 0; i < 7; ++i)
		strcpy(buffers[i], "-1");

	

	while(fgets(bufferLinha, 300, fp) != NULL){
		/* TokensNaLinha é o número de strings efetivamente lidas.*/
		tokensLidos = sscanf(bufferLinha, "%d %s %s %s %s %s %s %s", &linha, buffers[0], buffers[1], buffers[2], 
								buffers[3], buffers[4], buffers[5], buffers[6]);

		if(tokensLidos == 1){
			for(i = 0; i < 7; ++i)
				strcpy(buffers[i], "-1");

			continue;
		}

		to_upcase(buffers, 0, strlen(buffers[0]));
		to_upcase(buffers, 1, strlen(buffers[1]));
		if(strcmp(buffers[0], "ENDMACRO") == 0 || strcmp(buffers[1], "ENDMACRO") == 0)
			break;

		strcpy(bufferDef, buffers[0]);
		for(i = 1; i < (tokensLidos-1); ++i){
			sprintf(bufferDef, "%s %s", bufferDef, buffers[i]);
		}
		sprintf(bufferDef, "%s%c", bufferDef, '\n');

		addDef(bufferDef, lista);




		for(i = 0; i < 7; ++i)
			strcpy(buffers[i], "-1");
	}


	for(i = 0; i < 7; ++i){
		free(buffers[i]);
	}
	free(buffers);
	return fp;
}

int expande_macros(FILE *fp, FILE *final, lMacro **macros){

	liberaLMacro(macros);
	return 0;
}

lMacro *createLMacro(){
	lMacro *new = (lMacro *)malloc(sizeof(lMacro));

	if(!new){
		printf("Erro ao alocar memória na funcao 'createLMacro'. \n");
		exit(1);
	}

	new->prox = NULL;
	new->def = NULL;
	new->args = NULL; 
	new->linha = -1;
	new->numLinhasDef = 0;
	new->numArgs = 0;
	return new;
}

void addMacro(lMacro *n,  lMacro **lista){
	lMacro *new;

	if(n == NULL){
		new = createLMacro();
		(*lista) = new;
	}
	else{
		(*lista) = n;
	}

}

void addArg(char *arg, lMacro **macro){
	char buffer[50];

	strcpy(buffer, arg);

	if(buffer[strlen(buffer)-1] == ',')
		buffer[strlen(buffer)-1] = '\0';

	if((*macro)->numArgs == 0){
		(*macro)->args = (char **)malloc(sizeof(char *));
		
	}
	else{
		(*macro)->args = (char **)realloc((*macro)->args, (((*macro)->numArgs)+1)*sizeof(char *));		
	}

	(*macro)->args[(*macro)->numArgs] = (char *)malloc(strlen(buffer)*sizeof(char));
	strcpy((*macro)->args[(*macro)->numArgs], buffer);

	++((*macro)->numArgs);
}

void addDef(char *def, lMacro **macro){

	char buffer[300];

	strcpy(buffer, def);

	if((*macro)->numLinhasDef == 0){
		(*macro)->def = (char **)malloc(sizeof(char *));
		
	}
	else{
		(*macro)->def = (char **)realloc((*macro)->def, (((*macro)->numLinhasDef)+1)*sizeof(char *));		
	}

	(*macro)->def[(*macro)->numLinhasDef] = (char *)malloc(strlen(buffer)*sizeof(char));
	strcpy((*macro)->def[(*macro)->numLinhasDef], buffer);

	++((*macro)->numLinhasDef);
}


int liberaLMacro(lMacro **lista){
	lMacro *aux1 = (*lista), *aux2;
	int i;

	while(aux1 != NULL){
		aux2 = aux1->prox;

		printf("Dentro libera: %s\n", aux1->label);

		if(aux1->label != NULL)
			free(aux1->label);
		printf("Liberou Label.\n");

		if(aux1->def){
			for(i = 0; i < aux1->numLinhasDef; ++i)
				free(aux1->def[i]);
			free(aux1->def);
		}
		printf("Liberou def.\n");

		if(aux1->args){
			for(i = 0; i < aux1->numArgs; ++i)
				free(aux1->args[i]);
			free(aux1->args);
		}
		printf("Liberou args.\n");		

		free(aux1);
		printf("Liberou no.\n");

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
		if((buffers[word][i] < 'A' || buffers[word][i] > 'Z') && (buffers[word][i] >= 'a' && buffers[word][i] <= 'z'))
			buffers[word][i] -= 32;
	}

}

int main(){
	FILE *fp = fopen("testeMacro.asm", "r");
	lMacro *new, *aux;
	int i;

	new = busca_macros(fp);

	aux = new;

	while(aux != NULL){
		printf("linhasDef: %d\nlabel: %s\n", aux->numLinhasDef, aux->label);
		for(i = 0; i < aux->numLinhasDef; ++i){
			printf("%s", aux->def[i]);
			getchar();
		}
		

		aux = aux->prox;
	}

	liberaLMacro(&new);

	fclose(fp);
	return 0;
}