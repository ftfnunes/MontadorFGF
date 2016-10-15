#include "simulador.h"

/*Victor Araujo Vieira - 14/0032801*/
/*Simulador do assembly inventado*/

/*Funcao que le o arquivo de entrada .o, e insere os argumentos do vetor de memoria */
/*Recebe como argumento o nome do arquivo .o*/
void leArquivoObjeto(char *arquivoObjeto){

	int i = 0; 
	FILE *fp = fopen(arquivoObjeto, "r");
	if(fp == NULL){
		printf("Nao foi possivel abrir o codigo objeto!\n");
		exit(1);
	}

	while(!feof(fp)){
		fscanf(fp, "%d", &vetorMemoria[i]);
		i++;
	}
	numero_elementos = i;

	fclose(fp);
}

int verificaseCharInteiro(char c){
	if(c > 47 && c < 58) return 1; /*eh um inteiro*/

	return 0; /*nao eh um inteiro*/
}

int verificaseCharehEntreAeFouNumerico(char c){
	if((c > 64 && c < 71) || (c > 96 && c < 103) || (c > 47 && c < 58)) return 1;

	return 0;
}

/*Funcao que valida se o valor que o usuario digitou eh ou nao valido*/
/*Retorna 1 se for e 0, caso nao*/
int validaUsuarioDigita(char *string){
	int tamanhoString = strlen(string), i = 0, contador = 0;

	/*se o numero for um hexadecimal negativo*/
	if(string[0] == '-' && string[1] == '0' && (string[2] == 'x' || string[2] == 'X')){
		for(i = 3; i < tamanhoString; i++){
			if(verificaseCharehEntreAeFouNumerico(string[i]) == 0){
				contador++;
			}			
		}
		if(contador > 0){
			return 1;
		}else{
			return 0;
		}
	}
	/*se o numero for um hexadecimal positivo*/
	if(string[0] == '0' && (string[1] == 'x' || string[1] == 'X')){
		for(i = 2; i < tamanhoString; i++){
			if(verificaseCharehEntreAeFouNumerico(string[i]) == 0){
				contador++;
			}			
		}
		if(contador > 0){
			return 1;
		}else{
			return 0;
		}
	}
	/*se o numero for um decimal negativo*/
	if(string[0] == '-'){
		for(i = 1; i < tamanhoString; i++){
			if(verificaseCharInteiro(string[i]) == 0){
				contador++;
			}
		}
		if(contador > 0){
			return 1;
		}else{
			return 0;
		}
	}

	/*se o numero for um decimal positivo*/
	for(i = 0; i < tamanhoString; i++){
		if(verificaseCharInteiro(string[i]) == 0){
			contador++;
		}
	}

	if(contador > 0) return 1;

	return 0;

}

/*Funcao que converte de string para inteiro(pode ser tanto decimal quanto hexadecimal)*/
int stringToInt(char *nstr){
	int num = 0;

	if(nstr[0] == '-' && nstr[1] == '0' && (nstr[2] == 'x' || nstr[2] == 'X')){
		sscanf(nstr, "%x", &num);
		return num;
	}

	if(nstr[0] == '0' && (nstr[1] == 'x' || nstr[1] == 'X')){
		sscanf(nstr, "%x", &num);
		return num;
	}
	return atoi(nstr);
}

/*Funcao que recebe a entrada do usuario, e verifica se eh ou nao valida, e a instrucao INPUT recebera o resultado dessa funcao*/
/*Retorna o numero para a funcao INPUT*/
int funcaoInput(){

	char usuarioDigitou[100];
	int valorDigitado = 0;

	printf("INPUT: ");
	scanf("%s", usuarioDigitou);
	getchar();

	while(validaUsuarioDigita(usuarioDigitou) == 1){
		printf("VALOR DIGITADO INVALIDO! DIGITE NOVAMENTE\n");
		printf("INPUT: ");
		scanf("%s", usuarioDigitou);
		getchar();
	}
	
	valorDigitado = stringToInt(usuarioDigitou);

	return valorDigitado;
}

