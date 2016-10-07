#include "DuasPassagens.h"

symbol *get_token(FILE *fp){
	char *buffer = (char *)malloc(TAM_ROT*sizeof(char)), c_temp;
	int is_ok;
	symbol *newSymbol = (symbol *)malloc(sizeof(symbol));

	if(!fp)
		return NULL;

	c_temp = fgetc(fp);
	if(c_temp == EOF){
		return NULL;
	}


	switch(identify_char(c_temp)){
		case CHARACTER:
			buffer[0] = c_temp;
			is_ok = read_char_token(buffer, fp);

			if(is_ok == INVALID)
				newSymbol->type = INVALID;
			else if(identify_inst(buffer) != NOT_INST)
				newSymbol->type = INST;
			else if(identify_dir(buffer) != NOT_DIR)
				newSymbol->type = DIR;
			else
				newSymbol->type = SYMBOL;
			break;
		case NUMBER:
			buffer[0] = c_temp;
			is_ok = read_number_token(buffer, fp);
			newSymbol->type = (is_ok == VALID ? NUMBER : INVALID);
			break;
		case BLANK:
			free(buffer);
			free(newSymbol);
			return get_token(fp);
			break;
		case COMMA:
			buffer[0] = c_temp;
			buffer[1] = '\0';
			newSymbol->token = buffer;
			newSymbol->type = COMMA;
			break;
		case COLON:
			buffer[0] = c_temp;
			buffer[1] = '\0';
			newSymbol->token = buffer;
			newSymbol->type = COLON;
			break;
		case BREAK:
			buffer[0] = c_temp;
			buffer[1] = '\0';
			newSymbol->type = BREAK;
			break;
		case OPERATOR:
			buffer[0] = c_temp;
			buffer[1] = '\0';
			newSymbol->type = OPERATOR;
			break;

		default:
			buffer[0] = c_temp;
			buffer[1] = '\0';
			newSymbol->type = INVALID;
			break;
	}

	newSymbol->token = buffer;
	newSymbol->next = NULL;
	return newSymbol;
}

int read_char_token(char *buffer, FILE *fp){
	char c_temp;
	int c_type, i;

	for(i=1; i<50; i++){
		c_type = identify_char(c_temp = fgetc(fp));
		if(c_temp == EOF){
			buffer[i] = '\0';
			break;
		}
		if(c_type == BLANK || c_type == COLON || c_type == OPERATOR || c_type == BREAK || c_type == COMMA){
			buffer[i] = '\0';
			fseek(fp, -1, SEEK_CUR);
			return VALID;
		}
		else if(c_type == INVALID){
			buffer[i] = c_temp;
			buffer[i+1] = '\0';
			return INVALID;
		}
		buffer[i] = c_temp;
	}
	if((c_type = identify_char(fgetc(fp))) != BLANK && c_type != OPERATOR && c_type != COLON && c_type == COMMA && c_type != BREAK && c_temp != EOF){
		fseek(fp, -1, SEEK_CUR);
		buffer[50] = '\0';
		return INVALID;
	}

	return VALID;
}

int read_number_token(char *buffer, FILE* fp){
	char c_temp;
	int c_type, i, is_hexa = 0;

	c_type = identify_char(c_temp = fgetc(fp));
	if(c_temp == EOF){
		buffer[1] = '\0';
		return VALID;
	}
	if(c_temp == 'X' && buffer[0] == '0'){
		is_hexa = 1;
	}
	else if(c_type == CHARACTER){
		buffer[1] = c_temp;
		buffer[2] = '\0';
		return INVALID;
	}
	else if(c_type == BLANK || c_type == COLON || c_type == COMMA || c_type == OPERATOR || c_type == BREAK){
		buffer[1] = '\0';
		fseek(fp, -1, SEEK_CUR);
		return OK;
	}
	else if(c_type == INVALID){
		buffer[1] = c_temp;
		buffer[2] = '\0';
		return INVALID;
	}
	buffer[1] = c_temp;
	for(i=2; i<50; i++){
		c_type = identify_char(c_temp = fgetc(fp));
		if(c_temp == EOF){
			buffer[i] = '\0';
			break;
		}
		if(c_type == BLANK || c_type == COLON || c_type == COMMA || c_type == OPERATOR || c_type == BREAK){
			buffer[i] = '\0';
			fseek(fp, -1, SEEK_CUR);
			break;
		}
		if(is_hexa == 0 && c_type == CHARACTER){
			buffer[i] = c_temp;
			buffer[i+1] = '\0';
			return INVALID;
		}
		else if(is_hexa == 1 && !(c_temp >= 'A' && c_temp <= 'F') && c_type != NUMBER){
			buffer[i] = c_temp;
			buffer[i+1] = '\0';
			return INVALID;
		}
		else if(c_type == INVALID){
			buffer[i] = c_temp;
			buffer[i+1] = '\0';
			return INVALID;
		}
		buffer[i] = c_temp;
	}
	return VALID;
}

