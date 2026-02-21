typedef struct
{
    unsigned int **adrress; /*pointer to an adress pointer*/
    char symbol;
    unsigned int mode; /*addressing mode in code 0-3*/
    unsigned int line; /*witch line in file needs to be replaced*/
} Fixup;
