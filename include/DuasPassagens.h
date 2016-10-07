#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERRO -1
#define OK 1
#define YES 1
#define NO 0

#define TAM_ROT 50

/*character/token identifying*/
#define DIR 10
#define INST 9
#define COMMA 8
#define BREAK 7
#define SYMBOL 6
#define OPERATOR 5
#define CHARACTER 4
#define NUMBER 3
#define BLANK 2
#define COLON 1
#define INVALID 0
#define VALID 1

/*Table return*/
#define OK 1
#define ALRD_DEFINED -2

/*Section identifier*/
#define NONE 0
#define TEXT 1
#define DATA 2

/*Diretivas*/ 
#define SECTION 1
#define CNST 2
#define SPACE 3
#define NOT_DIR 4

/*Instrucoes*/
#define ADD 1
#define SUB 2
#define MULT 3
#define DIV 4
#define JMP 5
#define JMPN 6
#define JMPP 7
#define JMPZ 8
#define COPY 9
#define LOAD 10
#define STORE 11
#define INPUT 12
#define OUTPUT 13
#define STOP 14
#define NOT_INST 15


typedef struct T_line {
	char *token;
	int value;
	int is_const;
	int is_const0;
	int space_size;
	int section;
	struct T_line *next;
}  t_line;

typedef struct Table{
	t_line *first;
	t_line *last;
}  table;

typedef struct Symbol {
	char *token;
	int type;
	struct Symbol *next;
} symbol;



/*receives an opened file*/
symbol *get_token(FILE *fp);

int read_char_token(char *buffer, FILE *fp);

int read_number_token(char *buffer, FILE* fp);

int identify_char(char c);

table *create_tb();

int insert_symbol(table *tb, char *token, int value, int section);

t_line *find_symbol(table *tb, char *symbol);

symbol *get_line(FILE *fp);

int identify_dir(char *token);

table *section_text(FILE *fp);

int verify_line(symbol *line);

int identify_inst(char *token);

int number_of_addresses(int inst);

int identify_dir(char *token);

int is_argument(symbol **line);

table *first_pass(FILE *fp, int *is_ok);

int strtoint(char *nstr);

int second_pass(FILE *infp, table *ts, FILE *outfp);

int TwoPassAssembler(char *fileName);