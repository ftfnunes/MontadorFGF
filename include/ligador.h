#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*LIGADOR*/
/*Victor Araujo Vieira - 14/0032801*/

/*.h do ligador*/

/*Struct para a tabela de uso*/
typedef struct TU{
	int foiUsada; /*flag para indicar se essa label tem ou nao na tabela global de definicoes. 0 caso nao e 1 caso sim*/
	char simbolo[51];
	int posicao;
}TU;

/*Struct para a tabela de definicoes*/
typedef struct TD{
	char simbolo[51];
	int valor;
}TD;

/*Struct para a tabela global de definicoes*/
typedef struct TGD{
	int numArquivo;
	char simbolo[51];
	int valor;
}TGD;

/*Struct que representa o arquivo objeto*/
typedef struct ArquivoObjeto{
	int numeroArquivo; /*atributo que corresponde ao numero do arquivo*/
	TU *tabelaUso; /*a tabela de uso associada ao arquivo*/
	int tamTabUso; /*tamanho da tabela de uso*/
	TD *tabelaDef; /*a tabela de definicoes associada ao arquivo*/
	int tamTabDef; /*tamanho da tabela de definicoes*/
	char *nome; /*o nome do arquivo fonte*/
	int *codMaquina; /*Representa a parte do codigo em si*/
	int tamanho; /*tamanho do arquivo*/
	char *mapaBits; /*contem o mapa de bits do arquivo*/
}ArquivoObjeto;

typedef struct ArquivoFinal{
	char *nomeArqFinal;
	int *codMaqArqFinal;
	int tamArqFinal;
	char *mBitsArqFinal;
}ArquivoFinal;

int numeroArquivos; /*o numero de arquivos que foi passado para o programa*/
int tamanhoTGD; /*variavel global que contem o tamanho da tabela global de definicoes*/
int fatorCorrecao[3]; /*variavel global que contem os fatores dos arquivos, respectivamente*/


/*Assinaturas das funcoes*/
ArquivoObjeto leArquivo(char *nomeArquivo, ArquivoObjeto arquivo);
ArquivoFinal geraArquivoFinal(ArquivoFinal arqFinal, ArquivoObjeto *arq, int numeroArquivos);
TGD *geraTGD(TGD *tgd, ArquivoObjeto *arquivo, int numeroArquivos);
TGD *aplicaFatoresCorrecaoTGD(TGD *tgd, int tamanhoTGD, ArquivoObjeto *arquivo, int *fatorCorrecao);
int percorreTGD(TGD *tgd, int tamanhoTGD, char *simbolo);
void verificaseUsadaTU(ArquivoObjeto *arquivo, TGD *tgd, int numeroArquivos, int tamanhoTGD);
int procuraValorTGD(TGD *tgd, int tamanhoTGD, char *simbolo);
ArquivoFinal corrigeArquivoFinal(ArquivoFinal arqFinal, TGD *tgd, ArquivoObjeto *arquivo, int numeroArquivos, int tamanhoTGD, int *fatorCorrecao);
void liberaArqEntrada(ArquivoObjeto *arquivo, int numeroArquivos);
void liberaTGD(TGD *tgd);
void liberaAtributosArqSaida(ArquivoFinal arquivo);
void linker(char *arquivo1, char *arquivo2, char *arquivo3);
