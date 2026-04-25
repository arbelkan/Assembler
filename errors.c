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
    "missing comma",
    "comma not in place",
    "same decleration for both entry and extern symbol",
    "illegal symbol name",
    "symbol name longer than 31 characters",
    "colons not adjacent to symbol",
    "symbol already defines",
    "decleration both for macro and symbol",
    "symbol not defined",
    "symbol is missing",
    "illegal number",
    "illegal argument",
    "unkown instruction",
    "unkown directive",
    "unkown register",
    "too many arguments",
    "not enough arguments",
    "operand not suitable for instruction",
    "lable before extern is not allowed",
    "lable before entry is not allowed",
    "invalid lable in relative operand",
    "invalid label in direct operand",
    "missing opening quote",
    "missing closing quote"
    "external characters",
    "failed to emit code word at address",
    "failed to emit operand at address",
    "this instruction takes no operands",
    "missing operand",
    "missing source operand",
    "missing destination operand",
    "illegal addressing mode",
    "parse error"
};

/**
* printing error and its line using error code
**/
unsigned int print_error(unsigned int errorCode, unsigned int lineNumber){
    printf("error in line %u: %s\n", lineNumber, errors[errorCode]);
    return errorCode;
}
