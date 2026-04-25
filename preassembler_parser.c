#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preassembler_parser.h"
#include "ops.h"
#include "errors.h"

/* internal helpers */

static const char *skip_spaces_internal(const char *p);
static int validate_mcro_name(const char *line, int line_number);

/* public */

/* Find mcro declaration in line */
int find_mcro(const char *line) {
    const char *ptr = skip_spaces_internal(line);
    while (!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r')) {
        if (strncmp(ptr, "mcro", 4) == 0) {
            return SUCCESS;
        }
        ptr += 1;
    }
    return FAILURE;
}

/* Find mcroend declaration in line */
int find_mcroend(const char *line) {
    const char *ptr = skip_spaces_internal(line);
    while (!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r')) {
        if (strncmp(ptr, "mcroend", 7) == 0) {
            return SUCCESS;
        }
        ptr += 1;
    }
    return FAILURE;
}

/* Extract macro name from line starting with "mcro" */
char* extract_mcro_name(const char *line) {
    const char *ptr = skip_spaces_internal(line);
    static char mcro_name[MAX_MCRO_LEN + 2];
    int i = 0;

    ptr += 4; /* skip "mcro" */
    ptr = skip_spaces_internal(ptr); /* skip whitespace between "mcro" and name */

    /* Extract macro name */
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r') {
        mcro_name[i++] = *ptr++;
    }
    mcro_name[i] = '\0';
    return mcro_name;
}

/* Validate "mcro" start line format */
int validate_start_mcro(const char *line, int line_number) {
    const char *ptr = skip_spaces_internal(line);
    char *mcro_name = extract_mcro_name(line);

    if (strncmp(ptr, "mcro", 4) != 0) {
        print_error(DECLARATION_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
        return FAILURE;
    }
    ptr += 4;
    ptr = skip_spaces_internal(ptr);

    if (validate_mcro_name(ptr, line_number) == FAILURE) {
        return FAILURE;
    }

    ptr += strlen(mcro_name);
    ptr = skip_spaces_internal(ptr);
    if (!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r')) {
        print_error(DECLARATION_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
        return FAILURE;
    }
    return SUCCESS;
}

/* Validate "endmcro" line format */
int validate_end_mcro(const char *line, int line_number) {
    const char *ptr = skip_spaces_internal(line);

    /* Check for "mcroend" */
    if (strncmp(ptr, "mcroend", 7) != 0) {
        print_error(END_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
        return FAILURE;
    }
    ptr += 7;
    ptr = skip_spaces_internal(ptr);/* Only whitespace/ newline should follow */
    if(!(*ptr == '\0' || *ptr == '\n' || *ptr == '\r')) {
        print_error(END_OF_MACRO_LINE_INCLUDES_EXTERNAL_CHARACTERS, line_number);
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

/* Helper: function that validate macro name */
static int validate_mcro_name(const char *line, int line_number) {
    char mcro_name[MAX_MCRO_LEN + 2];
    int len = 0;
    int i;

    /* ptr already points at the name — extract directly, no +4 needed */
    while (*line && *line != ' ' && *line != '\t' && *line != '\n' && *line != '\r')
        mcro_name[len++] = *line++;
    mcro_name[len] = '\0';

    if (len == 0) {
        print_error(MACRO_NAME_IS_MISSING, line_number);
        return FAILURE;
    }
    if (len > MAX_MCRO_LEN) {
        print_error(ILLEGAL_MACRO_NAME, line_number);
        return FAILURE;
    }
    if (!isalpha((unsigned char)mcro_name[0])) {
        print_error(ILLEGAL_MACRO_NAME, line_number);
        return FAILURE;
    }
    for (i = 1; i < len; i++) {
        if (!isalnum((unsigned char)mcro_name[i]) && mcro_name[i] != '_') {
            print_error(ILLEGAL_MACRO_NAME, line_number);
            return FAILURE;
        }
    }
    if (op_find(mcro_name) != NULL) {
        print_error(MACRO_NAME_IS_AN_INSTRUCTION, line_number);
        return FAILURE;
    }
    return SUCCESS;
}