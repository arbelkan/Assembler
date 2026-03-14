#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "mcro.h"

/**
 * Initialize an empty macro
 */
Mcro initMcro(const char *name)
{
    Mcro mcro;

    mcro.name = (char *)malloc(strlen(name) + 1);
    if (mcro.name == NULL)
    {
        mcro.content = NULL;
        return mcro;
    }
    strcpy(mcro.name, name);

    mcro.content = (char *)malloc(1); /* empty string: just '\0' */
    if (mcro.content == NULL)
    {
        free(mcro.name);
        mcro.name = NULL;
        return mcro;
    }
    mcro.content[0] = '\0';

    return mcro;
}

/**
 * add content to specific macro
 */
int addContent(Mcro *mcro, const char *content)
{
    char *new_content;
    size_t new_len;

    if (mcro == NULL || content == NULL)
        return FAILURE;

    new_len = strlen(mcro->content) + strlen(content) + 1;
    new_content = (char *)realloc(mcro->content, new_len);
    if (new_content == NULL)
        return FAILURE;

    mcro->content = new_content;
    strcat(mcro->content, content);
    return SUCCESS;
}

/**
 * Print the expanded macro to output
 * Prints the macro name and its full content
 */
void printMcro(Mcro *mcro, FILE *output) {
    if (mcro == NULL || output == NULL) {
        return;
    }

    fprintf(output, "%s", mcro->content);
}

/**
 * Initialize an empty macro table
 */
void initMcrotable(McroTable *table) {
    table->macros = NULL;
    table->count = 0;
    table->capacity = 0;
    return;
}

/**
 * Free all memory allocated for macro table
 */
void freeMcrotable(McroTable *table) {
    int i;

    if (table == NULL)
        return;

    for (i = 0; i < table->count; i++) {
        free(table->macros[i].name);
        free(table->macros[i].content);
    }
    free(table->macros);

    /* defensive reset */
    table->macros   = NULL;
    table->count    = 0;
    table->capacity = 0;
}

/**
 * Search for a macro in the macro table by name
 * Returns the macro if found, 0 otherwise
 */
Mcro* searchMcro(McroTable *table, const char *name) {
    int i;
    if (table == NULL || name == NULL) {
        return NULL;
    }

    for (i = 0; i < table->count; i++) {
        if (strcmp(table->macros[i].name, name) == 0) {
            return &table->macros[i];
        }
    }

    return NULL;
}

/**
 * Add a macro to the macro table
 * If the table is full, it expands the capacity
 * Returns 1 on success, 0 on failure
 */
int addMcro(McroTable *table, const char *name) {
    Mcro *newMacros = NULL;
    int new_cap;

    if (table == NULL || name == NULL)
        return FAILURE;

    /* Expand table if necessary */
    if (table->count >= table->capacity) {
        new_cap = (table->capacity == 0) ? 4 : table->capacity * 2;
        newMacros = (Mcro *)realloc(table->macros, new_cap * sizeof(Mcro));
        if (newMacros == NULL)
            return FAILURE;
        table->macros = newMacros;
        table->capacity = new_cap;
    }

    /*Add new macro*/
    table->macros[table->count] = initMcro(name);
    if (table->macros[table->count].name == NULL || table->macros[table->count].content == NULL)
        return FAILURE;

    table->count++;
    return SUCCESS;
}