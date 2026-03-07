#include <stdio.h>
#include "data_image.h"

static void print_bin12(unsigned int v);

void data_image_init(DataImage *di) {
	int i;

	if (di == NULL) return;

	di->count = 0;
	
	for (i = 0 ; i < MAX_DATA_WORDS ; i++) {
		di->words[i].value = 0u;
		di->words[i].are = 'A';
	}
}



int data_image_emit(DataImage *di, unsigned int value) {
	if (di == NULL) return FAILURE;

	if (di->count >= MAX_DATA_WORDS) {
		printf("Error: DataImage overflow (MAX_DATA_WORDS=%d)\n", MAX_DATA_WORDS);
		return FAILURE;
	}

	di->words[di->count].value = (value & 0x0FFFu);
	di->words[di->count].are = 'A';
	di->count++;

	return SUCCESS;
}


/*TODO: delete after testing (temporary debug)*/
static void print_bin12(unsigned int v) {
	int i;

	v &= 0x0FFFu;
	for (i = 11 ; i >= 0 ; i--) {
		putchar((v & (1u << (unsigned int)i)) ? '1' : '0');
	}
}



void data_image_dump(const DataImage *di, int icf) {
	int i;

	if (di == NULL) {
		printf("DataImage: (null)\n");
		return;
	}

	printf("DataImage: count=%d\n", di->count);
	for (i = 0 ; i < di->count ; i++) {
		printf("  %04d: ", i + icf);
		print_bin12(di->words[i].value);
		printf(" are=%c\n", di->words[i].are);
	}
}




