#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pass1_instructions.h"
#include "ops.h"
#include "operand.h"
#include "symbols.h"
#include "encoder.h"
#include "code_image.h"
#include "fixups.h"

static const char *skip_spaces_local(const char *p);
static int parse_operands(const char *args, unsigned int expected_ops, Operand *src, Operand *dst, int line_no);
static int validate_addressing(const OpInfo *op, const Operand *src, const Operand *dst, int line_no);


int pass1_handle_instruction(AsmState *st, const ParsedLine *pl, int line_no) {
	const OpInfo *op;
	Operand src;
	Operand dst;
	int l_words;
	AddrMode src_mode;
	AddrMode dst_mode;
	Word first;
	Word extra;
	int next_addr;

	if (st == NULL || pl == NULL) return FAILURE;

	op = op_find(pl->op_name);

	if (op == NULL) {
		printf("Error (line %d): unknown instruction: %s\n", line_no, pl->op_name);
		return FAILURE;
	}

	/* parse operands according to expected operands count */
	if (parse_operands(pl->args, op->operands, &src, &dst, line_no) != SUCCESS) {
		return FAILURE;
	}

	/* validate addressing modes using op masks */
	if (validate_addressing(op, &src, &dst, line_no) != SUCCESS) {
		return FAILURE;
	}

	/* add label as CODE symbol (value = current IC) */
	if (pl->has_label) {
		if (symbols_add_code(&st->symbols, pl->label, st->IC, line_no) != SUCCESS) {
			/* symbols_add_code already printed an error */
		}
	}

	/* determine src/dst addressing modes for first word */
	src_mode = (AddrMode)0; /* default 0 */
	dst_mode = (AddrMode)0; /* default 0 */

	if (op->operands == 0u) {
		src_mode = (AddrMode)0;
		dst_mode = (AddrMode)0;
	}
	else if (op->operands == 1u) {
		src_mode = (AddrMode)0;
		dst_mode = dst.mode;
	}
	else { /* op->operands == 2u */
		src_mode = src.mode;
		dst_mode = dst.mode;
	}

	first = encode_first_word(op, src_mode, dst_mode);
	
	/* emit first word at current IC address */
	if (code_image_emit(&st->code, st->IC, first) != SUCCESS) {
		printf("Error (line %d): failed to emit code word at address: %d\n", line_no, st->IC);
		return FAILURE;
	}

	/* emit extra words (one per operand) src and then dst */
	next_addr = st->IC + 1;
	
	if (op->operands == 2u) {
		/* source operand word */
		if (src.mode == ADDR_IMMEDIATE) {
			extra = encode_immediate(src.imm_value);
		}
		else if (src.mode == ADDR_REGISTER) {
			extra = encode_register(src.reg_num);
		}
		else {
			fixups_add(&st->fixups, next_addr, src.mode, src.label, line_no);
			extra = encode_placeholder();
		}

		if (code_image_emit(&st->code, next_addr, extra) != SUCCESS) {
			printf("Error (line %d): failed to emit operand word at address: %d\n", line_no, next_addr);
			return FAILURE;
		}

		next_addr++;
	}

	if (op->operands >= 1u) {
		/* destination operand word */
		if (dst.mode == ADDR_IMMEDIATE) {
			extra = encode_immediate(dst.imm_value);
		}
		else if (dst.mode == ADDR_REGISTER) {
			extra = encode_register(dst.reg_num);
		}
		else {
			fixups_add(&st->fixups, next_addr, dst.mode, dst.label, line_no);
			extra = encode_placeholder();
		}

		if (code_image_emit(&st->code, next_addr, extra) != SUCCESS) {
			printf("Error (line %d): failed to emit operand word at address: %d\n", line_no, next_addr);
			return FAILURE;
		}

		next_addr++;
	}

	/* advance IC by instruction length (1+operands) */
	l_words = 1 + (int)op->operands;
	st->IC += l_words;
	
	/*temporary debug, TODO: delete after testing*/
	printf("Debug: instr line %d encoded %d word(s), IC=%d\n", line_no, l_words, st->IC - l_words);	

	return SUCCESS;
}




