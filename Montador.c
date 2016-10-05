#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Macros/macros.h"

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

	if(argc != 3){
		printf("Não foi fornecido o número de argumentos correto na chamada do programa.\n");
		exit(1);
	}

	if(argv[0][1] == 'p'){
		/* Chama pre-processamento.*/

	}

	if(argv[0][1] == 'm'){
		/* Chama pre-processamento.*/
		processa_macros(argv[2]);

	}

	if(argv[0][1] == 'o'){
		/* Chama pre-processamento.*/
		processa_macros(argv[2]);
		/* Chama algoritmo de duas passagens.*/
	}


	return 0;
}