int identify_char(char c){
	if((c >= 'A' && c <= 'Z') || c == '_')
		return CHARACTER;
	else if(c >= '0' && c <= '9')
		return NUMBER;
	else if(c == ':')
		return COLON;
	else if(c == ' ' || c == '\t')
		return BLANK;
	else if(c == '\n')
		return BREAK;
	else if(c == '+' || c == '-')
		return OPERATOR;
	else if(c == ',')
		return COMMA;
	else
		return INVALID;
}

table *create_tb(){
	table *tb = (table *)malloc(sizeof(table));
	tb->first = tb->last = NULL;
	if(!tb)
		printf("Erro no montador: falha na alocacao de memoria\n");
	return tb;
}

/*Cria uma lista de tokens presentes numa linha*/
symbol *get_line(FILE *fp){
	symbol *first = NULL, *last = NULL;

	first = last = get_token(fp);
	if(first == NULL)
		return NULL;
	if(first->type == BREAK)
		return first;

	while((last->next = get_token(fp)) != NULL && last->next->type != BREAK){
		last = last->next;
		last->next = NULL;
	}
	if(last->next != NULL)
		last = last->next;

	return first;
}

int insert_symbol(table *tb, char *token, int value, int section){
	t_line *newLine = (t_line *)malloc(sizeof(t_line));

	newLine->token = token;
	newLine->value = value;
	newLine->section = section;
	newLine->is_const = NO;
	newLine->is_const0 = NO;
	newLine->space_size = 1;
	newLine->next = NULL;

	if(tb->first == NULL){
		tb->first = newLine;
		tb->last = newLine;
	}
	else{
		if(find_symbol(tb, token) != NULL){
			free(newLine);
			return ALRD_DEFINED;
		}
		tb->last->next = newLine;
		tb->last = newLine;
	}

	return OK;
}

t_line *find_symbol(table *tb, char *symbol){
	t_line *cur_line;

	for(cur_line = tb->first; cur_line != NULL; cur_line = cur_line->next){
		if(strcmp(cur_line->token, symbol) == 0){
			return cur_line;
		}
	}
	return NULL;
}

int eval_arg(symbol **smbl, table *ts, char *line_number, int inst_code, int is_snd_arg){
	symbol *aux = *smbl;
	t_line *sym;
	long int number = 0, offset;

	sym = find_symbol(ts, aux->token);

	if(sym == NULL){
		printf("Linha %s - Erro Semantico: simbolo %s indefinido\n", line_number, aux->token);
		return ERRO;
	}
	else if(sym->section != TEXT && inst_code >= 5 && inst_code <= 8){
		printf("Linha %s - Erro Semantico: simbolo %s nao faz referencia a secao TEXT\n", line_number, aux->token);
		return ERRO;
	}
	else if(sym->section != DATA && (inst_code < 5 ||inst_code > 8)){
		printf("Linha %s - Erro Semantico: simbolo %s nao faz referencia a secao DATA\n", line_number, aux->token);
		return ERRO;
	}
	else if(inst_code == DIV && sym->is_const0 == YES){
		printf("Linha %s - Erro Semantico: %s eh constante 0\n", line_number, aux->token);
		return ERRO;
	}
	else if(((inst_code == STORE || inst_code == INPUT) && sym->is_const == YES) || (is_snd_arg == YES && sym->is_const == YES)){
		printf("Linha %s - Erro Semantico: Tentativa de alteracao de constante\n", line_number);
		return ERRO;
	}
	number = sym->value;
	aux = aux->next;
	if(aux != NULL && aux->type == OPERATOR){
		aux = aux->next;
		offset = strtoint(aux->token);
		if(offset >= sym->space_size){
			printf("Linha %s - Erro Semantico: Acesso a espaco de memoria nao reservado\n", line_number);
			return ERRO;
		}
		number += offset;
	}
	*(smbl) = aux;
	return number;
}

