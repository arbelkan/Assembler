#ifndef OPS_H
#define OPS_H

#include "defs.h"

/* addressing modes */
typedef enum {
	ADDR_IMMEDIATE = 0, /*number*/
	ADDR_DIRECT = 1, /*label*/
	ADDR_RELATIVE = 2, /*%label*/
	ADDR_REGISTER = 3 /*registers: r0,...,r7*/
} AddrMode;

/* bitmasks for allowed addressing modes */
#define MASK_IMM (1u << ADDR_IMMEDIATE)
#define MASK_DIR (1u << ADDR_DIRECT)
#define MASK_REL (1u << ADDR_RELATIVE)
#define MASK_REG (1u << ADDR_REGISTER)

typedef struct {
	const char *name;
	unsigned int opcode; /*0,...,15*/
	unsigned int funct; /*0 or specific*/
	unsigned int operands; /*0/1/2*/
	unsigned int src_mask;
	unsigned int dst_mask;
} OpInfo;


/* returns pointer to OpInfo if found, NULL otherwise */
const OpInfo *op_find(const char *name);


#endif
