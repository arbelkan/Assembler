#ifndef LINE_READER_H
#define LINE_READER_H

#include <stdio.h>
#include "asm_state.h"

/* limit for line length according the project rules */
#define LINE_MAX 80
/* buffer size for reading a line: 80 chars + '\0' + (optional spare) */
#define LINE_BUF_SIZE (LINE_MAX + 2)

/* status that read_line will return */
#define LR_OK   SUCCESS  /* a line was read successfully */
#define LR_FAIL FAILURE  /* error (invalid args, etc.) */
#define LR_EOF  2        /* reached end-of-file before reading a line */


/* self reminder what read_line do:
 *
 * Reads one line from fp into out (without '\n').
 * out_size must be >= 2 (LATER: probably i will define it to be LINE_BUF_SIZE).
 *
 * - Increments *line_no by 1 for each physical line read.
 *
 * - Sets *too_long to 1 if the line had more than LINE_MAX characters.
 *   In that case, out will contain the first (out_size-1) chars (typically 80),
 *   and the rest of the line is consumed/discarded until '\n' or EOF.
 *
 * Returns (status):
 *   LR_OK  (0) if a line was read,
 *   LR_EOF (2) if EOF reached before reading any character,
 *   LR_FAIL(1) on invalid arguments.
 */
int read_line(FILE *fp, char *out, int out_size, int *line_no, int *too_long);

#endif
