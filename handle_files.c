#include "handle_files.h"

/* function that opens file for reading */
int open_file_for_reading(char *file_name, FILE *file)
{
    file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", file_name);
        return FAILURE;
    }
    return SUCCESS;
}

/* function that opens file for writing */
int open_file_for_writing(char *file_name, FILE *file)
{
    file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", file_name);
        /** fclose(input);
         * TODO - don't forget adding condition in run_pre_assmbler() to close input
         */
        return FAILURE;
    }
    return SUCCESS;
}