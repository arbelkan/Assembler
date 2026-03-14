#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H

#define MAX_LINE 80
#define LINE_BUF_SIZE (MAX_LINE + 2)
#define MAX_PATH 1024

int run_preassembler(McroTable *table, const char *base_name);

#endif