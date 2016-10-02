#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listaMacros{
	char *label, **def, **args;
	int linha, numLinhasDef, numArgs;
	struct listaMacros *prox;
} lMacro;


/* Funções relacionadas a arquivos. */
lMacro *busca_macros(FILE *fp);
int expande_macros(FILE *fp, FILE *final, lMacro **macros);
FILE *registra_macro(FILE *fp, lMacro **lista);
char *macros_expandidas(char *arq_pre_processado);
void addArg(char *arg, lMacro **macro);
void addDef(char *def, lMacro **macro);

/* Funções relacionadas a manipulação da lista de macros. */
lMacro *createLMacro();
void addMacro(lMacro *n, lMacro **lista);
int liberaLMacro();

int isNumber(char *str);
int isLabel(char *str);
void to_upcase(char **buffers, int word, int len);