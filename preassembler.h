#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H

#define MAX_LINE 80
#define LINE_BUF_SIZE (MAX_LINE + 2)
#define MAX_PATH 1024

/** runs preassembler process by following algo:
 * 1. Read the next line from the source file. If EOF → go to step 9 (end).
 * 2. Is the first field a macro name in the macro table (e.g., GEN_MC)? If yes, replace it in place and copy all matching lines from the table into the file, go back to step 1. Otherwise, continue.
 * 3. Is the first field "mcro" (start of macro definition)? If no, go to step 6
 * 4. raise the "in macro" flag
 * 5. create new macro in macro table with mcro name (matching GEN_MC).
 * 6. Read the next line from the source file. If EOF → go to step 9 (end). If "in macro" flag is on add the line to content of the matcing macro in the table and delete line from source file, else go to step 1.
 * 7. Is the tag mcroend? If yes, erase the tag from the source file and the content. Otherwise → go to step 6.
 * 8. Turn off the "in macro" flag. Go back to step 1.
 * 9. End: save the macro file.
 * @param table - initialized McroTable
 * @param base_name - .as file name
 * @return SUCCESS or FAILURE of preassembler process */
int run_preassembler(McroTable *table, const char *base_name);

#endif