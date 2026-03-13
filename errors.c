#include <stdio.h>
#include "errors.h"

static char* errors[ERR_MAX] = {
    NULL,
    "macro name is an instruction",
    "decleration of macro line includes external characters",
    "end of macro line includes external characters",
    "macro name is missing",
    "illegal macro name",
    "failure in adding macro to table",
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

/**
* printing error and its line using error code
**/
unsigned int printError(unsigned int errorCode, unsigned int lineNumber)
{
    printf("error in line %u: %s\n", lineNumber, errors[errorCode]);
    return errorCode;
}
