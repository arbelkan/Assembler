#include <stdio.h>
#include <ctype.h>
#include "pass1.h"
#include "line_reader.h"
#include "parser.h"
#include "pass1_directives.h"
#include "symbols.h"
#include "ops.h"
#include "pass1_instructions.h"

static const char *skip_spaces(const char *p);
static int is_blank_or_comment(const char *line);


int pass1_run(AsmState *st, const char *am_filename) {
	FILE *fp;
	char line[LINE_BUF_SIZE];
	int line_no = 0;
	int too_long = 0;
	int rc;		/* rc get the end code (status) from read_line process */
	ParsedLine pl;
	int ok = SUCCESS; /* TODO - document change because of false success */

	if (st == NULL || am_filename == NULL) {
		printf("Error: invalid arguments to pass1_run\n");
		return FAILURE;
	}
	
	fp = fopen(am_filename, "r");
	if (fp == NULL) {
		printf("Error: cannot open file %s\n", am_filename);
		return FAILURE;
	}

	while ((rc = read_line(fp , line, (int)sizeof(line), &line_no, &too_long)) == LR_OK) {
		if (too_long) {
			/* TODO:route this through the shared errors module (Arbel) */	
			printf("Error: line %d is longer than %d characters\n", line_no, LINE_MAX);
			ok = FAILURE; /* TODO - document change because of false success */
		}
		if (is_blank_or_comment(line)) {
			continue;
		}
		if (parse_line(line, &pl) != SUCCESS) {
			printf("Parse error at line %d: %s\n", line_no, line);
			ok = FAILURE; /* TODO - document change because of false success */
			continue;
		}

		if (pl.kind == LINE_DIRECTIVE) {
			if (pass1_handle_directive(st, &pl, line_no) != SUCCESS) {
				/* directive handler already printed an error */
				/* keep going to find more errors */
			}
			else {
				/* Temporary debug: show DC/data count after directive */
				printf("Debug: after line %d directive, DC=%d, data.count=%d\n", line_no, st->DC, st->data.count); 
			}
		}
		else if (pl.kind == LINE_INSTRUCTION) {
			if (pass1_handle_instruction(st, &pl, line_no) != SUCCESS) {
				/* pass1_handle_instruction already printed an error */
				/* keep going to find more errors */
				/* TODO: make sure that empty if block doesnt cause bugs */
			}
		}
		else { /* empty/comment shouldn't reach here due to filtering; ignore */ }			
	}

	if (rc == LR_FAIL) {
		printf("Error: read_line failed while reading %s\n", am_filename);
		fclose(fp);
		return FAILURE;
	}

	fclose(fp);
	return ok; /* TODO - document change because of false success */
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

