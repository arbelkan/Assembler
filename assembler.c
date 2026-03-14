#include <stdio.h>
#include <string.h>

#include "asm_state.h"
#include "pass1.h"
#include "code_image.h"
#include "data_image.h"
#include "mcro.h"
#include "preassembler.h"

#define SUCCESS 0
#define FAILURE 1

#define MAX_PATH 1024

static void print_usage(const char *prog);
static void make_filename(char *out, int out_size, const char *base, const char *ext);
static int process_one_file_pass1(const char *base_name);
static int process_one_file_preassembler(const char *base_name);

int main (int argc, char *argv[]) {
	int i;
	int any_failed = 0;

	if (argc < 2) { 	/*if 0 input in command line*/
		print_usage(argv[0]);
		return FAILURE;
	}

	/* loop on the files given in the command line as input */
	for (i = 1 ; i < argc ; i++) {
		if (process_one_file_preassembler(argv[i]) != SUCCESS) /* checks if preassmbler proccess has failed */
			return FAILURE;
	}

/*	for (i = 1 ; i < argc ; i++) {		/* loop on the files given in the command line as input */
/*		if (process_one_file_pass1(argv[i]) != SUCCESS) 	/* !!!now start process_one_file_pass1 on each file!!! */
/*			any_failed = 1;
	}
*/
	return any_failed ? FAILURE : SUCCESS;	
}

static int process_one_file_preassembler(const char *base_name) {
	McroTable table;
	int status;

	initMcrotable(&table); /* initialize an empty mcro table */
	status = run_preassembler(&table, base_name);

	/* clean up */
	freeMcrotable(&table);
	return status;
}

static int process_one_file_pass1(const char *base_name) {
	char am_file[MAX_PATH];
	AsmState st;
	int ok;

	make_filename(am_file, MAX_PATH, base_name, ".am");
	asm_state_init(&st, base_name); 	/* initial work state of pass1 to the current AsmState st */
	
	ok = pass1_run(&st, am_file); 	/* !!!now start pass1 on AsmState st and create its output in am_file!!! */

	if (ok == SUCCESS) {
		asm_state_finish_pass1(&st);
		code_image_dump(&st.code); /* temporary debug. TODO: delete after testing*/
		data_image_dump(&st.data, st.ICF); /* temporary debug. TODO: delete after testing*/
		symbols_dump(&st.symbols); /* temporary debug. TODO: delete after testing*/
	}
	
	asm_state_free(&st);

	if (ok == FAILURE) {
		printf("Error: failed processing %s\n", am_file); 	/* TODO: later: replace all ERROR MSG to error module */
		return FAILURE;
	}
	return SUCCESS;
}



static void print_usage(const char *prog) {
	printf("Usage: %s file1 [file2...]\n", prog);
	printf("Note: this stage expects input files with .am extension (for example: file1.am)\n");
}



static void make_filename(char *out, int out_size, const char *base, const char *ext) {
	int base_len, ext_len;
	
	if (out_size <= 0) return;

	base_len = (int)strlen(base);
	ext_len = (int)strlen(ext);

	if ((base_len + ext_len) >= out_size) {
		base_len = out_size - ext_len - 1;
		if (base_len < 0) base_len = 0;
	}

	if (base_len > 0) strncpy(out, base, (size_t)base_len);

	out[base_len] = '\0';
	strncat(out, ext, (size_t)(out_size - base_len - 1));
}
