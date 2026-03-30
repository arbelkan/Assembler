/*NOTE: minimal asm_state.c so i can compile and test assembler.c */
#include <string.h>
#include <stdio.h>
#include "asm_state.h"



/* state lifecycle */ 

/* initial state (of 1 file) to clean */
void asm_state_init(AsmState *st, const char *base_name) {	
	if (st == NULL) return;

	/* initialize counters */
	st->IC = IC_INIT;
	st-> DC = 0;
	st->ICF = 0;
	st->DCF = 0;

	/* initialize code image (CodeImage code) */
	code_image_init(&st->code);

	/* initialize data image (DataImage data) */
	st->data.count = 0;
	data_image_init(&st->data);

	/* initialize symbol table (SymbolTable symbols) */ 
	symbols_init(&st->symbols);

	/* store base file name safely */
	if (base_name != NULL) {
		strncpy(st->base_name, base_name, (size_t)(sizeof(st->base_name) - 1));
		st->base_name[sizeof(st->base_name) - 1] = '\0';
	}
	else st->base_name[0] = '\0';
}



/* closing pass1 (for 1 file) */
void asm_state_finish_pass1(AsmState *st) {
	int code_words;
	int data_words;	

	if (st == NULL) return;

	/* freeze finale counters ICF DCF */
	st->ICF = st->IC;
	st->DCF = st->DC;

	code_words = st->ICF - IC_INIT;
	data_words = st->DCF;

	if ((code_words + data_words) > MAX_TOTAL_WORDS) {
		printf("Error: program too large after pass1 (code=%d, data=%d, max=%d)\n", code_words, data_words, MAX_TOTAL_WORDS);
		return;
	}

	symbols_relocate_data(&st->symbols, st->ICF);
	
}

/* free sources */
/* NOTE: needed only if i use dynamic allocate */
void asm_state_free(AsmState *st) {
	if (st == NULL) return;

	symbols_free(&st->symbols);

	/*
	*
	* TODO (later): if CodeImage/DataImage/SymbolTable/Fixups become dynamic, free them here:
	*   code_image_free(&st->code);
	*   data_image_free(&st->data);
	*   symbols_free(&st->symbols); - done
	*   fixups_free(&st->fixups);
	*/
}



int asm_state_emit_code(AsmState *st, int address, Word w) {
	int code_words;
	int data_words;

	if (st == NULL) return FAILURE;

	/* code_words is based on code image count (words written so far) */
	code_words = st->code.count;
	data_words = st->data.count;

	/* Enforce global memory: code + data cannot exceed MAX_TOTAL_WORDS */
	if ((code_words + data_words + 1) > MAX_TOTAL_WORDS) {
		printf("Error: program too large (code=%d, data=%d, max=%d)\n", code_words, data_words, MAX_TOTAL_WORDS);
		return FAILURE;
	}
	
	return code_image_emit(&st->code, address, w);
}




int asm_state_emit_data(AsmState *st, unsigned int value) {
	int code_words;
	int data_words;

	if (st == NULL) return FAILURE;

	code_words = st->code.count;
	data_words = st->data.count;

	if ((code_words + data_words + 1) > MAX_TOTAL_WORDS) {
		printf("Error: program too large (code=%d, data=%d, max=%d)\n", code_words, data_words, MAX_TOTAL_WORDS);
		return FAILURE;
	}

	return data_image_emit(&st->data, value);
}






