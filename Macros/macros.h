#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listaMacros{
	char *label, *def;
	int linha;
	struct listaMacros *prox;
} lMacro;


/* Funções relacionadas a arquivos. */
lMacro *busca_macros(FILE *fp);
int expande_macros(FILE *fp, FILE *final);
FILE *registra_macro(FILE *fp, lMacro **lista);
char *macros_expandidas(char *arq_pre_processado);

/* Funções relacionadas a manipulação da lista de macros. */
lMacro *createLMacro();
lMacro *addMacro(char *label, char *def, int linha, lMacro **lista);
int liberaLMacro();

int isNumber(char *str);
int isLabel(char *str);
void to_upcase(char **buffers, int word, int len);