int strtoint(char *nstr){
	int num = 0, count = 1, i;

	if(nstr[0] == '0' && nstr[1] == 'X'){
		for(i = 2; nstr[i] != '\0'; i++){
			if(identify_char(nstr[i]) == NUMBER)
				num = num*count + nstr[i]-'0';
			else
				num = num*count + nstr[i]-'A'+10;
			count *= 16;
		}
		return num;
	}
	return atoi(nstr);
}

int second_pass(FILE *infp, table *ts, FILE *outfp){
	int error_flag = OK;
	symbol *line, *smbl;
	int num_of_args, inst_code, dir_code;
	char *line_number;
	int arg1 = 0, arg2 = 0, cnst, n_spaces = 0, i;
	int is_negative = 0;

	fseek(infp, 0, SEEK_SET);

	while((line = get_line(infp)) != NULL){
		smbl = line;
		line_number = smbl->token;
		smbl = smbl->next;
		if(smbl == NULL)
			break;
		if(smbl->type == BREAK)
			continue;
		if(identify_dir(smbl->token) == SECTION){
			if(smbl->next->next == NULL)
				break;
			continue;
		}

		if(smbl->next != NULL && smbl->next->type == COLON){
			smbl = smbl->next->next;
			if(smbl->type == BREAK)
				continue;
		}

		if(smbl->type == INST){
			inst_code = identify_inst(smbl->token);
			num_of_args = number_of_addresses(inst_code) - 1;
			smbl = smbl->next;
			/*Recupera argumentos de acordo com a quantidade*/
			if(num_of_args == 1){
				arg1 = eval_arg(&smbl, ts, line_number, inst_code, NO);
				if(arg1 == ERRO){
					error_flag = ERRO;
					continue;
				}
				fprintf(outfp, "%d %d ", inst_code, arg1);
				if(smbl == NULL)
					break;
			}
			else if(num_of_args == 2){
				arg1 = eval_arg(&smbl, ts, line_number, inst_code, NO);
				if(arg1 == ERRO){
					error_flag = ERRO;
					continue;
				}
				smbl = smbl->next;
				arg2 = eval_arg(&smbl, ts, line_number, inst_code, YES);
				if(arg2 == ERRO){
					error_flag = ERRO;
					continue;
				}
				fprintf(outfp, "%d %d %d ", inst_code, arg1, arg2);
				if(smbl == NULL)
					break;
			}
			else{
				fprintf(outfp, "%d ", inst_code);
				if(smbl == NULL)
					break;
			}
		}
		else{
			dir_code = identify_dir(smbl->token);
			if(dir_code == SPACE){
				smbl = smbl->next;
				if(smbl == NULL){
					fprintf(outfp, "0 ");
					break;
				}
				if(smbl->type == BREAK)
					fprintf(outfp, "0 ");
				else{
					n_spaces = strtoint(smbl->token);
					for(i = 0; i<n_spaces; i++)
						fprintf(outfp, "0 ");
					if(smbl->next == NULL)
						break;
				}
			}
			else{
				smbl = smbl->next;
				if(smbl->type == OPERATOR && smbl->token[0] == '-'){
					is_negative = 1;
					smbl = smbl->next;
				}
				else if(smbl->type == OPERATOR)
					smbl = smbl->next;

				cnst = strtoint(smbl->token);
				cnst = is_negative == 1 ? cnst*(-1) : cnst;
				is_negative = 0;

				fprintf(outfp, "%d ", cnst);
				if(smbl->next == NULL)
					break;
			}
		}

	}
	
	return error_flag;
}

