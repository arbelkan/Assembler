#ifndef PASS2_H
#define PASS2_H

#include "defs.h"
#include "asm_state.h"

#define AM_EXT ".am"

/*
 * second pass over the .am file and implements the next algorithm:
 * 1. Read next line from source. If EOF, go to step 6.
 * 2. If first field is a label, skip it.
 * 3. If .data, .string, .extern, or instruction - go to step 1.
 * 4. If .entry - look up the operand symbol in the symbol table.
 *    If symbol is also declared .extern - error.
 *    If symbol not found - error.
 *    Otherwise, mark symbol as ENTRY. Go to step 1.
 * 5. After all lines read: for each entry in the fixup table,
 *    look up the label in the symbol table.
 *    If not found - error.
 *    If external - patch word with value=0, ARE='E'. Record for .ext output.
 *    If direct addressing - patch word with symbol address, ARE='R'.
 *    If relative addressing - patch word with distance from instruction, ARE='A'.
 * 6. If errors occurred, stop.
 * 7. Create output files (.ob, .ent, .ext).
 *
 * @param st - the AsmState after pass1 proccess
 * @param am_filename - the .am file name
 * @return SUCCESS or FAILURE
 */
int pass2_run(AsmState *st, const char *am_filename);

#endif