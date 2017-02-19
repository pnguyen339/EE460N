/*
    Name 1: Eric Wang
    Name 2: Peter Nguyen
    UTEID 1: efw345
    UTEID 2: ppn229
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

/********************* DEFINES ***********************/

/* High and low bytes */
#define Low8bits(x)  (x & 0x00FFu)
#define High8bits(x) ((x & 0xFF00u) >> 8)

/* Useful constants */
#define NUM_INSTR 16
#define WORD    16
#define LR      7

/* Opcode */
#define DECODE(x) ((x & 0xF000u) >> 12)

/* Operands within instruction */
#define OP1(x)    ((x & 0x0E00u) >> 9)
#define OP2(x)    ((x & 0x01C0u) >> 6)
#define OP3(x)     (x & 0x0007u)

/* Immediate values at end of instruction */
#define CONST_11(x) (x & 0x07FFu)
#define CONST_9(x)  (x & 0x01FFu)
#define CONST_8(x)  (x & 0x00FFu)
#define CONST_6(x)  (x & 0x003Fu)
#define CONST_5(x)  (x & 0x001Fu)
#define CONST_4(x)  (x & 0x000Fu)

/* Bits used for steering or otherwise important values */
#define SIGN_BIT(x) ((x & 0x8000u) >> 15)

#define BIT_11(x) ((x & 0x0800u) >> 11)
#define BIT_10(x) ((x & 0x0400u) >> 10)
#define BIT_9(x)  ((x & 0x0200u) >> 9)
#define BIT_5(x)  ((x & 0x0020u) >> 5)
#define BIT_4(x)  ((x & 0x0010u) >> 4)

/******************* END DEFINES *********************/


