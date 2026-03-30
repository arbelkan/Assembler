#ifndef DATA_IMAGE_H
#define DATA_IMAGE_H

#include "defs.h"
#include "word.h"

#define MAX_DATA_WORDS MAX_TOTAL_WORDS


typedef struct {
	/*unsigned int data[MAX_DATA_WORDS]; */	/* store 12 bit values */
	Word words[MAX_DATA_WORDS];
	int count; 	/* number of data stored*/
} DataImage;


/* initialize an empty DataImage */
void data_image_init(DataImage *di);


/* Emit one 12-bit value into the data image (appends at end).
 * Returns SUCCESS/FAILURE.
 */
int data_image_emit(DataImage *di, unsigned int value);


/* Debug helper (temporary): print data image in binary */
void data_image_dump(const DataImage *di, int icf);


#endif
