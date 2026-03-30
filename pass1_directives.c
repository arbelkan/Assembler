#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pass1_directives.h"
#include "data_image.h"

/* internal helpers */
static const char *skip_spaces(const char *p);
static int emit_data_word(AsmState *st, unsigned int value, int line_no);
static int handle_data(AsmState *st, const ParsedLine *pl, int line_no);
static int handle_string(AsmState *st, const ParsedLine *pl, int line_no);
static int handle_extern(AsmState *st, const ParsedLine *pl, int line_no);
static int handle_entry(AsmState *st, const ParsedLine *pl, int line_no);

static int parse_int_list_and_emit(AsmState *st, const char *args, int line_no);
static int parse_quoted_string_and_emit(AsmState *st, const char *args, int line_no);
static int parse_single_symbol(const char *args, char *out, int out_size, int line_no);


int pass1_handle_directive(AsmState *st, const ParsedLine *pl, int line_no) {
	if (st == NULL || pl == NULL) {
		return FAILURE;
	}
	switch (pl->dir_kind) {
		case DIR_DATA: return handle_data(st, pl, line_no);
		case DIR_STRING: return handle_string(st, pl, line_no);
		case DIR_EXTERN: return handle_extern(st, pl, line_no);
		case DIR_ENTRY: return handle_entry(st, pl, line_no);
		default: 
			printf("ERROR (line %d): unknown directive\n", line_no);
			return FAILURE;
	}
}


static int handle_data(AsmState *st, const ParsedLine *pl, int line_no) {
	if (pl->has_label) {
		if (symbols_add_data(&st->symbols, pl->label, st->DC, line_no) != SUCCESS) return FAILURE;
	}
	if (pl->args[0] == '\0') {
		printf("Error (line %d): .data missing arguments\n", line_no);
		return FAILURE;
	}
	return parse_int_list_and_emit(st, pl->args, line_no);
}


static int handle_string(AsmState *st, const ParsedLine *pl, int line_no) {
	if (pl->has_label) {
		if (symbols_add_data(&st->symbols, pl->label, st->DC, line_no) != SUCCESS) return FAILURE;
	}
	if (pl->args[0] == '\0') {
		printf("Error (line %d): .string missing arguments\n", line_no);
		return FAILURE;
	}
	return parse_quoted_string_and_emit(st, pl->args, line_no);
}


static int handle_extern(AsmState *st, const ParsedLine *pl, int line_no) {
	char sym[MAX_LABEL_LEN + 1];
	(void)st;

	if (pl->has_label) {
		/* TODO: check if  the spec forbid label before .extern; keep strictness for later if needed */
        	/* For now we treat it as an error to be safe */
        	printf("Error (line %d): label before .extern is not allowed\n", line_no);
        	return FAILURE;
	}

	if (parse_single_symbol(pl->args, sym, (int)sizeof(sym), line_no) != SUCCESS) {
		return FAILURE;
	}
	
	if(!is_valid_label(sym)) {
		printf("Error (line %d): invalid extern symbol name: %s\n", line_no, sym);
        	return FAILURE;
	}

	if (symbols_add_extern(&st->symbols, sym, line_no) != SUCCESS) return FAILURE;

    	return SUCCESS;
}


static int handle_entry(AsmState *st, const ParsedLine *pl, int line_no) {
	char sym[MAX_LABEL_LEN + 1];
	(void)st;

	if (pl->has_label) {
		printf("Error (line %d): label before .entry is not allowed\n", line_no);
        	return FAILURE;
	}

	if (parse_single_symbol(pl->args, sym, (int)sizeof(sym), line_no) != SUCCESS) {
		return FAILURE;
	}
	
	if (!is_valid_label(sym)) {
        	printf("Error (line %d): invalid entry symbol name: %s\n", line_no, sym);
        	return FAILURE;
	}

	/* TODO:
	- record this entry request for pass2 OR mark it later when symbol exists.
	- could store in a pending list in AsmState.
	*/
	return SUCCESS;
}




/* data emission helpers */


