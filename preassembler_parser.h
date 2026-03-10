#ifndef PREASSEMBLER_PARSER_H
#define PREASSEMBLER_PARSER_H

/**
 * validates mcro decleration line by format
 * @param line - mcro decleration line
 * @param line_number - the number of the decleration line
 * @return SUCCESS if valid, else FALIURE
 **/
int validate_start_mcro(const char *line, int line_number);

/**
 * validates mcro ending line by format
 * @param line - mcro ending line
 * @param line_number - the number of the ending line
 * @return SUCCESS if valid, else FALIURE
 **/
int validate_end_mcro(const char *line, int line_number);

#endif