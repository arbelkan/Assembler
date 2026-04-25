#ifndef ASM_STATE_H
#define ASM_STATE_H

#include <stddef.h>

#include "defs.h"
#include "symbols.h"
#include "word.h"
#include "code_image.h"
#include "data_image.h"
#include "fixups.h"

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
	FixupTable fixups;
	char base_name[MAX_PATH];
} AsmState;


/* state lifecycle */
void asm_state_init(AsmState *st, const char *base_name);
void asm_state_finish_pass1(AsmState *st);
void asm_state_free(AsmState *st);

/* Emit helpers that enforce the global memory limit (code + data <= MAX_TOTAL_WORDS). */
int asm_state_emit_code(AsmState *st, int address, Word w);
int asm_state_emit_data(AsmState *st, unsigned int value);

#endif
