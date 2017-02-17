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

#define NUM_INSTR	16
#define DECODE(x)	((x & 0xF000u) >> 12)

#define OP1(x)		((x & 0x0E00u) >> 9)
#define OP2(x)		((x & 0x01C0u) >> 6)
#define OP3(x)		((x & 0x0007u) >> 0)

#define CONST_11(x)	((x & 0x07FFu) >> 0)
#define CONST_9(x)	((x & 0x01FFu) >> 0)
#define CONST_5(x)	((x & 0x001Fu) >> 0)
//#define CONST_4(x)	((x & 0x000Fu) >> 0)	// don't actually need -- can interpret sign bit of CONST_5 for SHF, as amount is unsigned
//(DO NOT LEFT SHIFT A NEGATIVE AMOUNT)

#define BIT_11(x)	((x & 0x0800u) >> 11)
#define BIT_10(x)	((x & 0x0400u) >> 10)
#define BIT_9(x)	((x & 0x0200u) >> 9)
#define BIT_5(x)	((x & 0x0020u) >> 5)

int sext(int input, int num_bits) {

}

System_Latches op_br(int instr) {
	System_Latches next = CURRENT_LATCHES;
	
	if (	(next.N && BIT_11(instr)) ||
			(next.Z && BIT_10(instr)) ||
			(next.P && BIT_9(instr))	)
	{
		next.PC = Low16bits(next.PC + sext(CONST_9(instr), 9));
	}

	return next;
}

System_Latches op_add(int instr) {
	System_Latches next = CURRENT_LATCHES;

	if (BIT_5(instr)) {				// immediate value
		next.REGS[OP1(instr)] = Low16bits(next.REGS[OP2(instr)] + sext(CONST_5(instr), 5));
	}
	else {							// register
		next.REGS[OP1(instr)] = Low16bits(next.REGS[OP2(instr)] + next.REGS[OP3(instr)]);
	}
}

System_Latches op_ldb(int instr) {

}

System_Latches op_stb(int instr) {

}

System_Latches op_jsr(int instr) {

}

System_Latches op_and(int instr) {

}

System_Latches op_ldw(int instr) {

}

System_Latches op_stw(int instr) {

}

System_Latches op_rti(int instr) {

}

System_Latches op_xor(int instr) {

}

System_Latches op_invalid(int instr) {

}

System_Latches op_jmp(int instr) {

}

System_Latches op_shf(int instr) {

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
