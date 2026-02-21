#ifndef MACRO_H
#define MACRO_H

#define MAX_NAME  64
#define MAX_BODY 1024

typedef struct {
    char name[MAX_NAME];
    char body[MAX_BODY];
} Macro;

McroStatus parse_macro(FILE *src, char *first_line, Macro *out_macro);

#endif
