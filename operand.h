#ifndef OPERAND_H
#define OPERAND_H

#include "ops.h"
#include "parser.h" /*to use is_valid_label*/
#include "defs.h"

#define OPERAND_LABEL_MAX 31

typedef struct {
	AddrMode mode;
	int reg_num; /*valid only if mode is ADDR_REGISTER*/
	long imm_value; /*valid only if mode is ADDR_IMMEDIATE*/
	char label[OPERAND_LABEL_MAX + 1]; /*valid only if mode is ADDR_DIRECT/ADDR_RELATIVE*/
} Operand;


/* parse only single operand token, returns SUCCESS/FAILURE */
int operand_parse(const char *token, Operand *out, int line_no);


#endif


