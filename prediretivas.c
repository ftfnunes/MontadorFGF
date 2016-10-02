#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Programa que realiza o pre processamento das diretivas IF E EQU do montador do Assembly inventado*/
/*Victor Araujo Vieira*/

/*Definicao da struct que ira contar cada linha do arquivo de entrada, bem como flags para auxiliar no algoritmo*/
typedef struct{
	char linhaArquivo[300]; /*string que recebe a linha do arquivo*/ /*OBS: a principio recebe uma string com o tamanho de 300, verificar se vai precisar de alocacao dinamica*/
	int possuiIF; /*flag que indica se tem ou nao a diretiva IF na linha */
	int possuiEQU; /*flag que indica se tem ou nao a diretiva EQU na linha*/
	char label_EQU[51] /*variavel que contem o nome da label da diretiva EQU*/;
	int valorEQU; /*variavel que contem o valor da label da diretiva EQU*/
	int linhaAtual /*indica o numero da linha, baseado no arquivo fonte*/;
}Linha;

typedef Linha *arquivoEntrada; /*Sera um vetor de structs do tipo Linha, aonde cada struct tera uma linha do arquivo*/

/*Uma maneira de resolver o problema do malloc da linha, eh estabelecer um tamanho fixo e, caso precise aumentar, utilizar o realloc*/


/*Funcao que recebe uma string e retorna a string com todos os chars maiusculos*/
char *deixaMaiusculo(char *string){
	int lenghtString = strlen(string);
	int i = 0;

	/*iteracao que percorre toda a string em seu comprimento e, caso o char seja minusculo, o transforma em maiusculo*/
	for(i = 0; i < lenghtString; i++){
		if(string[i] > 96 && string[i] < 123){
			string[i] = string[i] - 32;
		}
	}

	return string;

}


/*Funcao que verifica se tem comentarios na string recebida, que no caso, sera uma linha do arquivo fonte*/
/*Se tiver, retorna 1, senao, retorna 0*/
int verificaComentarios(char *string){

	char *ptr = NULL;

	ptr = strpbrk(string, ";"); /*a funcao strpbrk localiza um char dentro de uma string*/

	if(ptr != NULL) return 1; /*possui comentario*/

    return 0; /*Por default, assumir que nao tem comentarios na linha*/

}

/*Funcao que retira os comentarios da string recebida, que no caso, sera uma linha do arquivo fonte*/
/*Funcao retornara a string sem os comentarios */
char *retiraComentarios(char *string){

	return ;
}

/*Funcao que verifica se tem a diretiva IF na string recebida, que sera a linha do arquivo fonte*/
/*Se tiver, retorna 1, senao, retorna 0*/
int verificarIF(char *string){
	/*Essa funcao ja assume que todos os chars da string ja estao maiusculos*/

	char *ptr = NULL;

	ptr = strstr(string, "IF"); /*a funcao strstr localiza uma substring dentro de uma string*/

	if(ptr != NULL && (ptr+2) == ' ') return 1; /*possui a diretiva IF com espaco logo apos a diretiva*/
	/*Ou seja, a funcao verifica se tem IF e ainda valida se o IF tem um espaco logo em seguida*/

	return 0; /*Por default, assumir que nao tem a diretiva IF na linha*/
}

/*Funcao que verifica se o valor da label que a diretiva IF avalia eh valido ou nao*/
/*Se for 1 copia a proxima linha, senao, retira essa linha e a proxima*/
/*Recebe a struct que esta sendo verificada e uma string*/
void validaIF(Linha struct, char *string){


}

/*TODO: fazer escopo das funcoes que tratam a diretiva EQU e as funcoes que irao tratar erros lexicos, semanticos e sintaticos*/


/*Funcao que le o arquivo de entrada e insere as linhas na struct*/
/*Funcao retornara um vetor de structs do tipo Linha e recebe um arquivo ja aberto*/
Linha *leArquivo(FILE *fp){



	return arquivoEntrada;
}


/*main*/
int main(){


	return 0;
}