table *first_pass(FILE *fp, int *is_ok){
	table *ts = create_tb();
	int end_count = 0;
	symbol *line, *smbl; /*Lista de simbolos*/
	int error_flag = OK;
	int section = NONE;
	int num_of_args, inst_code, dir_code, has_stop = ERRO, has_label = NO, just_label = NO;
	int space_size;
	char *line_number;

	while((line = get_line(fp)) != NULL){
		if(just_label == NO)
			has_stop = NO;
		else
			just_label = NO;

		smbl = line;
		if(smbl->type != NUMBER){
			printf("Erro no formato do arquivo\n");
			exit(1);
		}
		line_number = smbl->token;
		smbl = smbl->next;

		if(smbl == NULL)
			break;

		if(smbl->type == BREAK)
			continue;

		if(verify_line(line) == ERRO){
			error_flag = ERRO;
			continue;
		}
		/*Avalia a diretiva SECTION*/
		if(identify_dir(smbl->token) == SECTION){
			smbl = smbl->next;
			if(smbl == NULL){
				printf("Linha %s - Erro Sintatico: erro de formacao da diretiva SECTION\n", line_number);
				break;
			}
			if(section == NONE && strcmp(smbl->token, "TEXT") == 0){
				section = TEXT;
			}
			else if(section == TEXT && strcmp(smbl->token, "DATA") == 0){
				section = DATA;
			}
			else if(strcmp(smbl->token, "TEXT") == 0 && section != NONE){
				printf("Linha %s - Erro Semantico: dupla definicao de secao TEXT\n", line_number);
				error_flag = ERRO;
				continue;
			}
			else if(strcmp(smbl->token, "DATA") == 0 && section != TEXT){
				printf("Linha %s - Erro Semantico: secao DATA deve vir apos secao TEXT\n", line_number);
				error_flag = ERRO;
				continue;
			}
			else{
				printf("Linha %s - Erro Sintatico: erro de formacao da diretiva SECTION\n", line_number);
				error_flag = ERRO;
				continue;
			}
			smbl= smbl->next;
			if(smbl == NULL)
				break;
			else if(smbl->type != BREAK){
				printf("Linha %s - Erro Sintatico: erro de formacao da diretiva SECTION\n", line_number);
				error_flag = ERRO;
			}
			continue;
		}
		
		/*Verifica se ha rotulo*/
		if(smbl->next != NULL && smbl->next->type == COLON){
			if(smbl->type != SYMBOL){
				printf("Linha %s - Erro Sintatico: %s nao eh rotulo valido\n", line_number, smbl->token);
				error_flag = ERRO;
				continue;
			}
			else{
				if(find_symbol(ts, smbl->token) != NULL){
					printf("Linha %s - Erro Semantico: redefinicao de simbolo %s\n", line_number, smbl->token);
					error_flag = ERRO;
					continue;
				}
				if(has_label == YES){
					printf("Linha %s - Erro Semantico: Dois rotulos com mesmo endereco\n", line_number);
					error_flag = ERRO;
					continue;
				}
				insert_symbol(ts, smbl->token, end_count, section);
				smbl = smbl->next->next;
				if(smbl == NULL){
					printf("Linha %s - Erro Sintatico: linha mal formada\n", line_number);
					error_flag = ERRO;
					break;
				}
				has_label = YES;
				if(smbl->type == BREAK){
					just_label = YES;
					continue;
				}
			}
		}

		/*Espera uma diretiva ou instrucao*/
		if(smbl->type != INST && smbl->type != DIR){
			printf("Linha %s - Erro Sintatico: esperava instrucao ou diretiva, recebeu %s\n", line_number, smbl->token);
			error_flag = ERRO;
			continue;
		}
		/*Se for instrucao*/
		else if(smbl->type == INST){
			inst_code = identify_inst(smbl->token);
			end_count += number_of_addresses(inst_code);
			num_of_args = number_of_addresses(inst_code)-1;
			smbl = smbl->next;

			if(section != TEXT){
				printf("Linha %s - Erro Semantico: instrucao em secao errada\n", line_number);
				error_flag = ERRO;
			}

			if(num_of_args == 1){
				if(smbl == NULL){
					printf("Linha %s - Erro Sintatico: instrucao mal formada\n", line_number);
					error_flag = ERRO;
					break;
				}
				if(is_argument(&smbl) == ERRO){
					printf("Linha %s - Erro Sintatico: argumento mal formado\n", line_number);
					error_flag = ERRO;
					continue;
				}
				if(smbl == NULL)
					break;
				if(smbl->type != BREAK){
					printf("Linha %s - Erro Sintatico: muitos argumentos para instrucao\n", line_number);
					error_flag = ERRO;
					continue;
				}
			}
			else if(num_of_args == 2){
				if(smbl == NULL){
					printf("Linha %s - Erro Sintatico: instrucao mal formada\n", line_number);
					error_flag = ERRO;
					break;
				}
				if(is_argument(&smbl) == ERRO){
					printf("Linha %s - Erro Sintatico: argumento mal formado\n", line_number);
					error_flag = ERRO;
					continue;
				}
				if(smbl == NULL){
					printf("Linha %s - Erro Sintatico: instrucao mal formada\n", line_number);
					error_flag = ERRO;
					break;
				}
				if(smbl->type != COMMA){
					printf("Linha %s - Erro Sintatico: faltando virgula\n", line_number);
					error_flag = ERRO;
					continue;
				}
				smbl = smbl->next;
				if(smbl == NULL){
					printf("Linha %s - Erro Sintatico: instrucao mal formada\n", line_number);
					error_flag = ERRO;
					break;
				}
				if(is_argument(&smbl) == ERRO){
					printf("Linha %s - Erro Sintatico: argumento mal formado\n", line_number);
					error_flag = ERRO;
					continue;
				}
				if(smbl == NULL)
					break;
				if(smbl->type != BREAK){
					printf("Linha %s - Erro Sintatico: muitos argumentos para instrucao\n", line_number);
					error_flag = ERRO;
					continue;
				}
			}
			else{
				has_stop = OK;
				if(smbl == NULL){
					break;
				}
				if(smbl->type != BREAK){
					printf("Linha %s - Erro Sintatico: muitos argumentos para instrucao\n", line_number);
					error_flag = ERRO;
					continue;
				}
			}
		}
		/*Se for diretiva*/
		else{
			dir_code=identify_dir(smbl->token);
			if(section != DATA){
				printf("Linha %s - Erro Semantico: diretiva em secao errada\n", line_number);
				error_flag = ERRO;
			}
			if(has_label == NO){
				printf("Linha %s - Erro Sintatico: diretiva nao possui rotulo\n", line_number);
				error_flag = ERRO;
			}
			if(dir_code == SPACE){
				smbl = smbl->next;
				if(smbl == NULL){
					break;
				}
				if(smbl->type == NUMBER){
					space_size = strtoint(smbl->token);
					if(ts->last != NULL){
						ts->last->space_size = space_size;
					}
					end_count += space_size;
					smbl= smbl->next;
					if(smbl == NULL)
						break;
					if(smbl->type != BREAK){
						printf("Linha %s - Erro Sintatico: muitos argumentos para diretiva\n", line_number);
						error_flag = ERRO;
						continue;
					}
				}
				else if(smbl->type == BREAK){
					end_count++;
				}
			}
			else if(dir_code == CNST){
				if(ts->last != NULL){
					ts->last->is_const = YES; 
				}
				smbl = smbl->next;
				end_count++;
				if(smbl == NULL){
					printf("Linha %s - Erro Sintatico: ma formacao na diretiva CONST\n", line_number);
					error_flag = ERRO;
					break;
				}
				if(smbl->type == OPERATOR){
					smbl=smbl->next;
					if(smbl == NULL){
						printf("Linha %s - Erro Sintatico: ma formacao na diretiva CONST\n", line_number);
						error_flag = ERRO;
						break;
					}
				}
				if(smbl->type != NUMBER){
					printf("Linha %s - Erro Sintatico: argumento invalido para diretiva CONST\n", line_number);
					error_flag = ERRO;
					continue;
				}
				if(ts->last != NULL){
					ts->last->is_const0 = strtoint(smbl->token) == 0 ? YES : NO;
				} 
				smbl=smbl->next;
				if(smbl == NULL)
					break;
				if(smbl->type != BREAK){
					printf("Linha %s - Erro Sintatico: argumentos invalidos para diretiva CONST\n", line_number);
					error_flag = ERRO;
					continue;
				}
			}
		}
		has_label = NO;
	}

	if(section == NONE){
		printf("Linha %s - Erro Semantico: Secao TEXT nao encontrada\n", line_number);
		error_flag = ERRO;
	}

	if(has_stop == ERRO){
		printf("Linha %s - Erro Semantico: programa sem instrucao stop\n", line_number);
	}

	*is_ok = error_flag;

	return ts;
}


