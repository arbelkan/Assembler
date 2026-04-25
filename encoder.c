#include <stdio.h>
#include "encoder.h"

Word encode_first_word(const OpInfo *op, AddrMode src_mode, AddrMode dst_mode) {
	Word w;
	unsigned int v;

	/* default word */
	w.value = 0u;
	w.are = 'A';

	if (op == NULL) {
		return w;
	}

	/* pack field into 12-bit word */
	v = 0u;
	v |= ((op->opcode & 0x0Fu) << 8);
	v |= ((op->funct  & 0x0Fu) << 4);
	v |= (((unsigned int)src_mode & 0x03u) << 2);
	v |= ((unsigned int)dst_mode & 0x03u);

	w.value = (v & 0x0FFFu);
	return w;
}



Word encode_immediate(long value) {
	Word w;
	unsigned int v;

	w.are = 'A';

	/* keep only 12-bits */
	v= (unsigned int)value;
	w.value = (v & 0x0FFFu);

	return w;
}



Word encode_register(int reg_num) {
	Word w;
	unsigned int v;

	w.are = 'A';
	
	/* one-hot encoding: bit reg_num set */
	if (reg_num < 0 || reg_num > 7) { 
		/* should not happen because 0-7 been check in operand_parse */
		w.value = 0u;
		return w;
	}

	v = (1u << (unsigned int)reg_num);
	w.value = (v & 0x0FFFu);

	return w;
}



Word encode_placeholder(void) {
	Word w;
	w.are = 'A';
	w.value = 0u;
	return w;
}
























