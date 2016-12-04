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

int main(int argc,  char** argv){

	if(argc < 4 || argc > 6){
		printf("Não foi fornecido o número de argumentos correto na chamada do programa.\n");
		exit(1);
	}

	if(argv[1][1] == 'p'){
		geraArquivoFinal(argv[2], argv[3]);
		/* Chama pre-processamento.*/

	}
	else if(argv[1][1] == 'm'){
		geraArquivoFinal(argv[2], argv[3]);
		printf("%s\n", argv[3]);
		processa_macros(argv[3]);

	}

	else if(argv[1][1] == 'o'){
		geraArquivoFinal(argv[2], argv[3]);
		/* Chama pre-processamento.*/
		processa_macros(argv[3]);
		/* Chama algoritmo de duas passagens.*/
		TwoPassAssembler(argv[3]);
	}
	else
		printf("Formato invalido para a chamada do programa\n");


	return 0;
}