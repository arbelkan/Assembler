#ifndef PARSER_H
#define PARSER_H

#include "asm_state.h" /* for the defines: MAX_LABEL_LEN, SUCCESS/FAILURE */
#include "line_reader.h" /* for the define: LINE_MAX */

typedef enum {
	LINE_EMPTY = 0,
	LINE_COMMENT,
	LINE_DIRECTIVE,
	LINE_INSTRUCTION,
	LINE_ERROR
} LineKind;


typedef enum {
	DIR_NONE = 0,
	DIR_DATA,
	DIR_STRING,
	DIR_EXTERN,
	DIR_ENTRY
} DirectiveKind;


typedef struct {
	LineKind kind; 

	int has_label;
	char label[MAX_LABEL_LEN + 1];
	
	DirectiveKind dir_kind;

	char op_name[16]; /* for instructions */

	char args[LINE_MAX + 1]; /* the argoments of the line - raw text after the the directive/op token (trimmed left) */
} ParsedLine;


int parse_line(const char *line, ParsedLine *out); /* parse a single line, returns SUCCESS/FAILURE, if FAILURE than out->kind == LINE_ERROR */

int is_valid_label(const char *s); /* checks if the label name (after :) is valid - first char is letter, rest are letters/digits, length <= MAX_LABEL_LEN. returns 1 if valid */

DirectiveKind directive_from_token(const char *token); /* map a directive token (with leading .) to DirectiveKind. so .data will be DIR_DATA */

#endif
