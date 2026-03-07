#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errorCodes.h"
#include "mcro.h"

FILE main(int argc, char *argv[])
{
    unsigned int lineCnt = 1;/*count lines in file*/
    McroTable table = initMcroTable();/*creating macros table*/
    /* for now untill I get all the opcodes table or array */
    const char* instructions[] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "bne",
        "prn",
        "rts",
        "stop"
    };

}