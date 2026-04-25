#include <stdio.h>
#include "output_files.h"

/* internal helpers */
static void word_to_hex(unsigned int value, char *out);
static int create_ob_file(AsmState *st);
static int create_ent_file(AsmState *st);
static int create_ext_file(AsmState *st);

/* a method that creates all three output files (.ob, .ent, .ext) for the given assembled state */
int create_output_files(AsmState *st) {
    if (create_ob_file(st) != SUCCESS)
        return FAILURE;

    if (create_ent_file(st) != SUCCESS)
        return FAILURE;

    if (create_ext_file(st) != SUCCESS)
        return FAILURE;

    return SUCCESS;
}

/* helper - converts word from binary to HEX value */
static void word_to_hex(unsigned int value, char *out)
{
    unsigned int v = value & 0xFFFu;
    out[0] = "0123456789ABCDEF"[(v >> 8) & 0xF];
    out[1] = "0123456789ABCDEF"[(v >> 4) & 0xF];
    out[2] = "0123456789ABCDEF"[ v       & 0xF];
    out[3] = '\0';
}

/* helper - writes the object file (<base>.ob) */
static int create_ob_file(AsmState *st) {
    char file_name[MAX_PATH];
    char hex[4];
    FILE *fp;
    int i, addr;

    make_file_name(file_name, MAX_PATH, st->base_name, OB_EXT);

    if (open_file_for_writing(file_name, &fp) != SUCCESS)
        return FAILURE;

    /* header line: ICF (number of code words) and DCF (number of data words) */
    fprintf(fp, "  %d %d\n", st->ICF - IC_INIT , st->DCF);

    /* code image words */
    addr = IC_INIT;
    for (i = 0; i < st->ICF - IC_INIT; i++, addr++) {
        word_to_hex(st->code.words[i].value, hex);
        fprintf(fp, "%04d %s %c\n", addr, hex, st->code.words[i].are);
    }

    /* data image words (continue address numbering after code) */
    for (i = 0; i < st->DCF; i++, addr++) {
        word_to_hex(st->data.words[i].value, hex);
        fprintf(fp, "%04d %s %c\n", addr, hex, st->data.words[i].are);
    }

    fclose(fp);
    return SUCCESS;
}

/* helper - writes the entries file (<base>.ent) */
static int create_ent_file(AsmState *st) {
    char file_name[MAX_PATH];
    FILE *fp;
    int i;
    int has_entries = FALSE;
    SymbolTable *tab = &st->symbols;

    /* check whether any ENTRY symbols exist before opening the file */
    for (i = 0; i < tab->count; i++) {
        if (tab->arr[i].attrs & SYM_ATTR_ENTRY) {
            has_entries = TRUE;
            break;
        }
    }

    if (!has_entries)
        return SUCCESS; /* nothing to write - skip */

    make_file_name(file_name, MAX_PATH, st->base_name, ENT_EXT);

    if (open_file_for_writing(file_name, &fp) != SUCCESS)
        return FAILURE;

    for (i = 0; i < tab->count; i++) {
        if (tab->arr[i].attrs & SYM_ATTR_ENTRY) {
            fprintf(fp, "%s %04d\n",
                    tab->arr[i].name,
                    tab->arr[i].value);
        }
    }

    fclose(fp);
    return SUCCESS;
}

/* helper - writes the externals file (<base>.ext) */
static int create_ext_file(AsmState *st) {
    char file_name[MAX_PATH];
    FILE *fp;
    int i;
    int has_externs = FALSE;
    FixupTable *ft = &st->fixups;
    Symbol *sym;

    /* check whether any fixup references an EXTERN symbol */
    for (i = 0; i < ft->count; i++) {
        sym = symbols_find(&st->symbols, ft->arr[i].label);
        if (sym != NULL && (sym->attrs & SYM_ATTR_EXTERN)) {
            has_externs = TRUE;
            break;
        }
    }

    if (!has_externs)
        return SUCCESS; /* nothing to write - skip */

    make_file_name(file_name, MAX_PATH, st->base_name, EXT_EXT);

    if (open_file_for_writing(file_name, &fp) != SUCCESS)
        return FAILURE;

    /* one line per fixup that resolved to an extern symbol */
    for (i = 0; i < ft->count; i++) {
        sym = symbols_find(&st->symbols, ft->arr[i].label);
        if (sym != NULL && (sym->attrs & SYM_ATTR_EXTERN)) {
            fprintf(fp, "%s %04d\n", ft->arr[i].label, ft->arr[i].address);
        }
    }

    fclose(fp);
    return SUCCESS;
}