int identify_dir(char *token){
	if(strcmp(token, "SECTION") == 0)
		return SECTION;
	if(strcmp(token, "CONST") == 0)
		return CNST;
	if(strcmp(token, "SPACE") == 0)
		return SPACE;
	
	return NOT_DIR;
}

int number_of_addresses(int inst){
	switch(inst){
		case COPY:
			return 3;
		case STOP:
			return 1;
		default:
			return 2;
	}
}

int identify_inst(char *token){
	if(strcmp(token, "ADD") == 0)
		return ADD;
	if(strcmp(token, "SUB") == 0)
		return SUB;
	if(strcmp(token, "MULT") == 0)
		return MULT;
	if(strcmp(token, "DIV") == 0)
		return DIV;
	if(strcmp(token, "JMP") == 0)
		return JMP;
	if(strcmp(token, "JMPN") == 0)
		return JMPN;
	if(strcmp(token, "JMPP") == 0)
		return JMPP;
	if(strcmp(token, "JMPZ") == 0)
		return JMPZ;
	if(strcmp(token, "COPY") == 0)
		return COPY;
	if(strcmp(token, "LOAD") == 0)
		return LOAD;
	if(strcmp(token, "STORE") == 0)
		return STORE;
	if(strcmp(token, "INPUT") == 0)
		return INPUT;
	if(strcmp(token, "OUTPUT") == 0)
		return OUTPUT;
	if(strcmp(token, "STOP") == 0)
		return STOP;

	return NOT_INST;
}

