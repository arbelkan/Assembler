#ifndef FIXUPS_H
#define FIXUPS_H

#include "symbols.h"
#include "ops.h"

#define FIXUPS_INIT_CAP 16

/*
 * A Fixup records one operand word that could not be resolved in pass1 because it references a label.
 * Pass2 will iterate the fixup table and patch each entry in the code image.
 * Fields:
 *   address - the code image address (IC value) of the word to patch
 *   mode - ADDR_DIRECT or ADDR_RELATIVE
 *   label - the referenced symbol name (null-terminated)
 */
typedef struct {
    int address; /* code image address to patch */
    AddrMode mode; /* ADDR_DIRECT or ADDR_RELATIVE */
    char label[SYMBOL_NAME_MAX + 1]; /* referenced symbol name */
} Fixup;

/*
 * FixupTable - dynamic array of Fixup entries (mirrors the design of SymbolTable)
 */
typedef struct {
    Fixup *arr;
    int count;
    int cap;
} FixupTable;

/* Initialize an empty FixupTable
 * @param ft - the FixupTable to initialize
 */
void fixups_init(FixupTable *ft);

/* Free all heap memory owned by the table.
 * @param ft - the FixupTable to free
 */
void fixups_free(FixupTable *ft);

/*
 * Add a new fixup entry to table.
 * @param ft - the FixupTable that will be expanded
 * @param address - the new fixup adrress
 * @param mode - the new fixup mode (E / R)
 * @param lable - the lable name of the new fixup
 * @return SUCCESS or FAILURE (on allocation error).
 */
int fixups_add(FixupTable *ft, int address, AddrMode mode, const char *label);

/* Debug helper: print fixup table to stdout.
 * @param ft - the FixupTable to be printed
 */
void fixups_dump(const FixupTable *ft);


#endif