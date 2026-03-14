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
    static Mcro mcro;
    strcpy(mcro.name, name);
    mcro.content = "";
    return mcro;
}

/**
 * add content to specific macro
 */
int addContent(Mcro *mcro, const char *content)
{
    if (mcro == NULL || content == NULL)
    {
        return FAILURE;
    }

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

    fprintf(output, "%s\n", mcro->content);
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
    if (table == NULL) {
        return;
    }

    for (i = 0; i < table->count; i++) {
        free(table->macros[i].name);
        free(table->macros[i].content);
    }

    free(table->macros);
    free(table);
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
    if (table == NULL || name == NULL) {
        return FAILURE;
    }

    /*Expand table if necessary*/
    if (table->count >= table->capacity) {
        table->capacity *= 2;
        newMacros = realloc(table->macros, table->capacity * sizeof(Mcro));
        if (newMacros == NULL) {
            return FAILURE;
        }
        table->macros = newMacros;
    }

    /*Add new macro*/
    if (table->macros == NULL) {
        table->macros = malloc(table->capacity * sizeof(Mcro));
    }

    table->macros[table->count] = initMcro(name);

    table->count++;
    return SUCCESS;
}