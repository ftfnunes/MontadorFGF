#include "DuasPassagens.h"

int get_token(File *fp, char *token){
	char *buffer = (char *)malloc(TAM_ROT*sizeof(char)), c_temp;
	int i;

	if(!fp)
		return NOT_OPENED;

	c_temp = fgetc(fp);

	switch(identify_char(c_temp)){
		case CHARACTER:
			buffer[0] = c_temp;
			is_ok = read_char_token(buffer, fp);
			if(is_ok == VALID){
				strcpy(token, buffer);
				return VALID;
			}
			else
				return INVALID;
			break;
		case NUMBER:
			buffer[0] = c_temp;
			is_ok = read_number_token(buffer, fp);
			if(is_ok == VALID){
				strcpy(token, buffer);
				return VALID;
			}
			else
				return INVALID;
			break;
	}
}

int read_char_token(char *buffer, FILE *fp){
	char c_temp;
	int c_type;

	for(i=1; i<50; i++){
		c_type = identify_char(c_temp = fgetc(fp));
		if(c_type == BLANK || c_type == COLON || c_type == OPERATOR){
			buffer[i] = '\0';
			fseek(fp, -1, SEEK_CUR);
			break;
		}
		else if(c_type == INVALID){
			return INVALID;
		}
		buffer[i] = c_temp;
	}
	if((c_type = identify_char(fgetc(fp))) != BLANK && c_type != OPERATOR && c_type != COLON){
		fseek(fp, -1, SEEK_CUR);
		return INVALID;
	}

	return VALID;
}

int read_number_token(char *buffer, FILE* fp){
	char c_temp;
	int c_type;

	for(i=1; i<50; i++){
		c_type = identify_char(c_temp = fgetc(fp));
		if(c_type == BLANK || c_type == COLON || c_type == OPERATOR){
			buffer[i] = '\0';
			fseek(fp, -1, SEEK_CUR);
			break;
		}
		else if(c_type == INVALID || c_type == CHARACTER){
			return INVALID;
		}
		buffer[i] = c_temp;
	}
	return VALID;
}

int identify_char(char c){
	if(c >= 'A' && c <= 'Z')
		return CHARACTER;
	else if(c >= '0' && c <= '9')
		return NUMBER;
	else if(c == ':')
		return COLON;
	else if(c == ' ' || c == '\t' || c == '\n')
		return BLANK;
	else if(c == '+' || c == '-')
		return OPERATOR;
	else
		return INVALID;
}