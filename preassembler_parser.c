#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preassembler_parser.h"
#include "ops.h"
#include "errors.h"

#define SUCCESS 0
#define FAILURE 1

/* internal helpers */

static const char *skip_spaces_internal(const char *p);
static char* extract_mcro_name(const char *line);
static int validate_mcro_name(const char *line, int line_number);

/* public */

/* Find mcro decleration in line */
int find_mcro_line(const char *line) {
    const char *ptr = line;
    while (!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r')) {
        if (strncmp(ptr, "mcro", 4) == 0) {
            return SUCCESS;
        }
        ptr += 1;
    }
    return FAILURE;
}

/* Find mcroend decleration in line */
int find_mcroend_line(const char *line) {
    const char *ptr = line;
    while (!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r')) {
        if (strncmp(ptr, "mcroend", 7) == 0) {
            return SUCCESS;
        }
        ptr += 1;
    }
    return FAILURE;
}

/* Validate "mcro" start line format */
int validate_start_mcro(const char *line, int line_number) {
    const char *ptr = line;

    ptr = skip_spaces_internal(ptr);/* Skip leading whitespace */

    /* Check for "mcro" keyword */
    if (strncmp(ptr, "mcro", 4) != 0) {
        return FAILURE;
    }
    ptr += 4;

    /* Must have whitespace after "mcro" */
    if (!(*ptr == ' ' || *ptr == '\t'))
    {
        printError(DECLERATION_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
        return FAILURE;
    }
    ptr = skip_spaces_internal(ptr);/* Skip whitespace after "mcro" */

    /* Validate macro name */
    if (validate_mcro_name(ptr, line_number) == FAILURE) {
        return FAILURE;
    }
    ptr = skip_spaces_internal(ptr);/* Only whitespace/ newline should follow macro name */
    if(!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r'))
    {
        printError(DECLERATION_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
        return FAILURE;
    }
    return SUCCESS;
}

/* Validate "endmcro" line format */
int validate_end_mcro(const char *line, int line_number) {
    const char *ptr = skip_spaces_internal(line);

    /* Check for "mcroend" */
    if (strncmp(ptr, "mcroend", 7) != 0) {
        return FAILURE;
    }
    ptr += 7;
    ptr = skip_spaces_internal(ptr);/* Only whitespace/ newline should follow */
    if(!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r'))
    {
        printError(END_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
        return FAILURE;
    }
    return SUCCESS;
}

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
    char *mcro_name = "";
    int i = 0;

    ptr = skip_spaces_internal(ptr); /* Skip whitespace */

    /* Extract macro name */
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r')
    {
        mcro_name[i++] = *ptr++;
    }
    mcro_name[i] = '\0';
    return mcro_name;
}

/* Helper: function that validate macro name is not opcode name */
static int validate_mcro_name(const char *line, int line_number) {
    char *mcro_name = extract_mcro_name(line);
    if ((*mcro_name == '\0') || (*mcro_name == '\n') || (*mcro_name == '\r'))
    {
        printError(MACRO_NAME_IS_MISSING,line_number);
        return FAILURE;
    }
    if (op_find(mcro_name) != NULL)
    {
        printError(MACRO_NAME_IS_AN_INSTRUCTION,line_number);
        return FAILURE;
    }
    return SUCCESS;
}