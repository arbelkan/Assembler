#ifndef MCRO_H
#define MCRO_H

/*Structure to represent a single macro*/
typedef struct {
    char *name;/*Macro name*/
    char *content;/*Macro content*/
} Mcro;

/*Structure to represent the macro table*/
typedef struct {
    Mcro *macros;/*Array of macros*/
    int count;/*Number of macros in table*/
    int capacity;/*Capacity of macros array*/
} McroTable;

/*Function declarations*/

/**
 * Initialize a new macro
 * @param name - name of the new macro
 * @return pointer to new Mcro, NULL on failure
 */
Mcro initMcro(const char *name);

/**
 * add content to specific macro
 * @param mcro - pointer to the specific macro
 * @param content - the content to add
 * @return success or failure
 */
int addContent(Mcro *mcro, const char *content);

/**
 * Initialize an empty macro table
 * @param table - pointer to new McroTable
 */
void initMcrotable(McroTable *table);

/**
 * Free all memory allocated for macro table
 * @param table - pointer to McroTable to free
 */
void freeMcrotable(McroTable *table);

/**
 * Search for a macro in the macro table by name
 * @param table - pointer to McroTable
 * @param name - macro name to search for
 * @return pointer to Mcro if found, NULL otherwise
 */
Mcro* searchMcro(McroTable *table, const char *name);

/**
 * Add a macro to the macro table with empty content
 * @param table - pointer to McroTable
 * @param name - macro name
 * @return success or failure
 */
int addMcro(McroTable *table, const char *name);

/**
 * Print the expanded macro to output
 * @param mcro - pointer to mcro to print
 * @param output - FILE pointer to output stream
 */
void printMcro(Mcro *mcro, FILE *output);

#endif