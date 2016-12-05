#include "ligador.h"

/*Funcao que le o arquivo e preenche os atributos da struct ArquivoObjeto*/
ArquivoObjeto leArquivo(char *nomeArquivo, ArquivoObjeto arquivo){
	char strAux[51], tdSimb[51], tuSimb[51];
	TD *td = NULL;
	TU *tu = NULL;
	int sizeNome = 0, posicaoArquivo = 0, i = 0;
	FILE *fp = fopen(nomeArquivo, "r");
	if(fp == NULL){
		printf("Nao foi possivel abrir o arquivo %s!\n", nomeArquivo);
		exit(1); /*sai do programa caso falhe a abertura do arquivo*/
	}

	/*parte que pega o nome do arquivo*/
	/*a variavel posicaoArquivo armazena o local em que o arquivo esta no buffer*/
	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(strcmp(strAux, "N:") == 0){
			posicaoArquivo = ftell(fp); 
			break; /*quando achar a label N:, para o loop*/
		}
	}

	fseek(fp, posicaoArquivo, SEEK_SET);
	arquivo.nome = NULL;

	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(strcmp(strAux, "S:") == 0){
			posicaoArquivo = ftell(fp);
			break; /*quando achar a label S:, para o loop*/
		}
		sizeNome = strlen(strAux);
		arquivo.nome = (char*)malloc((sizeNome+1)*sizeof(char));
		strcpy(arquivo.nome, strAux); /*copia o nome da strAux para o atributo nome da struct*/
	}

	/*parte que pega o tamanho do arquivo*/

	fseek(fp, posicaoArquivo, SEEK_SET);

	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(strcmp(strAux, "TD:") == 0){
			posicaoArquivo = ftell(fp);
			break; /*quando achar a label TD: para o loop*/
		}
		arquivo.tamanho = atoi(strAux); /*converte a string que contem o tamanho do arquivo para inteiro*/
	}

	/*parte que pega a tabela de definicoes*/

	fseek(fp, posicaoArquivo, SEEK_SET);
	i = 1;
	arquivo.tabelaDef = NULL;
	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(strcmp(strAux, "TU:") == 0){
			posicaoArquivo = ftell(fp);
			break; /*quando achar a label TU: para o loop*/
		}

		fseek(fp, -strlen(strAux), SEEK_CUR); /*para voltar no buffer e pegar o simbolo e o valor desse simbolo corretamente*/

		fscanf(fp, "%s %s", tdSimb, strAux);

		/*parte do codigo que aloca realoca a tabela de definicoes*/
		td = (TD*)realloc(arquivo.tabelaDef, i*sizeof(TD));
		if(td != NULL){
			arquivo.tabelaDef = td;
			strcpy(arquivo.tabelaDef[i-1].simbolo, tdSimb);
			arquivo.tabelaDef[i-1].valor = atoi(strAux);
			i++;
		}else{
			free(arquivo.tabelaDef);
			printf("Erro ao realocar a tabela de definicoes!\n");
			exit(1);
		}
	}

	arquivo.tamTabDef = i - 1;

	/*parte que pega a tabela de uso*/

	fseek(fp, posicaoArquivo, SEEK_SET);
	i = 1;
	arquivo.tabelaUso = NULL;
	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(strcmp(strAux, "MB:") == 0){
			posicaoArquivo = ftell(fp);
			break; /*quando achar a label MB: para o loop*/
		}

		fseek(fp, -strlen(strAux), SEEK_CUR); /*para voltar no buffer e pegar o simbolo e a posicao desse simbolo corretamente*/

		fscanf(fp, "%s %s", tuSimb, strAux);

		/*parte do codigo que aloca realoca a tabela de uso*/
		tu = (TU*)realloc(arquivo.tabelaUso, i*sizeof(TU));
		if(tu != NULL){
			arquivo.tabelaUso = tu;
			strcpy(arquivo.tabelaUso[i-1].simbolo, tuSimb);
			arquivo.tabelaUso[i-1].posicao = atoi(strAux);
			arquivo.tabelaUso[i-1].foiUsada = 1; /*por default, assumir que todas as labels da TU foram usadas na TGD*/
			i++;
		}else{
			free(arquivo.tabelaUso);
			printf("Erro ao realocar a tabela de uso!\n");
			exit(1);
		}
	}

	arquivo.tamTabUso = i - 1;

	/*parte que pega o mapa de bits do arquivo*/

	fseek(fp, posicaoArquivo, SEEK_SET);
	arquivo.mapaBits = NULL;
	arquivo.mapaBits = (char*)malloc(arquivo.tamanho + 1); /*aloca o tamanho necessario para o mapa de bits*/
	if(arquivo.mapaBits == NULL){
		printf("Nao foi possivel alocar a string para o mapa de bits do arquivo!\n");
		exit(1);
	}

	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(strcmp(strAux, "C:") == 0){
			posicaoArquivo = ftell(fp);
			break; /*quando achar a label C:, para o loop*/
		}

		strcpy(arquivo.mapaBits, strAux);
	}

	/*parte que pega o codigo maquina do arquivo*/
	fseek(fp, posicaoArquivo, SEEK_SET);
	arquivo.codMaquina = NULL;
	arquivo.codMaquina = (int*)malloc(arquivo.tamanho*sizeof(int));
	if(arquivo.codMaquina == NULL){
		printf("Nao foi possivel alocar o vetor do codigo de maquina!\n");
		exit(1);
	}

	i = 0;

	while(!feof(fp)){
		fscanf(fp, "%s", strAux);
		if(feof(fp)) break; /*para nao repetir o ultimo numero*/
		arquivo.codMaquina[i] = atoi(strAux);
		i++;
	}

	fclose(fp);

	return arquivo;
}

