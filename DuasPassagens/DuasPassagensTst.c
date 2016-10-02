#include <stdio.h>
#include "DuasPassagens.h"

int main(){
	FILE *infp = fopen("teste.asm", "r"), *outfp = fopen("teste.o", "w");
	t_line *sym;
	table *ts;
	char **p;

	if(!infp){
		printf("Nao foi possivel abrir arquivo\n");
		return 0;
	}
	ts = first_pass(infp);

	if(ts != NULL){
		for(sym = ts->first; sym != NULL; sym = sym->next){
			printf("%s : %d\n", sym->token, sym->value);
		}
		second_pass(infp, ts, outfp);
	}

	fclose(infp);

	return 0;
}