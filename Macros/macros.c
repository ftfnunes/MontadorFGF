#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

/*
	Para realizar a expansão das macros, é necessário chamar somente a função macros_expandidas, com o seguinte argumento:

	-> arp_pre_processado: nome do arquivo pré-processado sem a extensão.
	
	Esta função retorna um ponteiro para caracteres alocado dinâmicamente com o nome do arquivo gerado após as expansões das macros.
	O arquivo de entrada possui como primeiro elemento da linha o número da linha no arquivo original.
*/
void processa_macros(char *arq_pre_processado){
	FILE *fp, *final;
	char arqFinal[200], arqEntrada[200];

	sprintf(arqEntrada, "%s.pre", arq_pre_processado);
	sprintf(arqFinal, "%s.mcr", arq_pre_processado);

	fp = fopen(arqEntrada, "r");
	if(!fp){
		printf("Erro ao abrir o arquivo 'arq_pre_processado'.\n");
		exit(1); /* Sai se o arquivo pre-processado não for aberto.*/
	}

	final = fopen(arqFinal, "w+");
	if(!final){
		printf("Erro ao abrir o arquivo 'final'.\n");
		exit(1);/* Sai se o arquivo de saída da expansao de macros não for aberto.*/
	}

	busca_macros(fp, final);

	fclose(fp);
	fclose(final);
}

void busca_macros(FILE *fp, FILE *final){
	char bufferLinha[300], **buffers;
	FILE *auxFile;
	lMacro *lista = NULL, *new;
	int linha, i, tokensLidos, flagText = 0, numArgs;


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

		/*printf("%d %s %s %s %s %s %s %s\n", linha, buffers[0], buffers[1], buffers[2], buffers[3], buffers[4], buffers[5], buffers[6]);*/

		if(tokensLidos <= 1){
			for(i = 0; i < 7; ++i)
				strcpy(buffers[i], "\0");
			flagText = 0;
			continue;
		}

		to_upcase(buffers, 0, strlen(buffers[0]));
		to_upcase(buffers, 1, strlen(buffers[1]));

		if(isMacro(buffers[0], &lista) == 0){
			/* Tem que passar a label e os argumentos. */
			++flagText;
			numArgs = tokensLidos - 2;
			for(i = 0; i < numArgs; ++i){
				if(buffers[i+1][strlen(buffers[i+1])-1] == ',')
					buffers[i+1][strlen(buffers[i+1])-1] = '\0';
			}
			expande_macros(final, &lista, buffers[0], &(buffers[1]), numArgs, linha);
		}
		if(isMacro(buffers[1], &lista) == 0){
			/* Tem que passar a label e os argumentos. */
			++flagText;
			numArgs = tokensLidos - 3;
			for(i = 0; i < numArgs; ++i){
				if(buffers[i+2][strlen(buffers[i+2])-1] == ',')
					buffers[i+2][strlen(buffers[i+2])-1] = '\0';
			}
			expande_macros(final, &lista, buffers[1], &(buffers[2]), numArgs, linha);
		}

		if(isLabel(buffers[0]) == 0 && strcmp(buffers[1], "MACRO") == 0){
			++flagText;
			new = createLMacro();
			new->prox = lista;
			lista = new;

			/* Coloca no nó o nome da label da macro.*/
			buffers[0][strlen(buffers[0])-1] = '\0';
			new->label = (char *)malloc((strlen(buffers[0]))*sizeof(char));
			strcpy(new->label, buffers[0]);
			to_upcase(&(new->label), 0, strlen(new->label));

			temMacroIgual(new, &lista, linha);

			/* Adiciona os argumentos da função no nó*/
			for(i = 0; i < (tokensLidos-3); ++i)
				addArg(buffers[i+2], &lista);


			fp = registra_macro(auxFile, &lista); /* Encontrou a label de uma macro e começa o processo de registá-la.*/

		}

		if(!flagText){
			fprintf(final, "%d", linha);
			for(i = 0; i < tokensLidos; ++i){
				fprintf(final, " %s", buffers[i]);
			}
			fprintf(final, "\n");
		}

		for(i = 0; i < 7; ++i)
			strcpy(buffers[i], "\0");

		auxFile = fp;
		flagText = 0;
	}


	for(i = 0; i < 7; ++i){
		free(buffers[i]);
	}
	free(buffers);
	
	liberaLMacro(&lista);
}


