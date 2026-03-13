#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H

#define MAX_LINE 82  /* 80 chars + newline + null terminator */
#define TRUE 1
#define FALSE 0

int run_preassembler(McroTable *table, char *as_filename);
#endif