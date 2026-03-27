#include <stdio.h>
#include <ctype.h>
#include "pass2.h"
#include "handle_files.h"
#include "line_reader.h"
#include "parser.h"
#include "symbols.h"
#include "fixups.h"
#include "code_image.h"
#include "word.h"
#include "ops.h"
#include "errors.h"

/* Internal helpers */
static int handle_entry_directive(AsmState *st, const ParsedLine *pl, int line_number);
static int resolve_one_fixup(AsmState *st, const Fixup *fix);

/* second pass over the .am file */
int pass2_run(AsmState *st, const char *am_filename) {
    char am_file[MAX_PATH];
    FILE *fp;
    char line[LINE_BUF_SIZE];
    int line_number = 0;
    int too_long = FALSE;
    int rc, i;
    int ok = SUCCESS;
    ParsedLine pl;

    if (st == NULL || am_filename == NULL) {
        printf("Error: pass2_run received NULL arguments\n");
        return FAILURE;
    }

    /* build and open the .am filename using handle_files */
    make_file_name(am_file, MAX_PATH, am_filename, AM_EXT);
    if (open_file_for_reading(am_file, &fp) != SUCCESS) {
        return FAILURE;
    }

    /* line loop: steps 1 - 4 */
    while ((rc = read_line(fp, line, (int)sizeof(line), &line_number, &too_long)) == LR_OK) {
        if (parse_line(line, &pl) != SUCCESS) {
            continue;
        }

        if (pl.kind == LINE_EMPTY || pl.kind == LINE_COMMENT) {
            continue;
        }
		/* step 4 - handle .entry operands */
        if (pl.kind == LINE_DIRECTIVE && pl.dir_kind == DIR_ENTRY) {
            if (handle_entry_directive(st, &pl, line_number) != SUCCESS) {
                ok = FAILURE;
            }
        }
    }

    fclose(fp);

	fixups_dump(&st->fixups); /* temporary debug. TODO: delete after testing*/

    if (rc == LR_FAIL) {
        printf("Error: read_line failed while reading %s\n", am_file);
        return FAILURE;
    }

    /* step 5 - after all lines read: for each entry in the fixup table, look up the label in the symbol table */
    for (i = 0; i < st->fixups.count; i++) {
        if (resolve_one_fixup(st, &st->fixups.arr[i]) != SUCCESS) {
            ok = FAILURE;
        }
    }

	/* step 6 - check for any errors */
    if (ok != SUCCESS) {
        return FAILURE;
    }

	/* step 7 - create output files */
    /* TODO- output creation function module and functions */
    return ok;
}

/* helper - .entry handler */
static int handle_entry_directive(AsmState *st, const ParsedLine *pl, int line_number) {
    Symbol *sym = symbols_find(&st->symbols, pl->args);

    /* check extern conflict before marking entry */
    if (sym != NULL && (sym->attrs & SYM_ATTR_EXTERN)) {
        printError(SAME_DECLERATION_FOR_BOTH_ENTRY_AND_EXTERNAL_SYMBOL, (unsigned int)line_number);
        return FAILURE;
    }

    if (symbols_mark_entry(&st->symbols, pl->args, line_number) != SUCCESS) {
        printError(SYMBOL_NOT_DEFINED, (unsigned int)line_number);
        return FAILURE;
    }

    return SUCCESS;
}

/* helper - resolve a single fixup and patch one word in the code image */
static int resolve_one_fixup(AsmState *st, const Fixup *fix) {
    Symbol *sym = symbols_find(&st->symbols, fix->label);
    Word w;
    int dist;

    if (sym == NULL) {
        printError(SYMBOL_NOT_DEFINED, (unsigned int)fix->line_number);
        return FAILURE;
    }

    w.value = 0u;
    w.are = 'A';

    if (sym->attrs & SYM_ATTR_EXTERN) {
        w.value = 0u;
        w.are = 'E';
        /* TODO: record use-address for .ext output file */
    }
    else if (fix->mode == ADDR_DIRECT) {
        w.value = (unsigned int)sym->value & 0x0FFFu;
        w.are = 'R';
    }
    else { /* ADDR_RELATIVE */
        dist = sym-> value - fix->address;
        w.value = (unsigned int)dist & 0x0FFFu;
        w.are = 'A';
    }

    if (code_image_set(&st->code, fix->address, w) != SUCCESS) {
        printError(SYMBOL_NOT_DEFINED, (unsigned int)fix->line_number);
        return FAILURE;
    }

    return SUCCESS;
}