FILE *registra_macro(FILE *fp, lMacro **lista){
	char **buffers, bufferLinha[200];
	int linha, i, tokensLidos, numArgs;

	buffers = (char **)malloc(7*sizeof(char *));
	if(!buffers){
		printf("Erro ao alocar memória na funcao 'registra_macro' em 'buffers'.\n");
		exit(1);
	}
	for(i = 0; i < 7; ++i)
		buffers[i] = (char *)malloc(60*sizeof(char));

	for(i = 0; i < 7; ++i)
		strcpy(buffers[i], "\0");


	while(fgets(bufferLinha, 300, fp) != NULL){
		/* TokensNaLinha é o número de strings efetivamente lidas.*/
		tokensLidos = sscanf(bufferLinha, "%d %s %s %s %s %s %s %s", &linha, buffers[0], buffers[1], buffers[2],
								buffers[3], buffers[4], buffers[5], buffers[6]);

		if(tokensLidos == 1){
			for(i = 0; i < 7; ++i)
				strcpy(buffers[i], "\0");

			continue;
		}

		/* 	Expandir caso encontrar uma macro já registrada!
			O atributo "linha" da estrutura armazena a linha de 'ENDMACRO'.

		*/

		if((*lista) != NULL && isMacro(buffers[0], lista) == 0){
			numArgs = tokensLidos - 2;
			for(i=0; i < tokensLidos-1; ++i){
				if(buffers[i][strlen(buffers[i])-1] == ',')
					buffers[i][strlen(buffers[i])-1] = '\0';
			}
			copia_macro(lista, buffers[0], &(buffers[1]), numArgs);
			for(i = 0; i < 7; ++i)
				strcpy(buffers[i], "\0");
			continue;
		}

		if((*lista) != NULL && isMacro(buffers[1], lista) == 0){
			numArgs = tokensLidos - 3;
			for(i=0; i < tokensLidos-1; ++i){
				if(buffers[i][strlen(buffers[i])-1] == ',')
					buffers[i][strlen(buffers[i])-1] = '\0';
			}
			copia_macro(lista, buffers[1], &(buffers[2]), numArgs);
			for(i = 0; i < 7; ++i)
				strcpy(buffers[i], "\0");
			continue;
		}


		to_upcase(buffers, 0, strlen(buffers[0]));
		to_upcase(buffers, 1, strlen(buffers[1]));
		if(strcmp(buffers[0], "ENDMACRO") == 0 || strcmp(buffers[1], "ENDMACRO") == 0){
			break;
		}


		for(i=0; i < tokensLidos-1; ++i){
			if(i > 0 && strcmp(buffers[i-1], "COPY") != 0 && buffers[i][strlen(buffers[i])-1] == ',')
				buffers[i][strlen(buffers[i])-1] = '\0';
			addDef(buffers[i], lista);
		}
		addDef("\n\0", lista);

		for(i = 0; i < 7; ++i)
			strcpy(buffers[i], "\0");
	}


	for(i = 0; i < 7; ++i){
		free(buffers[i]);
	}
	free(buffers);
	return fp;
}

void copia_macro(lMacro **lista, char *label, char **args, int numArgs){
	lMacro *aux, *aux2;
	int i, j;
	char buffer[100];

	for(aux2 = (*lista); aux2; aux2 = aux2->prox){
		if(strcmp(label, aux2->label) == 0)
			aux = aux2;
	}

	for(i = 0; i < aux->numDef; ++i){
		strcpy(buffer, aux->def[i]);

		(*lista)->def = (char **)realloc((*lista)->def, (((*lista)->numDef)+1)*sizeof(char *));

		for(j = 0; j < aux->numArgs; ++j){
			if(strcmp(buffer, aux->args[j]) == 0)
				strcpy(buffer, args[j]);
		}

		(*lista)->def[(*lista)->numDef] = (char *)malloc(strlen(buffer)*sizeof(char));
		strcpy((*lista)->def[(*lista)->numDef], buffer);

		++((*lista)->numDef);
	}

}

