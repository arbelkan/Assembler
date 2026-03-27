#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H

#include "asm_state.h"
#include "handle_files.h"
#include "symbols.h"
#include "defs.h"

#define OB_EXT  ".ob"
#define ENT_EXT ".ent"
#define EXT_EXT ".ext"

/**
 * a method that creates all three output files (.ob, .ent, .ext) for the given assembled state
 * @param st - the fully assembled AsmState (after pass2)
 * @return SUCCESS if all required files were written, FAILURE otherwise.
 **/
int create_output_files(AsmState *st);

#endif