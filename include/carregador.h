#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MEMORIA 4096

struct chnk{
	int tam, endereco;
};
typedef struct chnk chunk;

struct printing{
	int *endereco, *prog;
};
typedef struct printing Printing;


int simulador(FILE *fp);

Printing *relocador(FILE *fp, chunk *c, int n_chunks);

int define_enderecos(int *enderecos_prog, int tam_prog, int pos_prog, int init_chunk, int fim_chunk);

void reloca(int *prog, int *enderecos_prog, char *mapa_bits, int tam_prog);

void imagem_memoria(FILE *saida, Printing *p, int tam_prog);
