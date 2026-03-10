#include <ctype.h>
#include "preassembler.h"
#include "mcro.h"
#include "defs.h"
#include "handle_files.h"
#include "parser.h"
#include "errors.h"
#include "preassembler_parser.h"

#define MAX_LINE_LEN 80

/* helpers */

/* Helper: Skip leading whitespace */
static const char *skip_spaces_internal(const char *p) {
	while (*p && (*p == ' ' || *p == '\t')) {
		p++;
	}
	return p;
}

int run_preassembler(McroTable *table, char *as_filename)
{

}
