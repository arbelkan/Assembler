#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "operand.h"

static const char *skip_spaces_local(const char *p);
static int parse_register(const char *s, int *reg_out);
static int parse_signed_long(const char *s, long *out_val);


int operand_parse(const char *token, Operand *out, int line_no) {
	const char *p;
	int r;
	long v;

	if (token == NULL || out == NULL) return FAILURE;

	out->mode = ADDR_DIRECT;
	out->reg_num = -1;
	out->imm_value = 0;
	out->label[0] = '\0';

	p = skip_spaces_local(token);

	if (*p == '\0') {
		printf("Error (line %d): missing operand\n", line_no);
		return FAILURE;
	}

	/* immediate: #number */
	if (*p == '#') {
		p++;
		p = skip_spaces_local(p);
		if (parse_signed_long(p, &v) != SUCCESS) {
			printf("Error (line %d): invalid immediate number\n", line_no);
			return FAILURE;
		}
		out->mode = ADDR_IMMEDIATE;
		out->imm_value = v;
		return SUCCESS;
	}

	/* relative: %label */
	if (*p == '%') {
		p++;
		p = skip_spaces_local(p);
		if (!is_valid_label(p)) {
			printf("Error (line %d): invalid label in relative operand\n", line_no);
			return FAILURE;
		}
		out->mode = ADDR_RELATIVE;
		strncpy(out->label, p, OPERAND_LABEL_MAX);
		out->label[OPERAND_LABEL_MAX] = '\0';
		return SUCCESS;
	}

	/* register: r0,...,r7 */
	if (parse_register(p, &r) == SUCCESS) {
		out->mode = ADDR_REGISTER;
		out->reg_num = r;
		return SUCCESS;
	}

	/* direct: label */
	if (!is_valid_label(p)) {
		printf("Error (line %d): invalid label in direct operand\n", line_no);
		return FAILURE;
	}
	out->mode = ADDR_DIRECT;
	strncpy(out->label, p, OPERAND_LABEL_MAX);
	out->label[OPERAND_LABEL_MAX] = '\0';
	return SUCCESS;
}



static const char *skip_spaces_local(const char *p) {
	while (p != NULL && *p != '\0' && isspace((unsigned char)*p)) {
		p++;
	}
	return p;
}


static int parse_register(const char *s, int *reg_out) {
	const char *p;

	if (s == NULL || reg_out == NULL) return FAILURE;

	p = s;

	if (p[0] != 'r') return FAILURE;
	if (p[1] < '0' || p[1] > '7') return FAILURE;
	if (p[2] != '\0') return FAILURE;

	*reg_out = p[1] - '0';
	return SUCCESS;
}


static int parse_signed_long(const char *s, long *out_val) {
	const char *p;
	int sign;
	long v;

	if (s == NULL || out_val == NULL) return FAILURE;

	p = s;
	sign = 1;
	
	if (*p == '+' || *p == '-') {
		if (*p == '-') sign = -1;
		p++;
	}
	
	if (!isdigit((unsigned char)*p)) return FAILURE;

	v = 0;

	while (isdigit((unsigned char)*p)) {
		v = (v * 10) + (*p - '0');
		p++;
	}

	p = skip_spaces_local(p);
	if (*p != '\0') return FAILURE; /* only spaces allowed after number */

	*out_val = v * sign;
	return SUCCESS;
}

