/*Funcao que cria o arquivo final de ligacao */
ArquivoFinal geraArquivoFinal(ArquivoFinal arqFinal, ArquivoObjeto *arq, int numeroArquivos){
	int i = 0;
	arqFinal.nomeArqFinal = NULL;
	arqFinal.codMaqArqFinal = NULL;
	arqFinal.mBitsArqFinal = NULL;
	if(numeroArquivos == 2){
		arqFinal.tamArqFinal = arq[0].tamanho + arq[1].tamanho; /*tamanho do arquivo final eh a soma do tamanho de todos os arquivos*/

		arqFinal.codMaqArqFinal = (int*)malloc(arqFinal.tamArqFinal*sizeof(int));
		if(arqFinal.codMaqArqFinal == NULL){
			printf("Nao foi possivel alocar o vetor do codigo de maquina do arquivo final!\n");
			exit(1);
		}
			/*parte que atribui o vetor de codigo de maquina do arquivo final*/
		for(i = 0; i < arq[0].tamanho; i++){
			arqFinal.codMaqArqFinal[i] = arq[0].codMaquina[i];	
		}
		for(i = arq[0].tamanho; i < arqFinal.tamArqFinal; i++){
			arqFinal.codMaqArqFinal[i] = arq[1].codMaquina[i - (arq[0].tamanho)];
		}

	}else{

		arqFinal.tamArqFinal = arq[0].tamanho + arq[1].tamanho + arq[2].tamanho;  /*tamanho do arquivo final eh a soma do tamanho de todos os arquivos*/

		arqFinal.codMaqArqFinal = (int*)malloc(arqFinal.tamArqFinal*sizeof(int));
		if(arqFinal.codMaqArqFinal == NULL){
			printf("Nao foi possivel alocar o vetor do codigo de maquina do arquivo final!\n");
			exit(1);
		}

		/*parte que atribui o vetor de codigo de maquina do arquivo final*/
		for(i = 0; i < arq[0].tamanho; i++){
			arqFinal.codMaqArqFinal[i] = arq[0].codMaquina[i];	
		}
		for(i = arq[0].tamanho; i < arq[0].tamanho + arq[1].tamanho; i++){
			arqFinal.codMaqArqFinal[i] = arq[1].codMaquina[i - (arq[0].tamanho)];
		}
		for(i = arq[0].tamanho + arq[1].tamanho; i < arqFinal.tamArqFinal; i++){
			arqFinal.codMaqArqFinal[i] = arq[2].codMaquina[i - (arq[1].tamanho + arq[0].tamanho)];
		}
	}
	
	/*faz as atribuicoes para os atributos do arquivo final*/
	/*nome do arquivo final vai ser o nome do primeiro arquivo que foi passado como argumento na funcao principal*/
	arqFinal.nomeArqFinal = (char*)malloc((strlen(arq[0].nome)+1)*sizeof(char));
	if(arqFinal.nomeArqFinal == NULL){
		printf("Nao foi possivel alocar o nome do arquivo final!\n");
		exit(1);
	}
	strcpy(arqFinal.nomeArqFinal, arq[0].nome);

	/*o mapa de bits final vai ser a concatenacao dos mapas de bits dos outros arquivos*/
	arqFinal.mBitsArqFinal = (char*)malloc((arqFinal.tamArqFinal+1)*sizeof(char));
	if(arqFinal.mBitsArqFinal == NULL){
		printf("Nao foi possivel alocar o mapa de bits do arquivo final!\n");
		exit(1);
	}
	strcpy(arqFinal.mBitsArqFinal, arq[0].mapaBits);
	for(i = 1; i < numeroArquivos; i++){
		strcat(arqFinal.mBitsArqFinal, arq[i].mapaBits);
	}

	return arqFinal;

}

