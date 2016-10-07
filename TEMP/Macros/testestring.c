#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void to_upcase(char **buffers, int word, int len){
	int i;

	printf("%s %d %d\n", buffers[word], word, len);
	for(i = 0; i < len; ++i){
		if(buffers[word][i] < 'A' || buffers[word][i] > 'Z')
			buffers[word][i] -= 32;
	}

}

int isLabel(char *str){

	if(!str){
		printf("Parâmetro na funcao 'isLabel' recebido nulo.\n");
		exit(1);
	}

	if(str[strlen(str)-1] == ':')
		return 0;
	else
		return 1;
}

int main(){
	FILE *fp = fopen("testeMacro.asm", "r");
	char buffer1[50], buffer2[50], b3[50], b4[50], b5[50], b6[50], b7[50], buffers[] = "LABEL1:", *b;
	int i, linha, n1, n2, n3;

	printf("%s\n", buffers);
	buffers[strlen(buffers)-1] = '\0';
	b = (char *)malloc((strlen(buffers))*sizeof(char));
	strcpy(b, buffers);
	printf("%s\n", b);
	getchar();


	//for(i=0; i<20; ++i){
	while(fgets(buffer1, 50, fp) != NULL){
		//fscanf(fp, "%s", buffer1) != EOF;
		printf("bf1: %s", buffer1);
		n1 = sscanf(buffer1, "%d", &linha);
		n2 = sscanf(buffer1, "%s", buffer2);
		n3 = sscanf(buffer1, "%s %s %s %s %s %s", buffer2, b3, b4, b5, b6, b7);
		//printf("b2: %s\nb3: %s\nb4: %s\nb5: %s\nb6: %s\nb7: %s\n", buffer2, b3, b4, b5, b6, b7);
		//printf("n1: %d\nn2: %d\nn3: %d\n", n1, n2, n3);
		//printf("%d %d %d %d\n", isLabel(buffer2), isLabel(b3), isLabel(b4), isLabel(b5));
		getchar();
	}



	fclose(fp);

	return 0;
}