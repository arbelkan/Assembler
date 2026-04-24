#include <ctype.h>
#include <string.h>

#include "parser.h"

/* internal helpers */
static const char *skip_spaces(const char *p);
static void copy_token(char *dst, int dst_size, const char *start, int len);
static const char *read_token(const char *p, char *token, int token_size);
static void copy_rest_trim_left(char *dst, int dst_size, const char *p);




static void parsed_line_reset(ParsedLine *out) {
	int i;
	
	out->kind = LINE_ERROR;
	out->has_label = 0;
	out->dir_kind = DIR_NONE;

	out->label[0] = '\0';
	out->op_name[0] = '\0';
	out->args[0] = '\0';

	/* Not strictly needed, but keeps things deterministic */
	for (i = 1; i <= MAX_LABEL_LEN; i++) out->label[i] = '\0';
	for (i = 1; i < (int)sizeof(out->op_name); i++) out->op_name[i] = '\0';
	for (i = 1; i <= LINE_MAX; i++) out->args[i] = '\0';
}


int parse_line(const char *line, ParsedLine *out) {
	const char *p;
	const char *after_tok;
	char first_tok[64];
	int tok_len;
	const char *colon;

	if (line == NULL || out == NULL) {
		return FAILURE;
	}

	parsed_line_reset(out);

	p = skip_spaces(line);
	if (*p == '\0') {
		out->kind = LINE_EMPTY;
		return SUCCESS;
	}
	if (*p == ';') {
		out->kind = LINE_COMMENT;
		return SUCCESS;
	}

	/* optional label checks - find the first token and see if is ends with : */
	after_tok = read_token(p, first_tok, (int)sizeof(first_tok));
	if (after_tok == NULL) {
		out->kind = LINE_ERROR;
		return FAILURE;
	}
	tok_len = (int)strlen(first_tok);
	colon = strchr(first_tok, ':');
	if (colon != NULL && colon[1] == '\0') { /* than token ends with : - so token is maybe label */
		first_tok[tok_len -1] = '\0'; /* remove : */
		if (!is_valid_label(first_tok)) {
			out->kind = LINE_ERROR;
			return FAILURE;
		}
		out->has_label = 1;
		copy_token(out->label, (int)sizeof(out->label), first_tok, (int)strlen(first_tok));

		/* continue parsing after label token */
		p = skip_spaces(after_tok);
		if (*p == '\0') {  /* TODO decide if line with only a label is legal/illegal depends on spec; for now mark error) */
			out->kind = LINE_ERROR;
			return FAILURE;
		}

		/* resd the next token as directive/op */
		after_tok = read_token(p, first_tok, (int)sizeof(first_tok));
		if (after_tok == NULL) {
			out->kind = LINE_ERROR;
			return FAILURE;
		}
	}
	
	/* Now first_tok is directive/op token */
	if (first_tok[0] == '.') {
		out->dir_kind = directive_from_token(first_tok);
		if (out->dir_kind == DIR_NONE) {
			out->kind = LINE_ERROR;
			return FAILURE;
		}
		out->kind = LINE_DIRECTIVE;
		
		/* args are the rest of line after the directive token */
		copy_rest_trim_left(out->args, (int)sizeof(out->args), after_tok);
		return SUCCESS;
	}

	/* otherwise: instruction */
	out->kind = LINE_INSTRUCTION;
	copy_token(out->op_name, (int)sizeof(out->op_name), first_tok, (int)strlen(first_tok));
	copy_rest_trim_left(out->args, (int)sizeof(out->args), after_tok);
	return SUCCESS;	
}




/* public */


int is_valid_label(const char *s) {
	int i;
	int len;

	if (s == NULL || s[0] == '\0') { return 0; }
	
	len = (int)strlen(s);
	if (len > MAX_LABEL_LEN) { return 0; }
	
	if (!isalpha((unsigned char)s[0])) { return 0; }  /* first char must be letter */
	for (i = 1; i < len; i++) {
		if (!isalnum((unsigned char)s[i])) { return 0; }
	}

	return 1;
}


DirectiveKind directive_from_token(const char *token) {
	if (token == NULL) return DIR_NONE;

	if (strcmp(token, ".data") == 0) return DIR_DATA;
	if (strcmp(token, ".string") == 0) return DIR_STRING;
	if (strcmp(token, ".extern") == 0) return DIR_EXTERN;
	if (strcmp(token, ".entry") == 0) return DIR_ENTRY;

	return DIR_NONE;
}



/* internal helpers */


static const char *skip_spaces(const char *p) {
	while (p != NULL && *p != '\0' && isspace((unsigned char)*p)) {
		p++;
	}
	return p;
}


/* Copy exactly len chars (or dst_size-1), and null-terminate */
static void copy_token(char *dst, int dst_size, const char *start, int len) {
	int i;
	int n;

	if (dst == NULL || dst_size <= 0) return;
	if (start == NULL || len < 0) {
		dst[0] = '\0';
		return;
	}

	n = len;
	if (n > (dst_size - 1)) { n = dst_size - 1; }

	for (i = 0 ; i < n ; i++) {
		dst[i] = start[i];	
	}
	dst[n] = '\0';
}


/* Read a token starting at p into token[], return pointer after token.
 * Token delimiters: space/tab or ','.
 * Note: ':' is considered part of token (so label token like "LOOP:" stays together).
 */
static const char *read_token(const char *p, char *token, int token_size) {
	const char *start;
	int len;
	
	if (p == NULL || token == NULL || token_size <= 1) return NULL;

	p = skip_spaces(p);
	if (*p == '\0') {
		token[0] = '\0';
		return p;
	}

	start = p;
	while (*p != '\0' && !isspace((unsigned char)*p) && *p != ',') {
		p++;
	}
	
	len = (int)(p - start);
	copy_token(token, token_size, start, len);

	return p;
}


/* Copy rest of line into dst, trimming leading spaces/tabs */
static void copy_rest_trim_left(char *dst, int dst_size, const char *p) {
	int i;

	if (dst == NULL || dst_size <= 0) return;

	if (p == NULL) {
		dst[0] = '\0';
		return;
	}
	
	p = skip_spaces(p);

	i = 0;
	while (*p != '\0' && i < (dst_size - 1)) {
		dst[i++] = *p++;
	}
	dst[i] = '\0';
}