/*Funcao que gera a tabela global de definicoes*/
TGD *geraTGD(TGD *tgd, ArquivoObjeto *arquivo, int numeroArquivos){

	int i = 0, j = 0, counter = 1;
	TGD *tempTGD = NULL;

	/*para cada arquivo, cria uma nova struct da tabela global de definicoes*/
	for(i = 0; i < numeroArquivos; i++){
		if(arquivo[i].tamTabDef > 0){ /*precisa ter algum elemento na TD do arquivo sendo avaliado*/
			for(j = 0; j < arquivo[i].tamTabDef; j++){
				tempTGD = (TGD*)realloc(tgd, counter*sizeof(TGD));
				if(tempTGD != NULL){
					tgd = tempTGD;
					strcpy(tgd[counter-1].simbolo, arquivo[i].tabelaDef[j].simbolo); /*copia o simbolo do arquivo para a TGD*/
					tgd[counter-1].valor = arquivo[i].tabelaDef[j].valor;
					tgd[counter-1].numArquivo = arquivo[i].numeroArquivo;
					counter++;
				}else{
					free(tgd);
					printf("Nao foi possivel realocar a Tabela global de definicoes!\n");
					exit(1);
				}
			}
		}
		
	}

	tamanhoTGD = counter - 1; /*atribui para a variavel tamanhoTGD o tamanho da TGD*/

	return tgd;
}

/*Funcao que corrige os valores da tabela global de definicoes com os fatores de correcao de cada arquivo*/
TGD *aplicaFatoresCorrecaoTGD(TGD *tgd, int tamanhoTGD, ArquivoObjeto *arquivo, int *fatorCorrecao){
	int i = 0;
	/*o fator de correcao do primeiro arquivo sempre eh 0*/
	fatorCorrecao[0] = 0; 
	fatorCorrecao[1] = arquivo[0].tamanho;
	fatorCorrecao[2] = arquivo[0].tamanho + arquivo[1].tamanho;
	for(i = 0; i < tamanhoTGD; i++){
		if(tgd[i].numArquivo == 2){ /*quando for o segundo arquivo, o fator de correcao eh o tamanho do proprio arquivo*/
			tgd[i].valor += arquivo[0].tamanho;	
		}
		if(tgd[i].numArquivo == 3){ /*quando for o terceiro arquivo, o fator de correcao eh o tamanho do proprio arquivo e do segundo*/
			tgd[i].valor += arquivo[0].tamanho + arquivo[1].tamanho;
		}
	}

	return tgd;
}

/*Funcao que percorre a tabela global de definicoes e verifica se o simbolo da tabela de uso existe ou nao na tabela de definicoes*/
int percorreTGD(TGD *tgd, int tamanhoTGD, char *simbolo){
	int i = 0, contador = 0;

	for(i = 0; i < tamanhoTGD; i++){
		if(strcmp(simbolo, tgd[i].simbolo) == 0){	
			contador++;
		}
	}

	if(contador > 0){
		return 1;  /*retorna 1 caso tenha a label da TU na TGD*/
	}

	return 0; /*retorna 0 por default, ou seja, nao possui na tabela de uso*/

}

