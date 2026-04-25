#ifndef PREASSEMBLER_PARSER_H
#define PREASSEMBLER_PARSER_H

#define SUCCESS 0
#define FAILURE 1
#define MAX_MCRO_LEN 31 /* same length limitation as label */

/**
 * check if line includes mcroend declaration
 * @param line - selected line
 * @return SUCCESS if includes, else FAILURE
 **/
int find_mcro(const char *line);

/**
 * check if line includes mcroend declaration
 * @param line - selected line
 * @return SUCCESS if includes, else FAILURE
 **/
int find_mcroend(const char *line);

/**
 * extracts macro name from declaration line
 * @param line - mcro declaration line
 * @return mcro name
 **/
char* extract_mcro_name(const char *line);

/**
 * validates mcro declaration line by format
 * @param line - mcro declaration line
 * @param line_number - the number of the declaration line
 * @return SUCCESS if valid, else FAILURE
 **/
int validate_start_mcro(const char *line, int line_number);

/**
 * validates mcro ending line by format
 * @param line - mcro ending line
 * @param line_number - the number of the ending line
 * @return SUCCESS if valid, else FAILURE
 **/
int validate_end_mcro(const char *line, int line_number);

#endif