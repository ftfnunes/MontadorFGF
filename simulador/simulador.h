#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Vetor da memoria do simulador do assembly inventado*/
/*Possui 216 por definicao */
int vetorMemoria[216];
int numero_elementos; /*numero de elementos que estao na memoria*/
int PC; /*Program Counter*/
int ACC; /*Acumulador*/

/*Opcodes das instrucoes*/
#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define JMP 5
#define JMPN 6
#define JMPP 7
#define JMPZ 8
#define COPY 9
#define LOAD 10
#define STORE 11
#define INPUT 12
#define OUTPUT 13
#define STOP 14

/*Assinaturas das funcoes*/
int verificaseCharInteiro(char c);
void leArquivoObjeto(char *arquivoObjeto);
int verificaseCharehEntreAeFouNumerico(char c);
int validaUsuarioDigita(char *string);
int stringToInt(char *nstr);
int funcaoInput();
void simulacao(char *nomeArquivo);