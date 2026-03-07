#ifndef CODE_IMAGE_H
#define CODE_IMAGE_H

#include "defs.h"
#include "word.h"

#define MAX_CODE_WORDS 2048

typedef struct {
	Word words[MAX_CODE_WORDS];
	int count; /* number of words written */
} CodeImage;


/* initialize an empty CodeImage */
void code_image_init(CodeImage *ci);


/* Emit a word at a specific memory address (IC address, starting at IC_INIT).
 * Updates ci->count accordingly.
 * Returns SUCCESS/FAILURE.
 */
int code_image_emit(CodeImage *ci, int address, Word w);


/* same as code_image_emit but for pass2 fixup */
int code_image_set(CodeImage *ci, int address, Word w);

/* Debug helper (temporary): print code image to stdout. TODO: delete after testing */
void code_image_dump(const CodeImage *ci);

#endif


