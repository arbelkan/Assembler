#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "fixups.h"

/* internal helper */
static int ensure_capacity(FixupTable *ft);

/* public functions */
/* Initialize an empty FixupTable */
void fixups_init(FixupTable *ft) {
    if (ft == NULL) return;
    ft->arr = NULL;
    ft->count = 0;
    ft->cap = 0;
}

/* Free all heap memory owned by the table */
void fixups_free(FixupTable *ft) {
    if (ft == NULL) return;
    free(ft->arr);
    ft->arr = NULL;
    ft->count = 0;
    ft->cap = 0;
}

/* Add a new fixup entry table */
int fixups_add(FixupTable *ft, int address, AddrMode mode, const char *label) {
    Fixup *entry;

    if (ft == NULL || label == NULL)
        return FAILURE;

    if (ensure_capacity(ft) != SUCCESS)
        return FAILURE;

    entry = &ft->arr[ft->count];
    entry->address = address;
    entry->mode = mode;

    strncpy(entry->label, label, SYMBOL_NAME_MAX);
    entry->label[SYMBOL_NAME_MAX] = '\0';

    ft->count++;
    return SUCCESS;
}

/* Debug helper: print fixup table to stdout */
void fixups_dump(const FixupTable *ft) {
    int i;

    if (ft == NULL) {
        printf("FixupTable: (null)\n");
        return;
    }

    printf("FixupTable: count=%d\n", ft->count);
    for (i = 0; i < ft->count; i++) {
        printf("  [%d] addr=%04d  mode=%d  label=%s\n",
               i,
               ft->arr[i].address,
               (int)ft->arr[i].mode,
               ft->arr[i].label);
    }
}

/* Helper: grow the table capacity */
static int ensure_capacity(FixupTable *ft) {
    int new_cap;
    Fixup *new_arr;

    if (ft->count < ft->cap)
        return SUCCESS;

    new_cap = (ft->cap == 0) ? FIXUPS_INIT_CAP : (ft->cap * 2);
    new_arr = (Fixup *)realloc(ft->arr, (size_t)new_cap * sizeof(Fixup));
    if (new_arr == NULL) {
        printf("Error: fixups_add: memory allocation failed\n");
        return FAILURE;
    }

    ft->arr = new_arr;
    ft->cap = new_cap;
    return SUCCESS;
}
