#ifndef PASS1_INSTRUCTIONS_H
#define PASS1_INSTRUCTIONS_H

#include "asm_state.h"
#include "parser.h"
#include "defs.h"

/* Handle a parsed instruction line during pass1:
 * - validate op exists
 * - validate operand count
 * - parse operands
 * - validate addressing legality using ops masks
 * - add CODE label if exists
 * - advance IC by L (1..3)
 */
int pass1_handle_instruction(AsmState *st, const ParsedLine *pl, int line_no);

#endif
