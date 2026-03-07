#ifndef PASS1_DIRECTIVES_H
#define PASS1_DIRECTIVES_H

#include "asm_state.h"
#include "parser.h"

/* this module handle with a parsed directive line during pass1 */
int pass1_handle_directive(AsmState *st, const ParsedLine *pl, int line_no);

#endif
