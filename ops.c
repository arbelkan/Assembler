#include <string.h>
#include "ops.h"

/* the operation table (16 ops) */
static const OpInfo op_table[] = {
	/* { name, opcode, funct, operands, src_mask, dst_mask } */
	{ "mov", 0, 0, 2, MASK_IMM|MASK_DIR|MASK_REG, MASK_DIR|MASK_REG },
	{ "cmp", 1, 0, 2, MASK_IMM|MASK_DIR|MASK_REG, MASK_IMM|MASK_DIR|MASK_REG },
	{ "add", 2, 10, 2, MASK_IMM|MASK_DIR|MASK_REG, MASK_DIR|MASK_REG },
	{ "sub", 2, 11, 2, MASK_IMM|MASK_DIR|MASK_REG, MASK_DIR|MASK_REG },
	{ "lea", 4, 0, 2, MASK_DIR, MASK_DIR|MASK_REG },
	{ "clr", 5, 10, 1, 0u, MASK_DIR|MASK_REG },
	{ "not", 5, 11, 1, 0u, MASK_DIR|MASK_REG },
	{ "inc", 5, 12, 1, 0u, MASK_DIR|MASK_REG },
	{ "dec", 5, 13, 1, 0u, MASK_DIR|MASK_REG },
	{ "jmp", 9, 10, 1, 0u, MASK_DIR|MASK_REL },
	{ "bne", 9, 11, 1, 0u, MASK_DIR|MASK_REL },
	{ "jsr", 9, 12, 1, 0u, MASK_DIR|MASK_REL },
	{ "red", 12, 0, 1, 0u, MASK_DIR|MASK_REG },
	{ "prn", 13, 0, 1, 0u, MASK_IMM|MASK_DIR|MASK_REG },
	{ "rts", 14, 0, 0, 0u, 0u },
	{ "stop",15,  0, 0, 0u, 0u }
};


const OpInfo *op_find(const char *name) {
	int i;
	int n;

	if (name == NULL) return NULL;

	n = (int)(sizeof(op_table) / sizeof(op_table[0]));
	for (i = 0 ; i < n ; i++) {
		if (strcmp(op_table[i].name, name) == 0) {
			return &op_table[i];
		}
	}

	return NULL;
}