int sext32(int input, int num_bits) {
  int sign = (1 << (num_bits - 1)) & input;
  
  input |= -1 * sign;     /* bit manipulation! Is this snazzy or what? */

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

int getWordAt(int byteaddr) {
  int result = 0;

  if (byteaddr % 2 == 1) {
    /* illegal operand address exception */
  }

  result |= MEMORY[byteaddr][1] << 8;   /* 8 bits in a byte */
  result |= MEMORY[byteaddr][0];

  return result;
}

int getByteAt(int byteaddr) {
  int result = 0;

  result |= MEMORY[byteaddr >> 1][byteaddr % 2];

  return result;
}

void setWordAt(int byteaddr, int value) {

  if (byteaddr % 2 == 1) {
    /* illegal operand address exception */
  }

  MEMORY[byteaddr][1] = High8bits(value);
  MEMORY[byteaddr][0] = Low8bits(value);

}

void setByteAt(int byteaddr, int value) {
  
  MEMORY[byteaddr >> 1][byteaddr % 2] = Low8bits(value);

}

void fetch(int* x) {
  *x = getWordAt(CURRENT_LATCHES.PC);
}


System_Latches op_br(int instr) {
  System_Latches next = NEXT_LATCHES;
  
  if (  (next.N && BIT_11(instr)) ||
      (next.Z && BIT_10(instr)) ||
      (next.P && BIT_9(instr))  )
  {
    next.PC = Low16bits(next.PC + (sext32(CONST_9(instr), 9) << 1));
  }

  return next;
}

System_Latches op_add(int instr) {
  System_Latches next = NEXT_LATCHES;
  int result;

  if (BIT_5(instr)) {       /* immediate value */
    result = next.REGS[OP2(instr)] + sext32(CONST_5(instr), 5);
  }
  else {              /* register */
    result = next.REGS[OP2(instr)] + next.REGS[OP3(instr)];
  }

  result = Low16bits(result);
  next.REGS[OP1(instr)] = result;

  setConditionCodes(&next, result);

  return next;
}

System_Latches op_ldb(int instr) {
  System_Latches next = NEXT_LATCHES;
  
  int result = next.REGS[OP2(instr)] + sext32(CONST_6(instr), 6);   /* addr */
  result = Low8bits(getByteAt(result));

  next.REGS[OP1(instr)] = result;
  
  setConditionCodes(&next, result);

  return next;
}

System_Latches op_stb(int instr) {
  System_Latches next = NEXT_LATCHES;
  
  int addr = next.REGS[OP2(instr)] + sext32(CONST_6(instr), 6);

  setByteAt(addr, Low8bits(next.REGS[OP1(instr)]));
  
  return next;
}

System_Latches op_jsr(int instr) {
  System_Latches next = NEXT_LATCHES;

  int temp = next.PC;

  if (BIT_11(instr)) {      /* immediate value */
    next.PC = Low16bits(next.PC + (sext32(CONST_11(instr), 11) << 1));
  }
  else {              /* register */
    next.PC = next.REGS[OP2(instr)];
  }

  next.REGS[LR] = temp;

  return next;
}

System_Latches op_and(int instr) {
  System_Latches next = NEXT_LATCHES;
  int result;

  if (BIT_5(instr)) {       /* immediate value */
    result = next.REGS[OP2(instr)] & sext32(CONST_5(instr), 5);
  }
  else {              /* register */
    result = next.REGS[OP2(instr)] & next.REGS[OP3(instr)];
  }

  result = Low16bits(result);
  next.REGS[OP1(instr)] = result;

  setConditionCodes(&next, result);

  return next;
}

System_Latches op_ldw(int instr) {
  System_Latches next = NEXT_LATCHES;
  
  int result = next.REGS[OP2(instr)] + (sext32(OP3(instr), 6) << 1);    /* addr */
  result = Low16bits(getWordAt(result));

  next.REGS[OP1(instr)] = result;
  
  setConditionCodes(&next, result);

  return next;
}

System_Latches op_stw(int instr) {
  System_Latches next = NEXT_LATCHES;
  
  int addr = next.REGS[OP2(instr)] + (sext32(CONST_6(instr), 6) << 1);
  setWordAt(addr, next.REGS[OP1(instr)]);
  
  return next;
}

System_Latches op_rti(int instr) {  /* not required */

}

System_Latches op_xor(int instr) {
  System_Latches next = NEXT_LATCHES;
  int result;

  if (BIT_5(instr)) {       /* immediate value */
    result = next.REGS[OP2(instr)] ^ sext32(CONST_5(instr), 5);
  }
  else {              /* register */
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
  System_Latches next = NEXT_LATCHES;

  next.PC = next.REGS[OP2(instr)];

  return next;
}

System_Latches op_shf(int instr) {
  System_Latches next = NEXT_LATCHES;
  int result;

  if (BIT_4(instr)) {       /* right shift */
    if (BIT_5(instr)) {     /* arithmetic */
      result = sext32(next.REGS[OP2(instr)], WORD) >> CONST_4(instr);
    }
    else {            /* logical */
      result = next.REGS[OP2(instr)] >> CONST_4(instr);
    }
  }
  else {              /* left shift: */
    result = next.REGS[OP2(instr)] << CONST_4(instr);
  }

  result = Low16bits(result);
  next.REGS[OP1(instr)] = result;

  setConditionCodes(&next, result);

  return next;
}

System_Latches op_lea(int instr) {
  System_Latches next = NEXT_LATCHES;
  int result;

  result = next.PC + (sext32(CONST_9(instr), 9) << 1);
  
  result = Low16bits(result);
  next.REGS[OP1(instr)] = result;

  /* setConditionCodes(&next, result)   not meant to set this */

  return next;
}

System_Latches op_trap(int instr) {
  System_Latches next = NEXT_LATCHES;

  next.REGS[LR] = next.PC;
  next.PC = getWordAt(CONST_8(instr) << 1);

  return next;
}

System_Latches (*execute[NUM_INSTR])(int) = { op_br , op_add, op_ldb, op_stb,
                        op_jsr, op_and, op_ldw, op_stw,
                        op_rti, op_xor, op_invalid, op_invalid,
                        op_jmp, op_shf, op_lea, op_trap };

void process_instruction() {
  int x;
  fetch(&x);

  NEXT_LATCHES.PC += 2;

  NEXT_LATCHES = (*execute[DECODE(x)])(x);
}