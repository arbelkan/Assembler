#include <stdio.h>
#include <string.h>
#include "mcro.h"
#include "defs.h"
#include "handle_files.h"
#include "errors.h"
#include "preassembler_parser.h"
#include "preassembler.h"

/* helpers */
static const char *skip_spaces(const char *p);
static void get_first_token(const char *line, char *token, int token_size);

/* runs preassembler process */
int run_preassembler(McroTable *table, const char *base_name) {
    FILE *input = NULL;
    FILE *output = NULL;
    char src_name[MAX_PATH];
    char out_name[MAX_PATH];
    char line[LINE_BUF_SIZE];
    char token[LINE_BUF_SIZE];
    int  in_macro = FALSE ;
    int  line_number = 0;
    int  status = SUCCESS;
    int current_mcro = -1;
    Mcro *found = NULL;

    if (table == NULL || base_name == NULL)
        return FAILURE;

    /* build source and destination filenames */
    make_file_name(src_name, MAX_PATH, base_name, ".as");
    make_file_name(out_name, MAX_PATH, base_name, ".am");

    /* open files */
    if (open_file_for_reading(src_name, &input) == FAILURE)
        return FAILURE;
    if (open_file_for_writing(out_name, &output) == FAILURE) {
        fclose(input);
        return FAILURE;
    }

    /* Main loop - step 1 */
    while (fgets(line, LINE_BUF_SIZE, input) != NULL) {
        line_number++;

        /* checks if line is too long */
        if (strlen(line) > MAX_LINE && line[MAX_LINE] != '\n' && line[MAX_LINE] != '\0') {
            print_error(LINE_LONGER_THAN_80_CHARACTERS, line_number);
            /* not an error in the preassembler process and will not change status to FAILURE */
        }

        /* Step 3: "mcro" declaration? */
        if (find_mcro(line) == SUCCESS && find_mcroend(line) == FAILURE) {
            if (validate_start_mcro(line, line_number) == FAILURE) {
                status = FAILURE;
            }

            /* Step 4: raise "in macro" flag */
            in_macro = TRUE;

            /* Step 5: add new macro to table */
            if (add_mcro(table, extract_mcro_name(line)) == FAILURE) {
                print_error(FAILURE_IN_ADDING_MACRO_TO_TABLE, line_number);
                status = FAILURE;
                in_macro = 0;
            }
            current_mcro = table->count - 1;
            continue; /* declaration line not written to output */
        }

        /* Steps 6-8: inside a macro body */
        if (in_macro) {
            /* Step 7: mcroend? */
            if (find_mcroend(line) == SUCCESS) {
                if (validate_end_mcro(line, line_number) == FAILURE) {
                    status = FAILURE;
                }

                /* Step 8: turn off flag */
                in_macro = FALSE;
                current_mcro = -1;
                continue; /* mcroend line not written to output */
            }

            /* Step 6: accumulate into macro content */
            if (add_content(&table->macros[current_mcro], line) == FAILURE)
                status = FAILURE;
            continue; /* body line not written to output */
        }

        /* Step 2: is the first token a macro name? */
        get_first_token(line, token, LINE_BUF_SIZE);
        found = search_mcro(table, token);
        if (found != NULL) {
            print_mcro(found, output);
            continue;
        }
        fputs(line, output); /* Regular line: copy to output file */
    }

    /* Step 9: close files */
    fclose(input);
    fclose(output);

    /* if any error occurred — delete the .am file */
    if (status == FAILURE) {
        remove(out_name);
    }

    return status;
}

/* Helper: Skip leading whitespace */
static const char *skip_spaces(const char *p) {
	while (*p && (*p == ' ' || *p == '\t')) {
		p++;
	}
	return p;
}

/* Helper: Extracts the first whitespace-delimited token from line into token */
static void get_first_token(const char *line, char *token, int token_size) {
	int i = 0;
	const char *p = skip_spaces(line); /* skip leading whitespace */
	while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r' && i < token_size - 1)
		token[i++] = *p++;
	token[i] = '\0';
}