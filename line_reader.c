#include "line_reader.h"

int read_line(FILE *fp, char *out, int out_size, int *line_no, int *too_long) {
	int c;
	int i = 0;
	int long_line = 0;

	if (fp == NULL || out == NULL || out_size <= 1 || line_no == NULL || too_long == NULL) return LR_FAIL;
	
	*too_long = 0;

	c = fgetc(fp);	/* c get the first char in the line (in file fp) */
	
	if (c == EOF) {
		out[0] = '\0';
		return LR_EOF;
	}

	/* now starting to read new physical line */
	(*line_no)++;

	while (c != EOF && c != '\n' && c != '\r') {
		if (i < (out_size - 1)) out[i++] = (char)c;
		else long_line = 1;	/* buffer full, so mark long_line and keep consuming untill EOF */
		c = fgetc(fp);    /* c get the next char in the line */
	}

	out[i] = '\0';

	if (long_line) *too_long = 1;

	if ((int)LINE_MAX < i) {
		out[LINE_MAX] = '\0';
		*too_long = 1;
	}
	
	return LR_OK;
}
