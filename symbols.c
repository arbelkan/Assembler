#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbols.h"
#include "errors.h"

static int ensure_capacity(SymbolTable *tab);
static int add_symbol(SymbolTable *tab, const char *name, int value, unsigned int attrs, int line_no);

void symbols_init(SymbolTable *tab) {
	if (tab == NULL) return;
	tab->arr = NULL;
	tab->count = 0;
	tab->cap = 0;
}

void symbols_free(SymbolTable *tab) {
	if (tab == NULL) return;
	free(tab->arr);
	tab->arr = NULL;
	tab->count = 0;
	tab->cap = 0;
}


Symbol *symbols_find(SymbolTable *tab, const char *name) {
	int i;
	
	if (tab == NULL || name == NULL) return NULL;

	for (i = 0 ; i < tab->count ; i++) {
		if (strcmp(tab->arr[i].name, name) == 0) {
			return &tab->arr[i];
		}
	}
	return NULL;	
}


int symbols_add_code(SymbolTable *tab, const char *name, int value, int line_no) {
	return add_symbol(tab, name, value, SYM_ATTR_CODE, line_no);
}


int symbols_add_data(SymbolTable *tab, const char *name, int value, int line_no) {
	return add_symbol(tab, name, value, SYM_ATTR_DATA, line_no);
}


int symbols_add_extern(SymbolTable *tab, const char *name, int line_no) {
	return add_symbol(tab, name, 0, SYM_ATTR_EXTERN, line_no);
}


int symbols_mark_entry(SymbolTable *tab, const char *name, int line_no) {
	Symbol *s;

	if (tab == NULL || name == NULL || name[0] == '\0') {
		print_error(ILLEGAL_SYMBOL_NAME, line_no);
		return FAILURE;
	}

	s = symbols_find(tab, name);
	if (s == NULL) {
		print_error(SYMBOL_IS_MISSING, line_no);
		return FAILURE;
	}

	if (s->attrs & SYM_ATTR_EXTERN) {
		print_error(SAME_DECLERATION_FOR_BOTH_ENTRY_AND_EXTERNAL_SYMBOL, line_no);
		return FAILURE;
	}

	s->attrs |= SYM_ATTR_ENTRY;
	return SUCCESS;
}

void symbols_relocate_data(SymbolTable *tab, int icf) {
	int i;

	if (tab == NULL) return;

	for (i = 0 ; i < tab->count ; i++) {
		if ((tab->arr[i].attrs & SYM_ATTR_DATA) && !(tab->arr[i].attrs & SYM_ATTR_EXTERN)) {
			tab->arr[i].value += icf;
		}
	}
}

/* internal helpers */
static int ensure_capacity(SymbolTable *tab) {
	Symbol *new_arr;
	int new_cap;

	if (tab->count < tab->cap) return SUCCESS;

	new_cap = (tab->cap == 0) ? 16 : (tab->cap * 2);
	new_arr = (Symbol *)realloc(tab->arr, (size_t)new_cap * sizeof(Symbol));
	if (new_arr == NULL) return FAILURE;
	
	tab->arr = new_arr;
	tab->cap = new_cap;
	return SUCCESS;
}

static int add_symbol(SymbolTable *tab, const char *name, int value, unsigned int attrs, int line_no) {
	Symbol *existing;

	if (tab == NULL || name == NULL || name[0] == '\0') {
		print_error(ILLEGAL_SYMBOL_NAME, line_no);
		return FAILURE;
	}

	if ((int)strlen(name) > SYMBOL_NAME_MAX) {
		print_error(SYMBOL_NAME_LONGER_THAN_31_CHARACTERS, line_no);
		return FAILURE;
	}

	existing = symbols_find(tab, name);
	if (existing != NULL) {
		if ((attrs & SYM_ATTR_EXTERN) && (existing->attrs & SYM_ATTR_EXTERN)) return SUCCESS; /*allow repeat .extern for the same symbol*/
		print_error(SYMBOL_ALREADY_DEFINED, line_no);
		return FAILURE;
	}

	if (ensure_capacity(tab) != SUCCESS) {
		print_error(FAILURE_IN_ADDING_MACRO_TO_TABLE, line_no);
		return FAILURE;
	}

	strncpy(tab->arr[tab->count].name, name, (size_t)SYMBOL_NAME_MAX);
	tab->arr[tab->count].name[SYMBOL_NAME_MAX] = '\0';
	tab->arr[tab->count].value = value;
	tab->arr[tab->count].attrs = attrs;

	tab->count++;
	
	return SUCCESS;
}

/* debug helper */
void symbols_dump(const SymbolTable *tab) {
	int i;
	const Symbol *s;
	char attrs_buf[8];
	int k;

	if (tab == NULL) {
		printf("SymbolTable: (null)\n");
		return;
	}

	printf("SymbolTable: count=%d\n", tab->count);
	for (i = 0 ; i < tab->count ; i++) {
		s = &tab->arr[i];
		k = 0;
		if (s->attrs & SYM_ATTR_CODE)   attrs_buf[k++] = 'C';
        	if (s->attrs & SYM_ATTR_DATA)   attrs_buf[k++] = 'D';
        	if (s->attrs & SYM_ATTR_EXTERN) attrs_buf[k++] = 'X';
        	if (s->attrs & SYM_ATTR_ENTRY)  attrs_buf[k++] = 'E';
        	if (k == 0) attrs_buf[k++] = '-';
        	attrs_buf[k] = '\0';

		printf("  %-31s  value=%4d  attrs=%s\n", s->name, s->value, attrs_buf);
	}
}