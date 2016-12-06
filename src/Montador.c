#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DuasPassagens.h"
#include "macros.h"
#include "prediretivas.h"


/*
	"argc" deve ser igual a 3. Seus argumentos são:

	"argv[0]"" - Diretiva de modo:
		"-p" = Arquivo somente pré-processado.
		"-m" = Arquivo pré-processado e com as macros. 
		"-o" = Arquivo completamente montado.

	"argv[1]" - Nome do arquivo de entrada: sempre acompanhado da extensão ".asm".

	"argv[2]" - Nome do arquivo de saída: somente o nome sem extensão.	
*/

char *remove_asm(char *inFileName){
	char *outFileName = (char *) malloc(strlen(inFileName)*sizeof(char *));

	strncpy(outFileName, inFileName, strlen(inFileName)-4);
	outFileName[strlen(inFileName)-4] = '\0';

	return outFileName;
}

int main(int argc,  char** argv){

	char *outFileName1, *outFileName2, *outFileName3;

	if(argc < 3 || argc > 5){
		printf("Não foi fornecido o número de argumentos correto na chamada do programa.\n");
		exit(1);
	}

	/*if(argv[1][1] == 'p'){
		geraArquivoFinal(argv[2], argv[3]);
		 Chama pre-processamento.

	}
	else if(argv[1][1] == 'm'){
		geraArquivoFinal(argv[2], argv[3]);
		printf("%s\n", argv[3]);
		processa_macros(argv[3]);

	}*/


	if(argv[1][1] == 'o'){
		
		if(argc == 3){

			outFileName1 = remove_asm(argv[2]);
			/* Chama algoritmo de duas passagens.*/
			TwoPassAssembler(outFileName1, NO);
		}
		else if(argc == 4){

			outFileName1 = remove_asm(argv[2]);
			outFileName2 = remove_asm(argv[3]);

			geraArquivoFinal(argv[2], outFileName1);
			/* Chama algoritmo de duas passagens.*/
			TwoPassAssembler(outFileName1, YES);

			geraArquivoFinal(argv[3], outFileName2);
			/*Chama algoritmo de duas passagens*/
			TwoPassAssembler(outFileName2, YES);
		}
		else{
			outFileName1 = remove_asm(argv[2]);
			outFileName2 = remove_asm(argv[3]);
			outFileName3 = remove_asm(argv[4]);

			geraArquivoFinal(argv[2], outFileName1);
			/* Chama algoritmo de duas passagens.*/
			TwoPassAssembler(outFileName1, YES);

			geraArquivoFinal(argv[3], outFileName2);
			/* Chama algoritmo de duas passagens.*/
			TwoPassAssembler(outFileName2, YES);
			
			geraArquivoFinal(argv[4], outFileName3);
			/* Chama algoritmo de duas passagens.*/
			TwoPassAssembler(outFileName3, NO);
		}
	}
	else
		printf("Formato invalido para a chamada do programa\n");


	return 0;
}