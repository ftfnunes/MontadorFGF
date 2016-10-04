#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Programa que realiza o pre processamento das diretivas IF E EQU do montador do Assembly inventado*/
/*Victor Araujo Vieira*/

/*Definicao da struct que ira contar cada linha do arquivo de entrada, bem como flags para auxiliar no algoritmo*/
typedef struct{
	char linhaArquivo[501]; /*string que recebe a linha do arquivo*/ /*OBS: a principio recebe uma string com o tamanho de 300, verificar se vai precisar de alocacao dinamica*/
	int possuiIF; /*flag que indica se tem ou nao a diretiva IF na linha */
	int possuiEQU; /*flag que indica se tem ou nao a diretiva EQU na linha*/
	int IFvalido; /*Flag que indica se a condicao do IF foi ou nao validada*/
	char label_EQU[101] /*variavel que contem o nome da label da diretiva EQU*/;
	int valorEQU; /*variavel que contem o valor da label da diretiva EQU*/
	int linhaAtual /*indica o numero da linha, baseado no arquivo fonte*/;
}Linha;

int numero_de_linhas; /*variavel global que guarda o numero de linhas do arquivo*/
Linha *arquivoEntrada; /*Sera um vetor de structs do tipo Linha, aonde cada struct tera uma linha do arquivo*/



/*Funcao que recebe uma string e transforma todos os chars em maiusculos*/
void deixaMaiusculo(char *string){
	int lenghtString = strlen(string);
	int i = 0;

	/*iteracao que percorre toda a string em seu comprimento e, caso o char seja minusculo, o transforma em maiusculo*/
	for(i = 0; i < lenghtString; i++){
		if(string[i] > 96 && string[i] < 123){
			string[i] = string[i] - 32;
		}
	}

}

/*Funcao que recebe a label da diretiva EQU e verifica se apresenta ou nao erro lexico*/
/*Recebe uma string(label) e retorna 1 caso tenha erro e 0 caso nao tenha*/
int apontaErroLexicoLabelEQU(char *string){

	int tamanhoString = strlen(string);

	if(tamanhoString > 50 || (string[0] > 47 && string[0] < 58)) return 1;

	return 0; /*Por default assumir que nao tem erro lexico*/
}

/*Funcao que verifica se tem a diretiva EQU na linha e ja indica, se tem ou nao label*/
/*Se tiver a diretiva com label, retorna 1, se tiver apenas EQU retorna 2(Erro sintatico), se nao tiver, retorna 0*/
int verificaLabelseEQU(char *string){
	char *ptr = NULL, *ptr2 = NULL;
	ptr = strpbrk(string, ":"); /*o ponteiro recebera o local que aparece a primeira ocorrencia do char :*/
	ptr2 = strstr(string, "EQU");/*o ponteiro recebera o local que aprece a primeira ocorrencia da string "EQU"*/

	if(ptr != NULL && ptr2 != NULL) return 1; /*Possui uma label na linha e a diretiva EQU*/
	if(ptr == NULL && ptr2 != NULL) return 2; /*Possui apenas a diretiva EQU na linha, sem label, que eh um erro sintatico*/

	return 0; /*Por default, assumir que nao tem a diretiva EQU e nem uma label*/
}

/*Funcao que pega a label do EQU e o valor que esta sendo atribuido*/


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

	char *ptr = NULL;
	int i = 0, lengthStringAuxiliar = 0;

	ptr = strpbrk(string, ";");

	if(ptr == NULL){
		printf("Nao existe comentario na linha\n");
	}

	lengthStringAuxiliar = strlen(ptr); /*recebe o tamanho da string que possui o comentario*/

	for(i = 0; i < lengthStringAuxiliar; i++){
		*(ptr + i) = '\0'; /*coloca \0 no lugar do comentario*/
	}

	return string;
}

/*Funcao que verifica se tem a diretiva IF na string recebida, que sera a linha do arquivo fonte*/
/*Se tiver, retorna 1, senao, retorna 0*/
int verificarIF(char *string){
	/*Essa funcao ja assume que todos os chars da string ja estao maiusculos*/

	char *ptr = NULL;
	ptr = strstr(string, "IF"); /*a funcao strstr localiza uma substring dentro de uma string*/

	if(ptr != NULL && *(ptr+2) == ' ') return 1; /*possui a diretiva IF com espaco logo apos a diretiva*/
	/*Ou seja, a funcao verifica se tem IF e ainda valida se o IF tem um espaco logo em seguida*/

	return 0; /*Por default, assumir que nao tem a diretiva IF na linha*/
}

/*Funcao que indica o erro e o tipo do erro associado tanto na diretiva IF, quanto na EQU*/
void indicaErros(Linha *linhaCodigo){

}

/*Funcao que verifica se a label do EQU eh ou nao valida e insere o valor dela na variavel correspondente na struct */
/*Recebe a struct(linha) que esta sendo verificada */
void validaEQU(Linha linhaCodigo){


}

/*Funcao que verifica se o valor da label que a diretiva IF avalia eh valido ou nao*/
/*Se for 1 copia a proxima linha, senao, retira essa linha e a proxima*/
/*Recebe a struct(linha) que esta sendo verificada*/
void validaIF(Linha linhaCodigo){


}

/*TODO: fazer escopo das funcoes que tratam a diretiva EQU e as funcoes que irao tratar erros lexicos, semanticos e sintaticos*/


/*Funcao que le o arquivo de entrada e insere as linhas na struct*/
/*Funcao retornara um vetor de structs do tipo Linha e recebe uma string com o nome do arquivo*/
Linha *leArquivo(char *nomeArquivo){

	int linha = 0, contaLinhas = 0; /*indica a linha do arquivo fonte*/
	Linha *ptr = NULL; /*ponteiro auxiliar*/
	char temp[501]; /*recebe a linha temporariamente*/

	FILE *fp = fopen(nomeArquivo, "r"); /*abre o arquivo*/
	if(!fp){
		printf("Nao foi possivel abrir o arquivo fonte!\n");
		exit(1);
	}

	linha = 1; /*linha comeca do 1*/
	contaLinhas = 0; /*conta o numero de linhas do arquivo*/
	while(!feof(fp)){
		ptr = (Linha*)realloc(arquivoEntrada, linha*sizeof(Linha));
		if(ptr != NULL){
			arquivoEntrada = ptr;
		}else{
			free(arquivoEntrada);
			printf("Erro ao alocar/realocar memoria!\n");
			exit(1);
		}

		fgets(temp, 501, fp);
		deixaMaiusculo(temp);
		strcpy(arquivoEntrada[linha-1].linhaArquivo, temp); /*copia a linha do arquivo para a variavel que recebe a linha da struct*/
		arquivoEntrada[linha-1].linhaAtual = linha;
		linha++;
		contaLinhas++;
	}

	numero_de_linhas = contaLinhas; /*variavel global numero de linhas recebe o valor do numero de linhas*/
	fclose(fp); /*fecha o arquivo*/

	return arquivoEntrada;
}

/*Funcao que libera o vetor de structs do tipo Linha que eh usado no programa*/
void liberaStructArquivo(Linha *arquivo){
	free(arquivoEntrada);
}


/*main*/
int main(){

/*	int i = 0;

	arquivoEntrada = leArquivo("arquivoteste.txt");
	for(i = 0; i < numero_de_linhas; i++){
		printf("%d %s", arquivoEntrada[i].linhaArquivo);
	}

	free(arquivoEntrada);*/

	return 0;
}