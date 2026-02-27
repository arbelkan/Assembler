#include <stdio.h>
#include "errors.h"

static char* errors[ERR_MAX] = {
    NULL,
    "macro name is an instruction",
    "end of macro line includes external characters",
    "line longer than 80 characters",
    "external comma",
    "comma not in place",
    "same decleration for both entry and extern symbol",
    "illegal symbol name",
    "symbol name longer than 31 characters",
    "colons not adjacent to symbol",
    "symbol already defines",
    "decleration both for macro and symbol",
    "symbol not defines",
    "illegal number",
    "unkown instruction",
    "unkown register",
    "too many operands",
    "not enough operands",
    "operand not suitable for instruction"
};

unsigned int printError(unsigned int errorCode, unsigned int lineNumber)
{
    printf("error in line %u: %s\n", lineNumber, errors[errorCode]);
    return errorCode;
}
