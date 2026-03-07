#include <stdio.h>
#include "code_image.h"

void code_image_init(CodeImage *ci) {
	int i;

	if (ci == NULL) return;

	ci->count = 0;

	/* initialize memory to a safe default */
	for (i = 0 ; i < MAX_CODE_WORDS ; i++) {
		ci->words[i].value = 0u;
		ci->words[i].are = 'A';
	}
}



static int address_to_index(int address) {
	return (address - IC_INIT);
}



int code_image_emit(CodeImage *ci, int address, Word w) {
	int idx;

	if (ci == NULL) return FAILURE;

	idx = address_to_index(address);
	if (idx < 0 || idx >= MAX_CODE_WORDS) {
		printf("Error: CodeImage address out of range: %d\n", address);
		return FAILURE;
	}

	ci->words[idx] = w;

	if (idx + 1 > ci->count) {
		ci->count = idx + 1;
	}

	return SUCCESS;
}



int code_image_set(CodeImage *ci, int address, Word w) {
	return code_image_emit (ci, address, w);
}


/*TODO: delete after testing (temporary debug)*/
static void print_bin12(unsigned int v) {
	int i;
	v &= 0x0FFFu;
	
	for (i = 11 ; i >= 0 ; i--) {
		putchar((v & (1u << (unsigned int)i)) ? '1' : '0');
	}
}

void code_image_dump(const CodeImage *ci)
{
    int i;
    int addr;

    if (ci == NULL) {
        printf("CodeImage: (null)\n");
        return;
    }

    printf("CodeImage: count=%d\n", ci->count);

    for (i = 0; i < ci->count; i++) {
        addr = IC_INIT + i;
	printf("  %04d: ", addr);
	print_bin12(ci->words[i].value);
	printf(" are=%c\n", ci->words[i].are);
        /*printf("  %04d: value=0x%03X are=%c\n", addr, (unsigned int)(ci->words[i].value & 0x0FFFu), ci->words[i].are);*/
    }
}


























