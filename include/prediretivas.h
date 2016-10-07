#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Definicao da struct que ira contar cada linha do arquivo de entrada, bem como flags para auxiliar no algoritmo*/
typedef struct{
	char linhaArquivo[1001]; /*string que recebe a linha do arquivo*//*OBS: a principio recebe uma string com o tamanho de 300, verificar se vai precisar de alocacao dinamica*/
	int possuiIF; /*flag que indica se tem ou nao a diretiva IF na linha */
	int possuiEQU; /*flag que indica se tem ou nao a diretiva EQU na linha*/
	int IFvalido; /*Flag que indica se a condicao do IF foi ou nao validada. 0 caso nao seja, 1 caso seja, e 2 para o resto das linhas*/
	char label_EQU[201] /*variavel que contem o nome da label da diretiva EQU*/;
	char valorEQU[201]; /*variavel que contem o valor da label da diretiva EQU*/
	int linhaAtual /*indica o numero da linha, baseado no arquivo fonte*/;
}Linha;

int numero_de_linhas; /*variavel global que guarda o numero de linhas do arquivo*/
Linha *arquivoEntrada; /*Sera um vetor de structs do tipo Linha, aonde cada struct tera uma linha do arquivo*/

/*Assinaturas das funcoes*/
int verificaseCharInteiro(char c);
void deixaMaiusculo(char *string);
void apontaErroLexicoLabelEQU(Linha linhaCodigo);
int verificaLabels_eEQU(char *string);
int verificaComentarios(char *string);
void retiraComentarios(char *string);
int verificarIF(char *string);
void labelsEQU_Repetidas(Linha linhaCodigo);
Linha validaEQU(Linha linhaCodigo);
void indicaErroNaoExisteLabelIF(Linha linhaCodigo);
int verificaEspacosouNewline(char *string);
void validaProximaLinhaIF(Linha linhaCodigo, int valorIFvalido);
Linha validaIF(Linha linhaCodigo);
Linha *leArquivo(char *nomeArquivo);
void liberaStructArquivo(Linha *arquivo);
char *geraArquivoFinal(char *nomeArquivoOriginal, char *nomeArquivoFinal);