/* internal helpers */


static const char *skip_spaces_local(const char *p) {
	while (p != NULL && *p != '\0' && isspace((unsigned char)*p)) {
		p++;
	}
	return p;
}


/* Parse operands from args string.
   expected_ops:
     0 -> args must be empty (spaces allowed)
     1 -> one operand (dst)
     2 -> two operands: src , dst
*/
static int parse_operands(const char *args, unsigned int expected_ops, Operand *src, Operand *dst, int line_no) {
	const char *p;
	char tok1[SYMBOL_NAME_MAX + 3];
	char tok2[SYMBOL_NAME_MAX + 3];
	int i;

	if (src == NULL || dst == NULL) return FAILURE;

	/* initial tokens */
	tok1[0] = '\0';
	tok2[0] = '\0';

	p = (args == NULL) ? "" : args;
	p = skip_spaces_local(p);

	if (expected_ops == 0u) {
		if (*p != '\0') {
			printf("Error (line %d): this instruction takes no operands", line_no);
			return FAILURE;
		}
		return SUCCESS;
	}

	/* read first operand token untill comma or end */
	i = 0;
	while (*p != '\0' && *p != ',' && !isspace((unsigned char)*p)) {
		if (i < ((int)sizeof(tok1) - 1)) tok1[i++] = *p;
		p++;
	}
	tok1[i] = '\0';
	
	/* allow spaces after token */
	p = skip_spaces_local(p);
	
	if (expected_ops == 1u) {
		if (tok1[0] == '\0') {
			printf("Error (line %d): missing operand\n", line_no);
			return FAILURE;
		}
		if (*p != '\0') {
			printf("Error (line %d): extra characters after operand\n", line_no);
			return FAILURE;
		}
		/* for instruction with 1 operand, the operand is dst */
		if (operand_parse(tok1, dst, line_no) != SUCCESS) return FAILURE;
		return SUCCESS;
	}

	/* expected_ops == 2 */

	if (tok1[0] == '\0') {
		printf("Error (line %d): missing source operand\n", line_no);
		return FAILURE;
	}

	if (*p != ',') {
		printf("Error (line %d): missing ',' between operands\n", line_no);
		return FAILURE;
	}
	
	p++; /*consume comma*/
	p = skip_spaces_local(p);

	/* read second operand token untill comma or end */
	i = 0;
	while (*p != '\0' && *p != ',' && !isspace((unsigned char)*p)) {
		if (i < ((int)sizeof(tok2) - 1)) tok2[i++] = *p;
		p++;
	}
	tok2[i] = '\0';
	
	p = skip_spaces_local(p);
	if (*p != '\0') {
		printf("Error (line %d): extra characters after second operand\n", line_no);
		return FAILURE;
	}
	if (tok2[0] == '\0') {
		printf("Error (line %d): missing destination operand\n", line_no);
		return FAILURE;
	}

	if (operand_parse(tok1, src, line_no) != SUCCESS) return FAILURE;
	if (operand_parse(tok2, dst, line_no) != SUCCESS) return FAILURE;

	return SUCCESS;
}



static int validate_addressing(const OpInfo *op, const Operand *src, const Operand *dst, int line_no) {
	unsigned int m;

	if (op == NULL) return FAILURE;

	if (op->operands == 0u) return SUCCESS;

	if (op->operands == 1u) {
		m = 1u << (unsigned int)dst->mode;
		if ((op->dst_mask & m) == 0u) {
			printf("Error (line %d): illegal addressing mode for destination\n", line_no);
			return FAILURE;
		}
		return SUCCESS;
	}

	/* op->operands == 2 */
	m = 1u << (unsigned int)src->mode;
	if ((op->src_mask & m) == 0u) {
		printf("Error (line %d): illegal addressing mode for source\n", line_no);
		return FAILURE;
	}
	m = 1u << (unsigned int)dst->mode;
	if ((op->dst_mask & m) == 0u) {
		printf("Error (line %d): illegal addressing mode for destination\n", line_no);
		return FAILURE;
	}

	return SUCCESS;
}

























