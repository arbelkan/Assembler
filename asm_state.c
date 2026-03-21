/*NOTE: minimal asm_state.c so i can compile and test assembler.c */
#include <string.h>
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

	/* initialize symbol table (SymbolTable symbols) - placeholder for now*/ 
	symbols_init(&st->symbols);

	/* TODO - documentation for Shahr what has changed */
	/* initialize fixups table*/
	fixups_init(&st->fixups);

	/* store base file name safely */
	if (base_name != NULL) {
		strncpy(st->base_name, base_name, (size_t)(sizeof(st->base_name) - 1));
		st->base_name[sizeof(st->base_name) - 1] = '\0';
	}
	else st->base_name[0] = '\0';
}



/* closing pass1 (for 1 file) */
void asm_state_finish_pass1(AsmState *st) {
	if (st == NULL) return;

	/* freeze finale counters ICF DCF */
	st->ICF = st->IC;
	st->DCF = st->DC;

	symbols_relocate_data(&st->symbols, st->ICF);
	/*
	* TODO (later): relocate all DATA symbols by adding ICF:
	* symbols_relocate_data(&st->symbols, st->ICF);
	*
	* This is required because data addresses come after the code image.
	*/
}

/* free sources */
/* NOTE: needed only if i use dynamic allocate */
void asm_state_free(AsmState *st) {
	if (st == NULL) return;

	symbols_free(&st->symbols);

	fixups_free(&st->fixups); /* TODO - documentation for Shahr what has changed */

	/*
	*
	* TODO (later): if CodeImage/DataImage/SymbolTable/Fixups become dynamic, free them here:
	*   code_image_free(&st->code);
	*   data_image_free(&st->data);
	*   symbols_free(&st->symbols); - done
	*   fixups_free(&st->fixups);
	*/
}
