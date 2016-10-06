#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listaMacros{
	char *label, **def, **args;
	int linha, numDef, numArgs;
	struct listaMacros *prox;
} lMacro;


/* Funções relacionadas a arquivos. */
void busca_macros(FILE *fp, FILE *final);
void expande_macros(FILE *final, lMacro **lista, char *label, char **args, int numArgs, int linha);
FILE *registra_macro(FILE *fp, lMacro **lista);
char *macros_expandidas(char *arq_pre_processado);
void addArg(char *arg, lMacro **macro);
void addDef(char *def, lMacro **macro);

/* Funções relacionadas a manipulação da lista de macros. */
lMacro *createLMacro();
void addMacro(lMacro *n, lMacro **lista);
int liberaLMacro();
void temMacroIgual(lMacro *new, lMacro **lista);
int isMacro(char *lb, lMacro **macros);
void copia_macro(lMacro **lista, char *label, char **args, int numArgs);

int isNumber(char *str);
int isLabel(char *str);
void to_upcase(char **buffers, int word, int len);