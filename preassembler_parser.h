#ifndef PREASSEMBLER_PARSER_H
#define PREASSEMBLER_PARSER_H

#define SUCCESS 0
#define FAILURE 1
#define MAX_MCRO_LEN 30 /* from zero to 30 is 31 characters */

/**
 * check if line includes mcroend decleration
 * @param line - selected line
 * @return SUCCESS if includes, else FAILURE
 **/
int find_mcro(const char *line);

/**
 * check if line includes mcroend decleration
 * @param line - selected line
 * @return SUCCESS if includes, else FAILURE
 **/
int find_mcroend(const char *line);

/**
 * extracts macro name from decleration line
 * @param line - mcro decleration line
 * @return mcro name
 **/
char* extract_mcro_name(const char *line);

/**
 * validates mcro decleration line by format
 * @param line - mcro decleration line
 * @param line_number - the number of the decleration line
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