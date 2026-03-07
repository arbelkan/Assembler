#ifndef PASS1_H
#define PASS1_H

#include "asm_state.h"

/* Returns SUCCESS (0) on success, FAILURE (1) on failure */
int pass1_run(AsmState *st, const char *am_filename);

#endif