/*Funcao que verifica se o simbolo da tabela de uso de cada arquivo esta na tabela global de definicoes, se nao estiver, aponta erro*/
void verificaseUsadaTU(ArquivoObjeto *arquivo, TGD *tgd, int numeroArquivos, int tamanhoTGD){
	int i = 0, j = 0;

	for(i = 0; i < numeroArquivos; i++){
		for(j = 0; j < arquivo[i].tamTabUso; j++){
			if(percorreTGD(tgd, tamanhoTGD, arquivo[i].tabelaUso[j].simbolo) == 1){ /*caso o simbolo usado na TU  esteja na TGD, continua o loop*/
				continue;
			}else{ /*senao, imprime mensagem de*/
				printf("ERRO! Simbolo %s na Tabela de Uso do arquivo %s nao esta na Tabela Global de Definicoes!\n", arquivo[i].tabelaUso[j].simbolo, arquivo[i].nome);
				arquivo[i].tabelaUso[j].foiUsada = 0;
			}
		}
	}
}

/*Funcao que procura um simbolo na tabela global de definicoes e retorna o valor desse simbolo*/
int procuraValorTGD(TGD *tgd, int tamanhoTGD, char *simbolo){
	int i = 0, valorTGD = 0;

	for(i = 0; i < tamanhoTGD; i++){
		if(strcmp(simbolo, tgd[i].simbolo) == 0){
			valorTGD =  tgd[i].valor;
		}
	}

	return valorTGD;
}

/*Funcao que corrige o arquivo final de acordo com os valores da tabela global de definicoes*/
ArquivoFinal corrigeArquivoFinal(ArquivoFinal arqFinal, TGD *tgd, ArquivoObjeto *arquivo, int numeroArquivos, int tamanhoTGD, int *fatorCorrecao){
	int i = 0, j = 0, k = 0;
	int aux1 = 0, aux2 = 0;

	for(i = 0; i < numeroArquivos; i++){
		for(j = 0; j < arquivo[i].tamTabUso; j++){
			if(arquivo[i].tabelaUso[j].foiUsada == 1){
				aux1 = arquivo[i].tabelaUso[j].posicao;
				aux2 = fatorCorrecao[i];
				arqFinal.codMaqArqFinal[aux1 + aux2] += procuraValorTGD(tgd, tamanhoTGD, arquivo[i].tabelaUso[j].simbolo);
			}		
		}
	}

	for(i = 1; i < numeroArquivos; i++){
		for(j = fatorCorrecao[i], k = 0; j < arqFinal.tamArqFinal && k < arquivo[i].tamanho; j++, k++){
			if((arqFinal.mBitsArqFinal[j] == '1') && (arquivo[i].codMaquina[k] == arqFinal.codMaqArqFinal[j])){
				arqFinal.codMaqArqFinal[j] += fatorCorrecao[i];
			}
		}
	}

	return arqFinal;
}

/*Funcao que libera todos os atributos da struct ArquivoObjeto que foram alocados dinamicamente*/
void liberaArqEntrada(ArquivoObjeto *arquivo, int numeroArquivos){
	int i = 0;
	for(i = 0; i < numeroArquivos; i++){
		free(arquivo[i].nome);
		free(arquivo[i].tabelaDef);
		free(arquivo[i].tabelaUso);
		free(arquivo[i].mapaBits);
		free(arquivo[i].codMaquina);
	}
	free(arquivo);
}

/*Funcao que libera a tabela global de definicoes*/
void liberaTGD(TGD *tgd){
	free(tgd);
}

/*Funcao que libera todos os atributos da struct ArquivoFinal que foram alocados dinamicamente*/
void liberaAtributosArqSaida(ArquivoFinal arquivo){
	free(arquivo.nomeArqFinal);
	free(arquivo.codMaqArqFinal);
	free(arquivo.mBitsArqFinal);
}

