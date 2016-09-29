#include <stdio.h>
#include <stdlib.h>

#define NOT_OPENED -1

#define TAM_ROT 50


/*character/token identifying*/
#define OPERATOR 5
#define CHARACTER 4
#define NUMBER 3
#define BLANK 2
#define COLON 1
#define INVALID 0
#define VALID 1

/*receives an opened file*/
int get_token(File *fp);

int identify_char(char c);