/*Funcao que avalia cada elemento do vetor, verifica qual a instrucao correspondente, e executa o que deveria fazer*/
/*Recebe o vetor de momoria*/
void simulacao(char *nomeArquivo){

	leArquivoObjeto(nomeArquivo);

	/*inicializa o PC e o ACC*/
	PC = vetorMemoria[0];
	ACC = 0; 
	int encerra = 0, posicaoMemoria = 0, posicaoAtual = 0;/*variavel para receber as posicoes da memoria e o valor que esta na memoria*/;

	/*PC sempre recebe a instrucao*/
	/*posicaoAtual recebe a instrucao que esta sendo avaliada*/
	/*posicaoMemoria recebe o valor dos operandos da instrucao, que eh a posicao que o operando esta na memoria*/
	printf("-------SIMULADOR ASSEMBLY INVENTADO------\n");
	printf("\n");
	/*loop da simulacao*/
	do{
		switch(PC){
			case ADD: 
				printf("INSTRUCAO ADD: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("ACC <- %d + %d\n", ACC, vetorMemoria[posicaoMemoria]);
				ACC += vetorMemoria[posicaoMemoria];	
				posicaoAtual += 2; /*PC ira apontar para posicaoAtual + 2, que eh o tamanho da instrucao ADD*/
				PC = vetorMemoria[posicaoAtual]; 
				break;
			case SUB: 
				printf("INSTRUCAO SUB: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("ACC <- %d - %d\n", ACC, vetorMemoria[posicaoMemoria]);
				ACC -= vetorMemoria[posicaoMemoria];	
				posicaoAtual += 2; /*PC ira apontar para posicaoAtual + 2, que eh o tamanho da instrucao ADD*/
				PC = vetorMemoria[posicaoAtual]; 
				break;
			case MULT: 
				printf("INSTRUCAO MULT: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("ACC <- %d * %d\n", ACC, vetorMemoria[posicaoMemoria]);
				ACC *= vetorMemoria[posicaoMemoria];	
				posicaoAtual += 2; /*PC ira apontar para posicaoAtual + 2, que eh o tamanho da instrucao ADD*/
				PC = vetorMemoria[posicaoAtual]; 
				break;
			case DIV: 
				printf("INSTRUCAO DIV: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("ACC <- %d / %d\n", ACC, vetorMemoria[posicaoMemoria]);
				if(vetorMemoria[posicaoMemoria] != 0){
					ACC /= vetorMemoria[posicaoMemoria];	
					posicaoAtual += 2; /*PC ira apontar para posicaoAtual + 2, que eh o tamanho da instrucao ADD*/
					PC = vetorMemoria[posicaoAtual]; 
				}else{
					printf("Erro! Nao pode fazer a divisao por 0!\n");
					encerra = 1;
				}
				
				break;
			case JMP: 
				printf("INSTRUCAO JMP: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("PC <- %d\n", posicaoMemoria);
				posicaoAtual = posicaoMemoria;
				PC = vetorMemoria[posicaoAtual]; 
				break;
			case JMPN: 
				printf("INSTRUCAO JMPN: ");
				if(ACC < 0){
					posicaoMemoria = vetorMemoria[posicaoAtual+1];
					printf("ACC < 0: PC <- %d \n", posicaoMemoria);
					posicaoAtual = posicaoMemoria;
					PC = vetorMemoria[posicaoAtual];
				}else{
					posicaoAtual += 2;
					printf("ACC >= 0: PC <- PROXIMA INSTRUCAO\n");
					PC = vetorMemoria[posicaoAtual];
				}
				break;
			case JMPP: 
				printf("INSTRUCAO JMPP: ");
				if(ACC > 0){
					posicaoMemoria = vetorMemoria[posicaoAtual+1];
					printf("ACC > 0: PC <- %d\n", posicaoMemoria);
					posicaoAtual = posicaoMemoria;
					PC = vetorMemoria[posicaoAtual];
				}else{
					posicaoAtual += 2;
					printf("ACC <= 0: PC <- PROXIMA INSTRUCAO\n");
					PC = vetorMemoria[posicaoAtual];
				}
				break;
			case JMPZ: 
				printf("INSTRUCAO JMPZ: ");
				if(ACC == 0){
					posicaoMemoria = vetorMemoria[posicaoAtual+1];
					printf("ACC = 0: PC <- %d\n", posicaoMemoria);
					posicaoAtual = posicaoMemoria;
					PC = vetorMemoria[posicaoAtual];
				}else{
					posicaoAtual += 2;
					printf("ACC != 0: PC <- PROXIMA INSTRUCAO\n");
					PC = vetorMemoria[posicaoAtual];
				}
				break;
			case COPY: 
				printf("INSTRUCAO COPY: ");
				vetorMemoria[posicaoAtual+2] = vetorMemoria[posicaoAtual+1]; /*OP2 <- OP1*/
				printf("MEM[%d] = %d <- MEM[%d] == %d\n", posicaoAtual+2, vetorMemoria[posicaoAtual+2], posicaoAtual+1, vetorMemoria[posicaoAtual+1]);
				posicaoAtual += 3; /*tamanho da instrucao COPY eh 3*/
				PC = vetorMemoria[posicaoAtual];
				break;
			case LOAD: 
				printf("INSTRUCAO LOAD: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("ACC <- MEM[%d] = %d\n", posicaoMemoria, vetorMemoria[posicaoMemoria]);
				ACC = vetorMemoria[posicaoMemoria]; /*ACC <- MEM[OP]*/
				posicaoAtual += 2; /*tamanho da instrucao LOAD eh 2*/
				PC = vetorMemoria[posicaoAtual];
				break;
			case STORE: 
				printf("INSTRUCAO STORE: ");
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("MEM[%d] <- ACC = %d\n", posicaoMemoria, ACC);
				vetorMemoria[posicaoMemoria] = ACC; /*MEM[OP] <- ACC*/
				posicaoAtual += 2; /*tamanho da instrucao STORE eh 2*/
				PC = vetorMemoria[posicaoAtual];
				break;
			case INPUT:
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				vetorMemoria[posicaoMemoria] = funcaoInput(); /*funcaoInput recebe o argumento do usuario e trata qualquer erro*/
				posicaoAtual += 2; /*tamanho da instrucao INPUT eh 2*/
				PC = vetorMemoria[posicaoAtual];
				break;
			case OUTPUT: 
				posicaoMemoria = vetorMemoria[posicaoAtual+1];
				printf("OUTPUT = %d\n", vetorMemoria[posicaoMemoria]);
				posicaoAtual += 2; /*tamanho da instrucao OUTPUT eh 2*/
				PC = vetorMemoria[posicaoAtual];
				break;
			case STOP: 
				printf("INSTRUCAO STOP ");
				encerra = 1;
				break;
			default: 
				printf("INSTRUCAO INVALIDA!\n");
				encerra = 1;/*se a instrucao for invalida encerra o programa*/
				break;
		}
	}while(encerra == 0);

	printf("\n\n-------FIM DA SIMULACAO!-------\n\n");
	
}