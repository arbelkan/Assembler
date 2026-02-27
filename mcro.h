#ifndef MACRO_H
#define MACRO_H

#define MAX_NAME  64
#define MAX_BODY 1024

//defining a macro structure
typedef struct {
    char name[MAX_NAME];
    char body[MAX_BODY];
} Mcro;

//defining a macros table structure
typedef struct
{
    int size = 0;
    Mcro *table = NULL;
} McroTable;

int findMcro(char *name, McroTable *t);
int addMcro(char *mcro, McroTable *t);
char *getMcro(char *line, FILE *output);//replacing macro name with macro body in output file

#endif
