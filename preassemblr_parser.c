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
static char* extract_mcro_name(const char *line) {
    const char *ptr = line + 4;  /* Skip "mcro" */
    char *macro_name = "";
    int i = 0;

    ptr = skip_spaces_internal(ptr); /* Skip whitespace */

    /* Extract macro name */
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && i < max_len - 1) {
        macro_name[i++] = *ptr++;
    }
    macro_name[i] = '\0';
    return macro_name;
}

/* public */

/* Helper: function that validate macro name is not opcode name */
static int validate_mcro_name(const char *line, int line_number)
{
    char *mcro_name = extract_mcro_name(line);
    if (strcmp(macro_name, '\0') == 0)
    {
        printf(printErrors(MACRO_NAME_IS_MISSING,line_number));
        return FAILURE;
    }
    if (op_find(mcro_name) != NULL)
    {
        printf(printErrors(MACRO_NAME_IS_AN_INSTRUCTION,line_number));
        return FAILURE;
    }
    return SUCCESS;
}

/* Validate "mcro" start line format */
int validate_start_mcro(const char *line, line_number) {
    const char *ptr = line;
    char *macro_name = extract_mcro_name(line);

    /* Skip leading whitespace */
    ptr = skip_spaces_internal(ptr);

    /* Check for "mcro" keyword */
    if (strncmp(ptr, "mcro", 4) != 0) {
        return FAILURE;
    }

    ptr += 4;

    /* Must have whitespace after "mcro" */
    if (!(*ptr == ' ' || *ptr == '\t')) {
        return FAILURE;
    }

    /* Skip whitespace after "mcro" */
    ptr = skip_spaces_internal(ptr);

    /* Validate macro name */
    if (validate_mcro_name(ptr, line_number) == FAILURE) {
        return FAILURE;
    }

    /* Only whitespace/newline should follow macro name */
    ptr = skip_spaces_internal(ptr);

    return (*ptr == '\0' || *ptr == '\n' || *ptr == '\r') ? SUCCESS : FAILURE;
}

/* Validate "endmcro" line format */
int validate_end_mcro(const char *line) {
    const char *ptr = skip_spaces_internal(line);

    /* Check for "endmcro" */
    if (strncmp(ptr, "endmcro", 7) != 0) {
        return FAILURE;
    }

    ptr += 7;

    /* Only whitespace/newline should follow */
    ptr = skip_spaces_internal(ptr);

    return (*ptr == '\0' || *ptr == '\n' || *ptr == '\r') ? SUCCESS : FAILURE;
}