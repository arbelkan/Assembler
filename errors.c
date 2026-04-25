#include <stdio.h>
#include "errors.h"

static char* errors[ERR_MAX] = {
    NULL,
    "macro name is an instruction",
    "declaration of macro line includes external characters",
    "end of macro line includes external characters",
    "macro name is missing",
    "illegal macro name",
    "failure in adding macro to table",
    "line longer than 80 characters",
    "external comma",
    "missing comma",
    "comma not in place",
    "same declaration for both entry and extern symbol",
    "illegal symbol name",
    "symbol name longer than 31 characters",
    "colons not adjacent to symbol",
    "symbol already defined",
    "declaration both for macro and symbol",
    "symbol not defined",
    "symbol is missing",
    "illegal number",
    "illegal argument",
    "unknown instruction",
    "unknown directive",
    "unknown register",
    "too many arguments",
    "not enough arguments",
    "operand not suitable for instruction",
    "label before extern is not allowed",
    "label before entry is not allowed",
    "invalid label in relative operand",
    "invalid label in direct operand",
    "missing opening quote",
    "missing closing quote",
    "external characters",
    "failed to emit code word at address",
    "failed to emit operand at address",
    "this instruction takes no operands",
    "missing operand",
    "missing source operand",
    "missing destination operand",
    "illegal addressing mode",
    "parse error",
    "failure in adding fixup to table",
    "program too large after pass1",
    "program too large",
    "invalid arguments for pass1",
    "cannot open file",
    "read_line failed while parsing",
    "CodeImage out of range",
    "DataImage overflow",
};

/**
* printing error and its line using error code
**/
void print_error(unsigned int errorCode, unsigned int lineNumber){
    printf("error in line %u: %s\n", lineNumber, errors[errorCode]);
}

void print_error_no_line(unsigned int errorCode){
    printf("error: %s", errors[errorCode]);
}
