#ifndef PASS2_H
#define PASS2_H

#include "defs.h"
#include "asm_state.h"

#define AM_EXT ".am"

/*
 * second pass over the .am file and implements the next algorithm:
 * 1. Read next line from source. If EOF, go to step 7.
 * 2. If first field is a label, skip it.
 * 3. If .data, .string, or .extern, go to step 1.
 * 4. If .entry- add the operand symbol to the symbol table as ENTRY. If symbol not found- error. Go to step 1.
 * 5. Otherwise it's an instruction. For each operand that is a label reference (in the fixup table)-
 * look up in symbol table. If found, complete the binary encoding. If external, write as E ARE
 * and add to external references list. If not found, error. Go to step 1.
 * 6. Read until complete. If errors, stop.
 * 7. Create output files
 *
 * @param st - the AsmState after pass1 proccess
 * @param am_filename - the .am file name
 * @return SUCCESS or FAILURE
 */
int pass2_run(AsmState *st, const char *am_filename);

#endif