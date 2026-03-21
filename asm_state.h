#ifndef ASM_STATE_H
#define ASM_STATE_H

#include <stddef.h>

#include "defs.h"
#include "symbols.h"
#include "word.h"
#include "code_image.h"
#include "data_image.h"
#include "fixups.h" /* TODO - documentation for Shahr what has changed */

#define MAX_PATH 1024
#define MAX_LABEL_LEN 31 	/*temporary*/


typedef struct {
	int IC;
	int DC;
	int ICF;
	int DCF;
	CodeImage code;
	DataImage data;
	SymbolTable symbols;
	FixupTable fixups; /* TODO - documentation for Shahr what has changed */
	char base_name[MAX_PATH];
} AsmState;


/* state lifecycle */
void asm_state_init(AsmState *st, const char *base_name);
void asm_state_finish_pass1(AsmState *st);
void asm_state_free(AsmState *st);

#endif
