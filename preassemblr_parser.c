#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preassembler_parser.h"
#include "parser.h"
#include "ops.h"
#include "errors.h"

#define SUCCESS 0
#define FAILURE 1

/* internal helpers */

/* Helper: Skip leading whitespace */
static const char *skip_spaces_internal(const char *p) {
    while (*p && (*p == ' ' || *p == '\t')) {
        p++;
    }
    return p;
}

/* Helper: Extract macro name from line starting with "mcro" */
static int extract_mcro_name(const char *line) {
    const char *ptr = line + 4;  /* Skip "mcro" */
    char *macro_name = "";
    int i = 0;

    ptr = skip_spaces_internal(ptr); /* Skip whitespace */

    /* Extract macro name */
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && i < max_len - 1) {
        macro_name[i++] = *ptr++;
    }
    macro_name[i] = '\0';
    return (i > 0) ? SUCCESS : FAILURE;
}

/* public */

/* function that validate macro name */
int validate_mcro_name(const char *line, int line_number)
{
    char *mcro_name = extract_mcro_name(line);
    if (op_find(mcro_name) != NULL)
    {
        printf(printErrors(MACRO_NAME_IS_AN_INSTRUCTION,line_number));
        return FAILURE;
    }
    return SUCCESS;
}