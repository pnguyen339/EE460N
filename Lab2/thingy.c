#include "lc3bsim2.c"		// obviously get rid of this later

/********************REMOVE THIS**********************/
System_Latches CURRENT_LATCHES, NEXT_LATCHES;
int MEMORY[WORDS_IN_MEM][2];
/********************REMOVE THIS**********************/
	//typedef struct System_Latches_Struct {

	//	int PC,                /* program counter */
	//		N,                   /* n condition bit */
	//		Z,                   /* z condition bit */
	//		P;                   /* p condition bit */
	//	int REGS[LC_3b_REGS];  /* register file. */
	//} System_Latches;

/********************* DEFINES ***********************/

/* Useful constants */
#define NUM_INSTR	16
#define WORD		16

/* Opcode */
#define DECODE(x)	((x & 0xF000u) >> 12)

/* Operands within instruction */
#define OP1(x)		((x & 0x0E00u) >> 9)
#define OP2(x)		((x & 0x01C0u) >> 6)
#define OP3(x)		 (x & 0x0007u)

/* Immediate values at end of instruction */
#define CONST_11(x)	(x & 0x07FFu)
#define CONST_9(x)	(x & 0x01FFu)
#define CONST_5(x)	(x & 0x001Fu)
#define CONST_4(x)	(x & 0x000Fu)

/* Bits used for steering or otherwise important values */
#define SIGN_BIT(x)	((x & 0x8000u) >> 15)

#define BIT_11(x)	((x & 0x0800u) >> 11)
#define BIT_10(x)	((x & 0x0400u) >> 10)
#define BIT_9(x)	((x & 0x0200u) >> 9)
#define BIT_5(x)	((x & 0x0020u) >> 5)
#define BIT_4(x)	((x & 0x0010u) >> 4)

/******************* END DEFINES *********************/


int sext32(int input, int num_bits) {
	int sign = (1 << (num_bits - 1)) & input;
	
	input |= -1 * sign;			/* bit manipulation! Is this snazzy or what? */

	return input;
}

void setConditionCodes(System_Latches* latches, int value) {
	latches->N = 0;
	latches->Z = 0;
	latches->P = 0;

	if (value == 0) {
		latches->Z = 1;
	}
	else if (SIGN_BIT(value)) {
		latches->N = 1;
	}
	else {
		latches->P = 1;
	}
}

System_Latches op_br(int instr) {
	System_Latches next = CURRENT_LATCHES;
	
	if (	(next.N && BIT_11(instr)) ||
			(next.Z && BIT_10(instr)) ||
			(next.P && BIT_9(instr))	)
	{
		next.PC = Low16bits(next.PC + (sext32(CONST_9(instr), 9)) << 1);
	}

	return next;
}

System_Latches op_add(int instr) {
	System_Latches next = CURRENT_LATCHES;
	int result;

	if (BIT_5(instr)) {				/* immediate value */
		result = next.REGS[OP2(instr)] + sext32(CONST_5(instr), 5);
	}
	else {							/* register */
		result = next.REGS[OP2(instr)] + next.REGS[OP3(instr)];
	}

	result = Low16bits(result);
	next.REGS[OP1(instr)] = result;

	setConditionCodes(&next, result);

	return next;
}

System_Latches op_ldb(int instr) {

}

System_Latches op_stb(int instr) {

}

System_Latches op_jsr(int instr) {
	System_Latches next = CURRENT_LATCHES;

	if (BIT_11(instr)) {			/* immediate value */
		next.PC = Low16bits(next.PC + (sext32(CONST_11(instr), 11) << 1));
	}
	else {							/* register */
		next.PC = next.REGS[OP2(instr)];
	}

	return next;
}

System_Latches op_and(int instr) {
	System_Latches next = CURRENT_LATCHES;
	int result;

	if (BIT_5(instr)) {				/* immediate value */
		result = next.REGS[OP2(instr)] & sext32(CONST_5(instr), 5);
	}
	else {							/* register */
		result = next.REGS[OP2(instr)] & next.REGS[OP3(instr)];
	}

	result = Low16bits(result);
	next.REGS[OP1(instr)] = result;

	setConditionCodes(&next, result);

	return next;
}

System_Latches op_ldw(int instr) {

}

System_Latches op_stw(int instr) {

}

System_Latches op_rti(int instr) {	/* not required */

}

System_Latches op_xor(int instr) {
	System_Latches next = CURRENT_LATCHES;
	int result;

	if (BIT_5(instr)) {				/* immediate value */
		result = next.REGS[OP2(instr)] ^ sext32(CONST_5(instr), 5);
	}
	else {							/* register */
		result = next.REGS[OP2(instr)] ^ next.REGS[OP3(instr)];
	}

	result = Low16bits(result);
	next.REGS[OP1(instr)] = result;

	setConditionCodes(&next, result);

	return next;
}

System_Latches op_invalid(int instr) { /* I have no idea what goes in here */

}

System_Latches op_jmp(int instr) {
	System_Latches next = CURRENT_LATCHES;

	next.PC = next.REGS[OP2(instr)];
}

System_Latches op_shf(int instr) {
	System_Latches next = CURRENT_LATCHES;
	int result;

	if (BIT_4(instr)) {				/* right shift */
		if (BIT_5(instr)) {			/* arithmetic */
			result = sext32(next.REGS[OP2(instr)], WORD) >> CONST_4(instr);
		}
		else {						/* logical */
			result = next.REGS[OP2(instr)] >> CONST_4(instr);
		}
	}
	else {							/* left shift: */
		result = next.REGS[OP2(instr)] << CONST_4(instr);
	}

	result = Low16bits(result);
	next.REGS[OP1(instr)] = result;

	setConditionCodes(&next, result);

	return next;
}

System_Latches op_lea(int instr) {

}

System_Latches op_trap(int instr) {

}

System_Latches (*execute[NUM_INSTR])(int) = {	op_br , op_add, op_ldb, op_stb,
												op_jsr, op_and, op_ldw, op_stw,
												op_rti, op_xor, op_invalid, op_invalid,
												op_jmp, op_shf, op_lea, op_trap };

void process_instruction() {
	int x = fetch(&x);

	NEXT_LATCHES = (*execute[DECODE(x)])(x);
}