static int emit_data_word(AsmState *st, unsigned int value, int line_no) {
	(void)line_no;

	if (asm_state_emit_data(st, value) != SUCCESS) return FAILURE;

	st->DC++;

	return SUCCESS;
}


static const char *skip_spaces(const char *p) {
	while (p != NULL && *p != '\0' && isspace((unsigned char)*p)) {
		p++;
	}
	return p;
}


static int parse_int_list_and_emit(AsmState *st, const char *args, int line_no) {
	const char *p;
	int sign;
	long val;
	int saw_digit;
	int expecting_number = 1;

	p = args;

	while (*p != '\0') {
		p = skip_spaces(p);

		if (*p == '\0') break;
		
		if(!expecting_number) {
			if (*p != ',') {
				printf("Error (line %d): .data expects ',' between numbers\n", line_no);
				return FAILURE;
			}
			p++; /* find a comma */
			expecting_number = 1;
			continue;
		}
		
		/* parse number */
		sign = 1;
		if (*p == '+' || *p == '-') {
			if (*p == '-') sign = -1;
			p++;
		}
		if (!isdigit((unsigned char)*p)) {
			printf("Error (line %d): .data expects a number\n", line_no);
			return FAILURE;
		}
		val = 0;
		saw_digit = 0;
		while (isdigit((unsigned char)*p)) {
			saw_digit = 1;
			val = (val * 10) + (*p - '0');
			p++;
		}
		if (!saw_digit) {
			printf("Error (line %d): .data expects digits\n", line_no);
			return FAILURE;
		}
		val *= sign;

		/* TODO: Range checks will be added later according the spec (e.g., 12-bit signed) */
		if (emit_data_word(st, (unsigned int)val, line_no) != SUCCESS) {
			return FAILURE;
		}
		
		expecting_number = 0;	
	}
	
	if (expecting_number) {
		printf("Error (line %d): .data expects a number at end\n", line_no);
		return FAILURE;
	}
	return SUCCESS;
}


static int parse_quoted_string_and_emit(AsmState *st, const char *args, int line_no) {
	const char *p;
	int c;

	p = skip_spaces(args);
	if (*p != '"') {
		printf("Error (line %d): .string expects opening quote (\")\n", line_no);
        	return FAILURE;
	}
	p++; /* skip openning quote */

	/* emit characters until closing quote */
	while (*p != '\0' && *p != '"') {
		c = (unsigned char)*p;
		if (emit_data_word(st, (unsigned int)c, line_no) != SUCCESS) {
			return FAILURE;
		}
		p++;
	}
	
	if (*p != '"') {
		printf("Error (line %d): .string missing closing quote (\")\n", line_no);
        	return FAILURE;
	}

	p++; /* skip closing quote */

	/* after closing quote: only spaces allowed */
	p = skip_spaces(p);
	if (*p != '\0') {
		printf("Error (line %d): extra characters after .string\n", line_no);
        	return FAILURE;
	}

	/* null terminator */
	if (emit_data_word(st, 0u, line_no) != SUCCESS) return FAILURE;
	
	return SUCCESS;
}


static int parse_single_symbol(const char *args, char *out, int out_size, int line_no) {
	const char *p;
	int i;

	if (out == NULL || out_size <= 1) return FAILURE;

	out[0] = '\0';

	if (args == NULL) {
		printf("Error (line %d): missing symbol name\n", line_no);
        	return FAILURE;
	}

	p = skip_spaces(args);
	if (*p == '\0') {
		printf("Error (line %d): missing symbol name\n", line_no);
        	return FAILURE;
	}

	i = 0;
	while (*p != '\0' && !isspace((unsigned char)*p)) {
		if (i < (out_size - 1)) { out[i++] = *p; }
		p++;
	}
	out[i] = '\0';

	/* ensure that only spaces remain*/
	p = skip_spaces(p);
	if (*p != '\0') {
		printf("Error (line %d): extra text after symbol name\n", line_no);
        	return FAILURE;
	}

	return SUCCESS;
}


