#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mcro.h"
#include "errors.h"
#include "defs.h"

int mcro_parser(char *input, McroTable *table, FILE *output);