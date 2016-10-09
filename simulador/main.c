#include "simulador.h"
/*Main do simulador*/
/*
	"argc" deve ser igual a 1. Seus argumentos são:

	"argv[0]"" - Arquivo objeto gerado pelo programa de traducao:

*/

int main(int argc,  char** argv){

	if(argc !=  2){
		printf("Não foi fornecido o número de argumentos correto na chamada do programa.\n");
		exit(1);
	}else{
		simulacao(argv[1]);
	}

	

	return 0;
}