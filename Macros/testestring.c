#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	FILE *fp = fopen("testeMacro.asm", "r");
	char buffer1[50];
	int i;

	for(i=0; i<20; ++i){
		fscanf(fp, "%s", buffer1);
		printf("%s\n", buffer1);
	}

	fclose(fp);

	return 0;
}