#include "handle_files.h"

/* function that opens file for reading */
int open_file_for_reading(char *file_name, FILE **file)
{
    *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", file_name);
        return FAILURE;
    }
    return SUCCESS;
}

/* function that opens file for writing */
int open_file_for_writing(char *file_name, FILE **file)
{
    *file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", file_name);
        return FAILURE;
    }
    return SUCCESS;
}

/* build a filename from a base name and extension */
void make_file_name(char *out, int out_size, const char *base, const char *ext) {
    int base_len, ext_len;

    if (out_size <= 0)
        return;

    base_len = (int)strlen(base);
    ext_len = (int)strlen(ext);

    if ((base_len + ext_len) >= out_size) {
        base_len = out_size - ext_len - 1;
        if (base_len < 0) base_len = 0;
    }

    if (base_len > 0)
        strncpy(out, base, (size_t)base_len);

    out[base_len] = '\0';
    strncat(out, ext, (size_t)(out_size - base_len - 1));
}
