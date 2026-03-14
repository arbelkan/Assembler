#ifndef HANDLE_FILES_H
#define HANDLE_FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

 /**
  * function that opens file for reading
  * @param file_name - the name of the file
  * @param file - the needed file
  * @return SUCCESS or FAILURE
  **/
int open_file_for_reading(char *file_name, FILE **file);

/**
  * function that opens file for writing
  * @param file_name - the name of the file
  * @param file - the needed file
  * @return SUCCESS or FAILURE
  **/
int open_file_for_writing(char *file_name, FILE **file);

/**
 * build a filename from a base name and extension
 * @param out - output buffer
 * @param out_size - size of output buffer
 * @param base - base filename (no extension)
 * @param ext - extension string
 **/
void make_file_name(char *out, int out_size, const char *base, const char *ext);
#endif