void expande_macros(FILE *final, lMacro **lista, char *label, char **args, int numArgs, int linha){
	lMacro *aux;
	char buffer[300];
	int i, j;

	/*if(linha == 7){
		printf("\nLabel linha 7: %s\n", (*lista)->label);
		for(i = 0; i < (*lista)->numDef; ++i)		{
			printf("Def %d: %s\n", i, (*lista)->def[i]);
		}
	}*/

	/* Encontra na lista a macro a ser expandida.*/
	for(aux = (*lista); aux != NULL; aux = aux->prox){
		if(strcmp(aux->label, label) == 0){
            break;
		}

	}

	fprintf(final, "%d", linha);
	for(i = 0; i < aux->numDef; ++i){
		if(strcmp(aux->def[i], "\n") == 0 && i == (aux->numDef -1)){
			fprintf(final, "\n");
			break;
		}

		if(strcmp(aux->def[i], "\n") == 0)
			fprintf(final, "\n%d", linha);
		else{
			strcpy(buffer, aux->def[i]);

			/* Se o que está em buffer é um dos argumentos da macro, coloca em buffer o valor do argumento na instanciação.*/
			for(j = 0; j < aux->numArgs; ++j){
				if(strcmp(buffer, aux->args[j]) == 0)
					strcpy(buffer, args[j]);
			}

			fprintf(final, " %s", buffer);
		}

	}

	return;
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
	new->numDef = 0;
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

	char buffer[100];

	strcpy(buffer, def);

	if((*macro)->def == NULL){
		(*macro)->def = (char **)malloc(sizeof(char *));

	}
	else{
		(*macro)->def = (char **)realloc((*macro)->def, (((*macro)->numDef)+1)*sizeof(char *));
	}

	(*macro)->def[(*macro)->numDef] = (char *)malloc(strlen(buffer)*sizeof(char));
	strcpy((*macro)->def[(*macro)->numDef], buffer);

	++((*macro)->numDef);
}

void temMacroIgual(lMacro *new, lMacro **lista, int linha){
	lMacro *aux = (*lista);
	int ocorrencia = 0;

	if(!new || !aux)
		return;

	while(aux){
		if(strcmp(new->label, aux->label) == 0){
			++ocorrencia;
		}

		if(ocorrencia > 1){
			printf("ERRO SEMANTICO, LINHA %d: A Macro '%s' esta declarada %d vezes. Somente sua ultima declaracao sera considerada.\n", linha, new->label, ocorrencia);
			break;
		}

		aux = aux->prox;
	}
}


int liberaLMacro(lMacro **lista){
	lMacro *aux1 = (*lista), *aux2;
	int i;

	while(aux1 != NULL){
		aux2 = aux1->prox;

		if(aux1->label != NULL)
			free(aux1->label);

		if(aux1->def){
			for(i = 0; i < aux1->numDef; ++i)
				free(aux1->def[i]);
			free(aux1->def);
		}

		if(aux1->args){
			for(i = 0; i < aux1->numArgs; ++i)
				free(aux1->args[i]);
			free(aux1->args);
		}

		free(aux1);

		aux1 = aux2;
	}

	return 0;
}

/* Retorna zero, caso exista uma macro com tal label.*/
int isMacro(char *lb, lMacro **macros){
	lMacro *aux = (*macros);
	int i = 1;

	if(!(*macros))
        return 1;

	if(!lb){
		printf("String invalida na funcao 'isMacro'.\n");
		exit(1);
	}

	while(aux){
		if(strcmp(lb, aux->label) == 0){
			--i;
			break;
		}
		aux = aux->prox;
	}

	return i;
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