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
static int handle_instruction_fixups(AsmState *st, int line_number);
static int resolve_one_fixup(AsmState *st, const Fixup *fix, int line_number);

static int fixup_cursor = 0; /* Module-level fixup cursor */

/* second pass over the .am file */
int pass2_run(AsmState *st, const char *am_filename) {
    char am_file[MAX_PATH];
    FILE *fp;
    char line[LINE_BUF_SIZE];
    int line_number = 0;
    int too_long = FALSE;
    int rc;
    int ok = SUCCESS;
    ParsedLine pl;

    if (st == NULL || am_filename == NULL) {
        printf("Error: pass2_run received NULL arguments\n");
        return FAILURE;
    }

    fixup_cursor = 0; /* reset fixup cursor for this file */

    /* build and open the .am filename using handle_files */
    make_file_name(am_file, MAX_PATH, am_filename, AM_EXT);
    if (open_file_for_reading(am_file, &fp) != SUCCESS) {
        return FAILURE;
    }

    /* Main loop - step 1 */
    while ((rc = read_line(fp, line, (int)sizeof(line), &line_number, &too_long)) == LR_OK) {
        if (parse_line(line, &pl) != SUCCESS) {
            /* malformed line - pass1 already caught and reported this */
            continue;
        }

        /* step 2: checks if the first field is a label declaration */
        if (pl.kind == LINE_EMPTY || pl.kind == LINE_COMMENT) {
            continue;
        }

        if (pl.kind == LINE_DIRECTIVE) {
            /* step 3: checks if .data / .string / .extern */
            if (pl.dir_kind == DIR_DATA || pl.dir_kind == DIR_STRING || pl.dir_kind == DIR_EXTERN) {
                continue;
            }

            /* step 4: checks if .entry and mark symbol as ENTRY */
            if (pl.dir_kind == DIR_ENTRY) {
                if (handle_entry_directive(st, &pl, line_number) != SUCCESS) {
                    ok = FAILURE;
                }
                continue;
            }
        }

        else if (pl.kind == LINE_INSTRUCTION) {
            /* step 5: resolve all label-operand fixups for this instruction */
            if (handle_instruction_fixups(st, line_number) != SUCCESS) {
                ok = FAILURE;
            }
        }
    }

    fclose(fp);

    if (rc == LR_FAIL) {
        printf("Error: read_line failed while reading %s\n", am_file);
        return FAILURE;
    }

    /* step 6: checks if pass2 process has failed or succeed */
    if (ok != SUCCESS) {
        return FAILURE;
    }

    /* step 7: creates output files */
    /* TODO- output creation function module and functions */
    return ok;
}

/* helper - .entry handler */
static int handle_entry_directive(AsmState *st, const ParsedLine *pl, int line_number) {
    Symbol *sym;

    /* in case of undfined symbol */
    if (symbols_mark_entry(&st->symbols, pl->args, line_number) != SUCCESS) {
        printError(SYMBOL_NOT_DEFINED, (unsigned int)line_number);
        return FAILURE;
    }

    /* in case of a symbol declared both for .extern and .entry */
    sym = symbols_find(&st->symbols, pl->args);
    if (sym != NULL && (sym->attrs & SYM_ATTR_EXTERN)) {
        printError(SAME_DECLERATION_FOR_BOTH_ENTRY_AND_EXTERNAL_SYMBOL,(unsigned int)line_number);
        return FAILURE;
    }

    return SUCCESS;
}

/* helper - resolve fixups for the current instruction line */
static int handle_instruction_fixups(AsmState *st, int line_number) {
    int ok = SUCCESS;
    Fixup *fix = NULL;
    Fixup *prev = NULL;

    while (fixup_cursor < st->fixups.count) {
        *fix = st->fixups.arr[fixup_cursor];

        /* stop when we reach the first fixup of the next instruction */
        if (fixup_cursor > 0) {
            *prev = st->fixups.arr[fixup_cursor - 1];
            if (fix->address - prev->address > 1) {
                break;
            }
        }

        if (resolve_one_fixup(st, fix, line_number) != SUCCESS) {
            ok = FAILURE;
        }

        fixup_cursor++;
    }

    return ok;
}

/* helper - resolve a single fixup and patch one word in the code image */
static int resolve_one_fixup(AsmState *st, const Fixup *fix, int line_number) {
    Symbol *sym;
    Word w;
    int dist;

    sym = symbols_find(&st->symbols, fix->label);
    if (sym == NULL) {
        printError(SYMBOL_NOT_DEFINED, (unsigned int)line_number);
        return FAILURE;
    }

    w.value = 0u;
    w.are   = 'A';

    /* in case of external symbol */
    if (sym->attrs & SYM_ATTR_EXTERN) {
        w.value = 0u;
        w.are = 'E';
        /* TODO: record use-address for .ext output file when implemented */
    }

    /* in case of direct addressing */
    else if (fix->mode == ADDR_DIRECT) {
        w.value = (unsigned int)sym->value & 0x0FFFu;
        w.are = 'R';
    }

    /* in case of relative addressing */
    else {
        dist = sym->value - (fix->address - 1);
        w.value = (unsigned int)dist & 0x0FFFu;
        w.are = 'A';
    }

    /* overwrite the placeholder word that pass1 left in the code image */
    if (code_image_set(&st->code, fix->address, w) != SUCCESS) {
        printf("Error (line %d): failed to patch code image at address %d\n", line_number, fix->address);
        return FAILURE;
    }

    return SUCCESS;
}