/*Funcao que recebe os nomes dos codigos fontes do montador e realiza o processo de ligacao*/
void linker(char *arquivo1, char *arquivo2, char *arquivo3){

	FILE *fp = NULL;
	ArquivoObjeto *arquivo = NULL;
	ArquivoFinal arqFinal;
	TGD *tgd = NULL;
	int i = 0;

	/*verifica quantos arquivos tem*/
	if(strlen(arquivo2) == 0 && strlen(arquivo3) == 0) numeroArquivos = 1;
	if(strlen(arquivo2) != 0 && strlen(arquivo3) == 0) numeroArquivos = 2;
	if(strlen(arquivo2) != 0 && strlen(arquivo3) != 0) numeroArquivos = 3;

	/*aloca o vetor de arquivos*/
	arquivo = (ArquivoObjeto*)malloc(numeroArquivos*sizeof(ArquivoObjeto));
	if(arquivo == NULL){
		printf("Nao foi possivel alocar o vetor de arquivos!\n");
		exit(1);
	}
	
	/*parte do codigo que vai executar a funcoes e escrever no arquivo final baseado no numero de arquivos*/
	switch(numeroArquivos){
		case 1:
			arquivo[0] = leArquivo(arquivo1, arquivo[0]);
			arquivo[0].numeroArquivo = 1;
			fp = fopen(arquivo[0].nome, "w");
			if(fp == NULL){
				printf("Nao foi possivel abrir o arquivo para escrita!\n");
				exit(1);
			}

			fprintf(fp, "N: %s\n", arquivo[0].nome);
			fprintf(fp, "S: %d\n", arquivo[0].tamanho);
			fprintf(fp, "MB: %s\n", arquivo[0].mapaBits);
			fprintf(fp, "C: ");
			for(i = 0; i < arquivo[0].tamanho; i++){
				fprintf(fp, "%d ", arquivo[0].codMaquina[i]);
			}
			fprintf(fp, "\n");

			break;
		case 2:
			arquivo[0] = leArquivo(arquivo1, arquivo[0]);
			arquivo[0].numeroArquivo = 1;
			arquivo[1] = leArquivo(arquivo2, arquivo[1]);
			arquivo[1].numeroArquivo = 2;

			fp = fopen(arquivo[0].nome, "w");
			if(fp == NULL){
				printf("Nao foi possivel abrir o arquivo para escrita!\n");
				exit(1);
			}
			
			tgd = geraTGD(tgd, arquivo, numeroArquivos);
			tgd = aplicaFatoresCorrecaoTGD(tgd, tamanhoTGD, arquivo, fatorCorrecao);

			arqFinal = geraArquivoFinal(arqFinal, arquivo, numeroArquivos);
			verificaseUsadaTU(arquivo, tgd, numeroArquivos, tamanhoTGD);
			arqFinal = corrigeArquivoFinal(arqFinal, tgd, arquivo, numeroArquivos, tamanhoTGD, fatorCorrecao);

			fprintf(fp, "N: %s\n", arqFinal.nomeArqFinal);
			fprintf(fp, "S: %d\n", arqFinal.tamArqFinal);
			fprintf(fp, "MB: %s\n", arqFinal.mBitsArqFinal);
			fprintf(fp, "C: ");
			for(i = 0; i < arqFinal.tamArqFinal; i++){
				fprintf(fp, "%d ", arqFinal.codMaqArqFinal[i]);
			}
			fprintf(fp, "\n");

			liberaTGD(tgd);
			liberaAtributosArqSaida(arqFinal);

			break;
		case 3:
			arquivo[0] = leArquivo(arquivo1, arquivo[0]);
			arquivo[0].numeroArquivo = 1;
			arquivo[1] = leArquivo(arquivo2, arquivo[1]);
			arquivo[1].numeroArquivo = 2;
			arquivo[2] = leArquivo(arquivo3, arquivo[2]);
			arquivo[2].numeroArquivo = 3;

			fp = fopen(arquivo[0].nome, "w");
			if(fp == NULL){
				printf("Nao foi possivel abrir o arquivo para escrita!\n");
				exit(1);
			}
			
			tgd = geraTGD(tgd, arquivo, numeroArquivos);
			tgd = aplicaFatoresCorrecaoTGD(tgd, tamanhoTGD, arquivo, fatorCorrecao);

			arqFinal = geraArquivoFinal(arqFinal, arquivo, numeroArquivos);
			verificaseUsadaTU(arquivo, tgd, numeroArquivos, tamanhoTGD);
			arqFinal = corrigeArquivoFinal(arqFinal, tgd, arquivo, numeroArquivos, tamanhoTGD, fatorCorrecao);

			fprintf(fp, "N: %s\n", arqFinal.nomeArqFinal);
			fprintf(fp, "S: %d\n", arqFinal.tamArqFinal);
			fprintf(fp, "MB: %s\n", arqFinal.mBitsArqFinal);
			fprintf(fp, "C: ");
			for(i = 0; i < arqFinal.tamArqFinal; i++){
				fprintf(fp, "%d ", arqFinal.codMaqArqFinal[i]);
			}
			fprintf(fp, "\n");

			liberaTGD(tgd);
			liberaAtributosArqSaida(arqFinal);

			break;
		default: printf("Numero de arquivos invalido!\n");
	}

	liberaArqEntrada(arquivo, numeroArquivos);

	fclose(fp);

}