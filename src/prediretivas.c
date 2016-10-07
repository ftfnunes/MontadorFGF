#include "prediretivas.h"
/*Programa que realiza o pre processamento das diretivas IF E EQU do montador do Assembly inventado*/
/*Victor Araujo Vieira*/

/*Funcao que verifica se o char eh um inteiro ou nao*/
/*Retorna 1 caso seja e 0 se nao for*/
int verificaseCharInteiro(char c){
	if(c > 47 && c < 58) return 1; /*eh um inteiro*/

	return 0; /*nao eh um inteiro*/
}

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
/*Recebe uma struct do tipo Linha e imprime o erro, caso tenha*/
void apontaErroLexicoLabelEQU(Linha linhaCodigo){

	int tamanhoString = strlen(linhaCodigo.label_EQU);
	if(tamanhoString > 50){
		printf("Linha %d: Erro lexico! Tamanho da label maior que 50!\n", linhaCodigo.linhaAtual);
	}
	if(linhaCodigo.label_EQU[0] > 47 && linhaCodigo.label_EQU[0] < 58){
		printf("Linha %d: Erro lexico! Numero no comeco de uma label\n", linhaCodigo.linhaAtual);
	}
}

/*Funcao que verifica se tem a diretiva EQU na linha e ja indica, se tem ou nao label*/
/*Se tiver a diretiva com label, retorna 1, se tiver apenas EQU retorna 2(Erro sintatico), se nao tiver, retorna 0*/
int verificaLabels_eEQU(char *string){
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
void retiraComentarios(char *string){

	char *ptr = NULL;
	int i = 0, lengthStringAuxiliar = 0;

	ptr = strpbrk(string, ";");

	if(ptr == NULL){
		printf("Nao existe comentario na linha\n");
	}

	lengthStringAuxiliar = strlen(ptr) - 1; /*recebe o tamanho da string que possui o comentario*/

	for(i = 0; i < lengthStringAuxiliar; i++){
		*(ptr + i) = ' '; /*coloca espaco no lugar do comentario*/	
 		
	}

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

/*Funcao que verifica se tem labels do EQU repetidas no codigo*/
void labelsEQU_Repetidas(Linha linhaCodigo){

	int i = 0;

	for(i = 0; i < numero_de_linhas; i++){
		if((strcmp(linhaCodigo.label_EQU, arquivoEntrada[i].label_EQU) == 0 && arquivoEntrada[i].possuiEQU == 1)){
			printf("Linha %d: Erro semantico! Labels repetidas\n", linhaCodigo.linhaAtual);
		}	
	}

}

/*Funcao que verifica se a label do EQU eh ou nao valida e insere o valor dela na variavel correspondente na struct */
/*Recebe a struct(linha) que esta sendo verificada */
Linha validaEQU(Linha linhaCodigo){

	int i = 0, lenght = 0;
	char auxiliar[201], auxiliar2[201], auxiliar3[201]; /*strings auxiliares*/
	char *ptr = NULL;

	ptr = strstr(linhaCodigo.linhaArquivo, "- "); /*verifica se tem negativo*/

	for(i = 0; linhaCodigo.linhaArquivo[i] != ':'; i++){
		auxiliar[i] = linhaCodigo.linhaArquivo[i];
		lenght++;
	}
	auxiliar[lenght] = '\0';
	strcpy(linhaCodigo.label_EQU, auxiliar); /*recebe a label*/
	
	if(ptr == NULL){
		sscanf(linhaCodigo.linhaArquivo, "%*s %*s %s", auxiliar3); /*recebe o valor da EQU em uma string*/
	}else{
		sscanf(linhaCodigo.linhaArquivo, "%*s %*s %s %s", auxiliar3, auxiliar2); /*para caso tenha um negativo sem com espaco. Ex: - 25*/
		strcat(auxiliar3, auxiliar2); /*junta o negativo com o outro caracter*/
	}

	strcpy(linhaCodigo.valorEQU, auxiliar3); /*recebe o valor que esta sendo atribuido ao EQU*/

	/*parte de indicar os erros*/
	labelsEQU_Repetidas(linhaCodigo); /*verifica se tem erros relacionados a repeticao de labels*/
	apontaErroLexicoLabelEQU(linhaCodigo); /*verifica se tem erros relacionados ao nome da label*/

	return linhaCodigo;
}

/*Funcao que verifica se tem ou nao label com a diretiva EQU que esta sendo chamada na diretiva IF*/
/*Se nao tiver, indica erro semantico*/
void indicaErroNaoExisteLabelIF(Linha linhaCodigo){

	int i = 0 ; 
	char auxiliar[201], contador = 0; /*contador ira incrementar caso tenha a label usada no IF no arquivo*/

	sscanf(linhaCodigo.linhaArquivo, "%*s %s", auxiliar);/*auxiliar recebe o nome da label que IF esta verificando*/
	for(i = 0; i < numero_de_linhas; i++){
		if(arquivoEntrada[i].possuiEQU == 1){
			if((strcmp(auxiliar, arquivoEntrada[i].label_EQU) == 0)){
				contador++;
			}
		}
	}

	if(contador == 0) printf("Linha %d: Erro semantico! Nao existe a label usada na diretiva IF\n", linhaCodigo.linhaAtual);
}

/*Funcao que verifica se na string tem apenas espacos ou \n*/
/*Retorna 1 caso tenha apenas espacos ou quebra de linha, e 0 caso contrario*/
int verificaEspacosouNewline(char *string){
	int i = 0, contador = 0, tamanhoString = strlen(string);

	for(i = 0; i < tamanhoString; i++){
		if(string[i] != ' ' && string[i] != '\n'){
			contador++;
		}
	}

	if(contador == 0) return 1; /*So tem espacos ou uma quebra de linha*/

	return 0; /*Por default, assumir que tem outros caracters*/
}

/*Funcao que verifica se a linha que vem depois do IF valido ou invalido eh ou nao apenas espacos e quebra de linha,
ou apenas quebra de linha. Para os dois casos, avalia a proxima linha*/
void validaProximaLinhaIF(Linha linhaCodigo, int valorIFvalido){

	int i = 0;

	/*caso IF seja valido ou invalido, parte do codigo que ira validar a proxima linha do arquivo*/
	if(linhaCodigo.IFvalido == valorIFvalido){
		for(i = 0; i < numero_de_linhas; i++){
			if(arquivoEntrada[i].linhaAtual > linhaCodigo.linhaAtual){
				/*verifica se a linha que vem depois do IF valido ou invalido, eh ou nao espaco em branco e quebra de linha, ou apenas quebra de linha*/
				if(verificaEspacosouNewline(arquivoEntrada[i].linhaArquivo) == 0){ 
					arquivoEntrada[i].IFvalido = valorIFvalido; /*valida tambem a linha depois do IF valido*/
					break; /*para o loop caso ja valide a proxima linha depois do IF, para nao verificar as proximas linhas*/
				}
			}
		}
	}
}

/*Funcao que verifica se o valor da label que a diretiva IF avalia eh valido ou nao*/
/*Se for 1 copia a proxima linha, senao, retira essa linha e a proxima*/
/*Recebe a struct(linha) que esta sendo verificada*/
Linha validaIF(Linha linhaCodigo){
	/*caso tenha uma label que nao existe, apontar erro semantico!*/
	int i = 0;
	char auxiliar[201];

	sscanf(linhaCodigo.linhaArquivo, "%*s %s", auxiliar);/*auxiliar recebe o nome da label que IF esta verificando*/
	for(i = 0; i < numero_de_linhas; i++){
		if(arquivoEntrada[i].possuiEQU == 1){
			if((strcmp(auxiliar, arquivoEntrada[i].label_EQU) == 0)){
				if(strcmp(arquivoEntrada[i].valorEQU, "1") == 0){
					linhaCodigo.IFvalido = 1; /*Label do EQU vale 1, logo, eh valida */
				}
				if(strcmp(arquivoEntrada[i].valorEQU, "1") != 0){
					linhaCodigo.IFvalido = 0; /*Label do EQU vale 0 ou outro valor, logo, eh invalida*/
				}
			}
		}
	}

	/*caso IF seja valido, parte do codigo que validara a proxima linha do arquivo*/
	validaProximaLinhaIF(linhaCodigo, 1);

	/*caso IF seja invalido, parte do codigo que invalidara a proxima linha do arquivo*/
	validaProximaLinhaIF(linhaCodigo, 0);

	linhaCodigo.IFvalido = 0; /*zera a flag de IFvalido da linha que esta sendo lida, considerando que eh um IF, e assumindo que a linha seguinte
							   tenha sido validada, caso o IF  seja valido */
	indicaErroNaoExisteLabelIF(linhaCodigo); /*indica se tem ou nao, erro semantico, em relacao a labels nao declaradas na diretiva IF*/

	return linhaCodigo;

}

/*Funcao que le o arquivo de entrada(formato .asm) e insere as linhas na struct*/
/*Funcao retornara um vetor de structs do tipo Linha e recebe uma string com o nome do arquivo*/
Linha *leArquivo(char *nomeArquivo){

	int linha = 0, contaLinhas = 0; /*indica a linha do arquivo fonte*/
	Linha *ptr = NULL; /*ponteiro auxiliare*/
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

		fgets(temp, 1001, fp);
		deixaMaiusculo(temp);
		strcpy(arquivoEntrada[linha-1].linhaArquivo, temp); /*copia a linha do arquivo para a variavel que recebe a linha da struct*/
		arquivoEntrada[linha-1].linhaAtual = linha; /*indica a inha do arquivo original*/
		if(verificaComentarios(arquivoEntrada[linha-1].linhaArquivo) == 1){ /*verifica se tem comentarios na linha, se tiver, ja retira*/
			retiraComentarios(arquivoEntrada[linha-1].linhaArquivo);
		}

		if(verificarIF(arquivoEntrada[linha-1].linhaArquivo) == 1){
			arquivoEntrada[linha-1].possuiIF = 1; /*ja confirma se tem IF na linha*/
		} 
		if(verificaLabels_eEQU(arquivoEntrada[linha-1].linhaArquivo) == 1){
			arquivoEntrada[linha-1].possuiEQU = 1; /*ja confirma se tem EQU com uma label na linha*/
		} 
		if(verificaLabels_eEQU(arquivoEntrada[linha-1].linhaArquivo) == 2){
			printf("Linha %d: Erro sintatico! Diretiva EQU sem uma label\n", linha);
		} 
		arquivoEntrada[linha-1].IFvalido = 2; /*todas as linhas, a principio, recebem na flag IFvalido o valor 2*/
		if(arquivoEntrada[linha-1].possuiEQU) arquivoEntrada[linha-1].IFvalido = 0; /*se possuir a diretiva EQU na linha, nao a reescreve no arquivo final*/
		linha++;
		contaLinhas++;
	}

	/*caso a ultima linha seja uma quebra de linha ou apenas espacos*/
	if(strcmp(arquivoEntrada[contaLinhas-1].linhaArquivo, arquivoEntrada[contaLinhas-2].linhaArquivo) == 0){ 
			arquivoEntrada[contaLinhas-1].IFvalido = 0;
	}else{
		arquivoEntrada[contaLinhas-1].IFvalido = 2;
	}
	numero_de_linhas = contaLinhas; /*variavel global numero de linhas recebe o valor do numero de linhas*/
	fclose(fp); /*fecha o arquivo*/

	return arquivoEntrada;
}

/*Funcao que libera o vetor de structs do tipo Linha que eh usado no programa*/
void liberaStructArquivo(Linha *arquivo){
	free(arquivoEntrada);
}

/*Funcao que vai gerar o arquivo final, e retorna o nome do arquivo final, com a extensao .pre*/
/*Recebe o nome do arquivo .asm(de entrada) e o nome do arquivo final, que tera a extensao .pre*/
char *geraArquivoFinal(char *nomeArquivoOriginal, char *nomeArquivoFinal){

	char *arquivoFinal;
	int lenghtStringArquivoFinal = strlen(nomeArquivoFinal) + 5, i = 0;
	/*aloca a string auxiliar para receber o nome do arquivo final + 5, ou seja, para receber o formato .pre + \0*/
	arquivoFinal = (char*)malloc(lenghtStringArquivoFinal*sizeof(char));

	sprintf(arquivoFinal, "%s%s", nomeArquivoFinal,".pre");
	arquivoFinal[lenghtStringArquivoFinal] = '\0';
	FILE *fp = fopen(arquivoFinal, "w");
	if(fp == NULL){
		printf("Nao foi possivel criar o arquivo final .pre!\n");
		exit(1);
	}

	/*Funcao que vai ler o arquivo original e criar a struct com as linhas do arquivo*/
	arquivoEntrada = leArquivo(nomeArquivoOriginal);

	/*Parte que faz as devidas validacoes para as diretivas IF e EQU*/
	for(i = 0; i < numero_de_linhas; i++){
		if(arquivoEntrada[i].possuiEQU == 1){
			arquivoEntrada[i] = validaEQU(arquivoEntrada[i]);
		}
		if(arquivoEntrada[i].possuiIF == 1){	
		 arquivoEntrada[i] = validaIF(arquivoEntrada[i]);
		}
	}

	/*parte que vai escrever no arquivo final*/
	for(i = 0; i < numero_de_linhas; i++){
		/*apenas escreve no arquivo se a flag IFvalido for 1 ou 2*/
		if(arquivoEntrada[i].IFvalido == 1 || arquivoEntrada[i].IFvalido == 2){
			/*insere no arquivo final a linha correspondente ao arquivo original e a string equivalente a linha do arquivoteste*/
			fprintf(fp, "%d %s", arquivoEntrada[i].linhaAtual, arquivoEntrada[i].linhaArquivo);
		}
	}

	liberaStructArquivo(arquivoEntrada);
	fclose(fp);

	return arquivoFinal;
}