int verify_line(symbol *line){
	symbol *aux;
	char *line_number;

	line_number = line->token;

	for(aux = line->next; aux != NULL; aux = aux->next){
		if(aux->type == INVALID){
			printf("Linha %s - Erro Lexico: token %s mal formado\n", line_number, aux->token);
			return ERRO;
		}
	}

	return OK;
}

int is_argument(symbol **line){
	symbol *aux = *(line);
	if(aux->type == SYMBOL){
		aux = aux->next;
		if(aux != NULL && aux->type == OPERATOR){
			if(aux->token[0] != '+')
				return ERRO;
			if(aux->next != NULL && aux->next->type == NUMBER){
				*line = aux->next->next; /*avanca para o token apos o argumento*/
				return OK;
			}
			else
				return ERRO;
		}
		*line = aux;
		return OK;
	}
	return ERRO;
}

int TwoPassAssembler(char *inFileName){
	FILE *infp, *outfp;
	table *tb;
	int firstPass_error, sndPass_error;
	char outputName[200];
	char inputName[200];


	sprintf(outputName, "%s.o", inFileName);
	sprintf(inputName, "%s.mcr", inFileName);

	outfp = fopen(outputName, "w");
	infp = fopen(inputName, "r");

	if(infp == NULL || outfp == NULL){
		printf("Erro na abertura dos arquivos\n");
		exit(1);
	} 

	tb = first_pass(infp, &firstPass_error);
	sndPass_error = second_pass(infp, tb, outfp);

	if(sndPass_error == ERRO || firstPass_error == ERRO){
		printf("Ocorreram erros na montagem\n");
		exit(1);
	}

	fclose(infp);
	fclose(outfp);

	return OK;
}
