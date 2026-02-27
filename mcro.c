#include <stdio.h>
#include <stdlib.h>
#include "errors.c"
#include "mcro.h"

//findig a macro by searching it name in the macros table
int findMcro(char *name, McroTable *t)
{
    int i;
    for (i = 0; t->size >= 0; i++)
    {
        if (t[i]->name == name)
            return i;//macro name was found
    }
    return -1;//macro name wasn't fount
}

//adding a new macro to the macros table
int addMcro(char *mcro, MacroTable *t)
{
    t-> size++;
    t->table = realloc(t->table, t->size * sizeof(Mcro));
    t->table[t->size-1]->name = mcro;
    t->table[t->size-1]->body = strlen(mcro);
}
char *getMcro(char *line, FILE *output);