#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "defs.h" /* for SUCCESS & FAILURE */

#define SYMBOL_NAME_MAX 31  /* consistent with MAX_LABEL_LEN */

/* Symbol attributes bitmask */
#define SYM_ATTR_CODE   0x01u
#define SYM_ATTR_DATA   0x02u
#define SYM_ATTR_EXTERN 0x04u
#define SYM_ATTR_ENTRY  0x08u

typedef struct {
	char name[SYMBOL_NAME_MAX + 1];
	int value; /* address or 0 if extern */
	unsigned int attrs;
} Symbol;

typedef struct {
	Symbol *arr;
	int count;
	int cap;
} SymbolTable;

void symbols_init(SymbolTable *tab);
void symbols_free(SymbolTable *tab);

Symbol *symbols_find(SymbolTable *tab, const char *name);

/* Add symbol kinds (return SUCCESS/FAILURE) */
int symbols_add_code(SymbolTable *tab, const char *name, int value, int line_no);
int symbols_add_data(SymbolTable *tab, const char *name, int value, int line_no);
int symbols_add_extern(SymbolTable *tab, const char *name, int line_no);

/* mark an existing symbol as ENTRY */
int symbols_mark_entry(SymbolTable *tab, const char *name, int line_no);

/* after pass1 - add ICF to all DATA symbols */
void symbols_relocate_data(SymbolTable *tab, int icf);

/* Debug helper (temporary): print symbol table to stdout */
void symbols_dump(const SymbolTable *tab);

#endif

