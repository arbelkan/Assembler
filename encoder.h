#ifndef ENCODER_H
#define ENCODER_H

#include "word.h"
#include "ops.h"


/* Encodes the first instruction word (12-bit value + ARE='A').
 * Bit layout (per project):
 *  bits 11-8 : opcode
 *  bits  7-4 : funct
 *  bits  3-2 : src addressing (0..3)
 *  bits  1-0 : dst addressing (0..3)
 */
Word encode_first_word(const OpInfo *op, AddrMode src_mode, AddrMode dst_mode);
Word encode_immediate(long value);
Word encode_register(int reg_num);
Word encode_placeholder(void); /* TODO: add fixups */

#endif
