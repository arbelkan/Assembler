#include <ctype.h>
#include "pre_assembler.h"
#include "mcro.h"
#include "defs.h"
#include "handle_files.h"
#include "parser.h"

/* helpers */

/* TODO- delete after moving to assembler.c and doing adjusments */
static void make_filename(char *out, int out_size, const char *base, const char *ext) {
    int base_len, ext_len;

    if (out_size <= 0) return;

    base_len = (int)strlen(base);
    ext_len = (int)strlen(ext);

    if ((base_len + ext_len) >= out_size) {
        base_len = out_size - ext_len - 1;
        if (base_len < 0) base_len = 0;
    }

    if (base_len > 0) strncpy(out, base, (size_t)base_len);

    out[base_len] = '\0';
    strncat(out, ext, (size_t)(out_size - base_len - 1));
}



int run_preassembler(McroTable *table, char *as_filename)
{

}
