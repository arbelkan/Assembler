#include <stdio.h>
#include <ctype.h>
#include "pass1.h"
#include "line_reader.h"
#include "parser.h"
#include "pass1_directives.h"
#include "symbols.h"
#include "ops.h"
#include "pass1_instructions.h"
#include "errors.h"

static const char *skip_spaces(const char *p);
static int is_blank_or_comment(const char *line);

int pass1_run(AsmState *st, const char *am_filename) {
	FILE *fp;
	char line[LINE_BUF_SIZE];
	int line_no = 0;
	int too_long = 0;
	int rc;		/* rc get the end code (status) from read_line process */
	int ok = SUCCESS;
	ParsedLine pl;

	if (st == NULL || am_filename == NULL) {
		print_error_no_line(INVALID_ARGUMENTS_FOR_PASS1);
		return FAILURE;
	}
	
	fp = fopen(am_filename, "r");
	if (fp == NULL) {
		print_error_no_line(CANNOT_OPEN_FILE);
		return FAILURE;
	}

	while ((rc = read_line(fp , line, (int)sizeof(line), &line_no, &too_long)) == LR_OK) {
		if (too_long) {
			print_error(LINE_LONGER_THAN_80_CHARACTERS, line_no);
			ok = FAILURE;
		}
		if (is_blank_or_comment(line)) {
			continue;
		}
		if (parse_line(line, &pl) != SUCCESS) {
			print_error(PARSE_ERROR, line_no);
			ok = FAILURE;
			continue;
		}

		if (pl.kind == LINE_DIRECTIVE) {
			if (pass1_handle_directive(st, &pl, line_no) != SUCCESS) {
				/* directive handler already printed an error */
				/* keep going to find more errors */
				ok = FAILURE;
			}
		}
		else if (pl.kind == LINE_INSTRUCTION) {
			if (pass1_handle_instruction(st, &pl, line_no) != SUCCESS) {
				/* pass1_handle_instruction already printed an error */
				/* keep going to find more errors */
				ok = FAILURE;
			}
		}
		else { /* empty/comment shouldn't reach here due to filtering; ignore */ }			
	}

	if (rc == LR_FAIL) {
		print_error_no_line(READLINE_FAILED_WHILE_READING);
		fclose(fp);
		return FAILURE;
	}

	fclose(fp);
	return ok;
}

/* helpers */
static const char *skip_spaces(const char *p) {
	while (p != NULL && *p != '\0' && isspace((unsigned char)*p)) {
		p++;
	}
	return p;
}

static int is_blank_or_comment(const char *line) {
	const char *p;
	if (line == NULL) { return FAILURE; }
	p = skip_spaces(line);
	if (*p == '\0') { return FAILURE; }
	if (*p == ';') { return FAILURE; }
	return SUCCESS;
}