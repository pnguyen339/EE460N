/*
    Name 1: Your Name
    UTEID 1: Your UTEID
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Simulator                                           */
/*                                                             */
/*   EE 460N - Lab 5                                           */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files:  ucode        Microprogram file                      */
/*         pagetable    page table in LC-3b machine language   */
/*         isaprogram   LC-3b machine language program file    */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void eval_micro_sequencer();
void cycle_memory();
void eval_bus_drivers();
void drive_bus();
void latch_datapath_values();

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
/* Definition of the control store layout.                     */
/***************************************************************/
#define CONTROL_STORE_ROWS 64
#define INITIAL_STATE_NUMBER 18

/***************************************************************/
/* Definition of bit order in control store word.              */
/***************************************************************/
enum CS_BITS {                                                  
    IRD,
    COND1, COND0,
    J5, J4, J3, J2, J1, J0,
    LD_MAR,
    LD_MDR,
    LD_IR,
    LD_BEN,
    LD_REG,
    LD_CC,
    LD_PC,
    GATE_PC,
    GATE_MDR,
    GATE_ALU,
    GATE_MARMUX,
    GATE_SHF,
    PCMUX1, PCMUX0,
    DRMUX,
    SR1MUX,
    ADDR1MUX,
    ADDR2MUX1, ADDR2MUX0,
    MARMUX,
    ALUK1, ALUK0,
    MIO_EN,
    R_W,
    DATA_SIZE,
    LSHF1,
/* MODIFY: you have to add all your new control signals */
		COND2,
	    GATESP,
	    GATEPP,
	    LD_USPSAVED,
	    SP,
	    PP,
	    GATEPSR,
	    LD_PRIV,
	    SetSS,
	    LD_CheckEXC,
	    CLR_Check,
	    GATEVector,
	    CLR_TrapFLG,
	    CC_Logic,
	    DRMUX2,
	    SR1MUX2,
    CONTROL_STORE_BITS
} CS_BITS;

/***************************************************************/
/* Functions to get at the control bits.                       */
/***************************************************************/
int GetIRD(int *x)           { return(x[IRD]); }
//int GetCOND(int *x)          { return((x[COND1] << 1) + x[COND0]); }
int GetJ(int *x)             { return((x[J5] << 5) + (x[J4] << 4) +
				      (x[J3] << 3) + (x[J2] << 2) +
				      (x[J1] << 1) + x[J0]); }
int GetLD_MAR(int *x)        { return(x[LD_MAR]); }
int GetLD_MDR(int *x)        { return(x[LD_MDR]); }
int GetLD_IR(int *x)         { return(x[LD_IR]); }
int GetLD_BEN(int *x)        { return(x[LD_BEN]); }
int GetLD_REG(int *x)        { return(x[LD_REG]); }
int GetLD_CC(int *x)         { return(x[LD_CC]); }
int GetLD_PC(int *x)         { return(x[LD_PC]); }
int GetGATE_PC(int *x)       { return(x[GATE_PC]); }
int GetGATE_MDR(int *x)      { return(x[GATE_MDR]); }
int GetGATE_ALU(int *x)      { return(x[GATE_ALU]); }
int GetGATE_MARMUX(int *x)   { return(x[GATE_MARMUX]); }
int GetGATE_SHF(int *x)      { return(x[GATE_SHF]); }
int GetPCMUX(int *x)         { return((x[PCMUX1] << 1) + x[PCMUX0]); }
//int GetDRMUX(int *x)         { return(x[DRMUX]); }
//int GetSR1MUX(int *x)        { return(x[SR1MUX]); }
int GetADDR1MUX(int *x)      { return(x[ADDR1MUX]); }
int GetADDR2MUX(int *x)      { return((x[ADDR2MUX1] << 1) + x[ADDR2MUX0]); }
int GetMARMUX(int *x)        { return(x[MARMUX]); }
int GetALUK(int *x)          { return((x[ALUK1] << 1) + x[ALUK0]); }
int GetMIO_EN(int *x)        { return(x[MIO_EN]); }
int GetR_W(int *x)           { return(x[R_W]); }
int GetDATA_SIZE(int *x)     { return(x[DATA_SIZE]); } 
int GetLSHF1(int *x)         { return(x[LSHF1]); }
/* MODIFY: you can add more Get functions for your new control signals */


int GetCOND(int *x)          { return(  (x[COND2] << 2) + (x[COND1] << 1) + x[COND0]); }
int GetDRMUX(int *x)         { return(  (x[DRMUX2] << 1) + x[DRMUX]); }
int GetSR1MUX(int *x)        { return(  (x[SR1MUX2] << 1) +x[SR1MUX]); }
int GetCC_Logic(int *x)      { return(  x[CC_Logic]);  }
int GetCLR_TrapFLG(int *x)   {  return x[CLR_TrapFLG]; }
int GetGATEVector(int *x)    {  return x[GATEVector];  }
int GetLD_CheckEXC(int *x)   {  return x[LD_CheckEXC];  }
int GetSetSS(int *x)        {   return x[SetSS];    }
int GetLD_PRIV(int *x)      {   return x[LD_PRIV];  }
int GetGATEPSR(int *x)      {   return x[GATEPSR];  }
int GetPP(int *x)           {   return x[PP];   }
int GetSP(int *x)           {   return x[SP];   }
int GetLD_USPSAVED(int *x)  {   return x[LD_USPSAVED];  }
int GetGATEPP(int *x)   {   return x[GATEPP];   }
int GetGATESP(int *x)   {   return x[GATESP];   }
int GetCLR_Check(int *x)    { return x[CLR_Check];}

/***************************************************************/
/* The control store rom.                                      */
/***************************************************************/
int CONTROL_STORE[CONTROL_STORE_ROWS][CONTROL_STORE_BITS];

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
   There are two write enable signals, one for each byte. WE0 is used for 
   the least significant byte of a word. WE1 is used for the most significant 
   byte of a word. */

#define WORDS_IN_MEM    0x2000 /* 32 frames */
#define MEM_CYCLES      5
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */
int BUS;	/* value of the bus */

typedef struct System_Latches_Struct{

int PC,		/* program counter */
    MDR,	/* memory data register */
    MAR,	/* memory address register */
    IR,		/* instruction register */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P,		/* p condition bit */
    BEN;        /* ben register */

int READY;	/* ready bit */
  /* The ready bit is also latched as you dont want the memory system to assert it 
     at a bad point in the cycle*/

int REGS[LC_3b_REGS]; /* register file. */

int MICROINSTRUCTION[CONTROL_STORE_BITS]; /* The microintruction */

int STATE_NUMBER; /* Current State Number - Provided for debugging */ 

/* For lab 4 */
int INTV; /* Interrupt vector register */
int INT_Signal; /* Interrupt Signal*/
int EXC; /* Exception Signal*/
int EXCV;
int SSP; /* Initial value of system stack pointer */
/* MODIFY: you should add here any other registers you need to implement interrupts and exceptions */
int USPSAVED;
int PRIV;
int TRAP_FLAG;
/* For lab 5 */
int PTBR; /* This is initialized when we load the page table */
int VA;   /* Temporary VA register */
/* MODIFY: you should add here any other registers you need to implement virtual memory */

} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/* For lab 5 */
#define PAGE_NUM_BITS 9
#define PTE_PFN_MASK 0x3E00
#define PTE_VALID_MASK 0x0004
#define PAGE_OFFSET_MASK 0x1FF

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int CYCLE_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands.                   */
/*                                                             */
/***************************************************************/
void help() {                                                    
    printf("----------------LC-3bSIM Help-------------------------\n");
    printf("go               -  run program to completion       \n");
    printf("run n            -  execute program for n cycles    \n");
    printf("mdump low high   -  dump memory from low to high    \n");
    printf("rdump            -  dump the register & bus values  \n");
    printf("?                -  display this help menu          \n");
    printf("quit             -  exit the program                \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  eval_micro_sequencer();   
  cycle_memory();
  eval_bus_drivers();
  drive_bus();
  latch_datapath_values();
  if(CYCLE_COUNT  == 299) {
  	  NEXT_LATCHES.INT_Signal = 1;
  	  NEXT_LATCHES.INTV = 0x01;
  }
  CURRENT_LATCHES = NEXT_LATCHES;

  CYCLE_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles.                 */
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
/* Purpose   : Simulate the LC-3b until HALTed.                 */
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

    printf("\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    printf("-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
	printf("  0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    printf("\n");

    /* dump the memory contents into the dumpsim file */
    fprintf(dumpsim_file, "\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    fprintf(dumpsim_file, "-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
	fprintf(dumpsim_file, " 0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
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
    printf("Cycle Count  : %d\n", CYCLE_COUNT);
    printf("PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    printf("IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    printf("STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    printf("BUS          : 0x%0.4x\n", BUS);
    printf("MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    printf("MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    printf("Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
	printf("%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    printf("\n");

    /* dump the state information into the dumpsim file */
    fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
    fprintf(dumpsim_file, "-------------------------------------\n");
    fprintf(dumpsim_file, "Cycle Count  : %d\n", CYCLE_COUNT);
    fprintf(dumpsim_file, "PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    fprintf(dumpsim_file, "IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    fprintf(dumpsim_file, "STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    fprintf(dumpsim_file, "BUS          : 0x%0.4x\n", BUS);
    fprintf(dumpsim_file, "MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    fprintf(dumpsim_file, "MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    fprintf(dumpsim_file, "Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
	fprintf(dumpsim_file, "%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
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
/* Procedure : init_control_store                              */
/*                                                             */
/* Purpose   : Load microprogram into control store ROM        */ 
/*                                                             */
/***************************************************************/
void init_control_store(char *ucode_filename) {                 
    FILE *ucode;
    int i, j, index;
    char line[200];

    printf("Loading Control Store from file: %s\n", ucode_filename);

    /* Open the micro-code file. */
    if ((ucode = fopen(ucode_filename, "r")) == NULL) {
	printf("Error: Can't open micro-code file %s\n", ucode_filename);
	exit(-1);
    }

    /* Read a line for each row in the control store. */
    for(i = 0; i < CONTROL_STORE_ROWS; i++) {
	if (fscanf(ucode, "%[^\n]\n", line) == EOF) {
	    printf("Error: Too few lines (%d) in micro-code file: %s\n",
		   i, ucode_filename);
	    exit(-1);
	}

	/* Put in bits one at a time. */
	index = 0;

	for (j = 0; j < CONTROL_STORE_BITS; j++) {
	    /* Needs to find enough bits in line. */
	    if (line[index] == '\0') {
		printf("Error: Too few control bits in micro-code file: %s\nLine: %d\n",
		       ucode_filename, i);
		exit(-1);
	    }
	    if (line[index] != '0' && line[index] != '1') {
		printf("Error: Unknown value in micro-code file: %s\nLine: %d, Bit: %d\n",
		       ucode_filename, i, j);
		exit(-1);
	    }

	    /* Set the bit in the Control Store. */
	    CONTROL_STORE[i][j] = (line[index] == '0') ? 0:1;
	    index++;
	}

	/* Warn about extra bits in line. */
	if (line[index] != '\0')
	    printf("Warning: Extra bit(s) in control store file %s. Line: %d\n",
		   ucode_filename, i);
    }
    printf("\n");
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
void load_program(char *program_filename, int is_virtual_base) {
    FILE * prog;
    int ii, word, program_base, pte, virtual_pc;

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

    if (is_virtual_base) {
      if (CURRENT_LATCHES.PTBR == 0) {
	printf("Error: Page table base not loaded %s\n", program_filename);
	exit(-1);
      }

      /* convert virtual_base to physical_base */
      virtual_pc = program_base << 1;
      pte = (MEMORY[(CURRENT_LATCHES.PTBR + (((program_base << 1) >> PAGE_NUM_BITS) << 1)) >> 1][1] << 8) |
	     MEMORY[(CURRENT_LATCHES.PTBR + (((program_base << 1) >> PAGE_NUM_BITS) << 1)) >> 1][0];

      printf("virtual base of program: %04x\npte: %04x\n", program_base << 1, pte);
		if ((pte & PTE_VALID_MASK) == PTE_VALID_MASK) {
	      program_base = (pte & PTE_PFN_MASK) | ((program_base << 1) & PAGE_OFFSET_MASK);
   	   printf("physical base of program: %x\n\n", program_base);
	      program_base = program_base >> 1;
		} else {
   	   printf("attempting to load a program into an invalid (non-resident) page\n\n");
			exit(-1);
		}
    }
    else {
      /* is page table */
     CURRENT_LATCHES.PTBR = program_base << 1;
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
	MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;;
	ii++;
    }

    if (CURRENT_LATCHES.PC == 0 && is_virtual_base)
      CURRENT_LATCHES.PC = virtual_pc;

    printf("Read %d words from program into memory.\n\n", ii);
}

/***************************************************************/
/*                                                             */
/* Procedure : initialize                                      */
/*                                                             */
/* Purpose   : Load microprogram and machine language program  */ 
/*             and set up initial state of the machine         */
/*                                                             */
/***************************************************************/
void initialize(char *ucode_filename, char *pagetable_filename, char *program_filename, int num_prog_files) {
    int i;
    init_control_store(ucode_filename);

    init_memory();
    load_program(pagetable_filename,0);
    for ( i = 0; i < num_prog_files; i++ ) {
	load_program(program_filename,1);
	while(*program_filename++ != '\0');
    }
    CURRENT_LATCHES.Z = 1;
    CURRENT_LATCHES.STATE_NUMBER = INITIAL_STATE_NUMBER;
    memcpy(CURRENT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[INITIAL_STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);


/* MODIFY: you can add more initialization code HERE */
    CURRENT_LATCHES.SSP = 0x3000; /* Initial value of system stack pointer */
    CURRENT_LATCHES.INT_Signal = 0; /* Initial value of Interrupt Signal*/
    CURRENT_LATCHES.INTV = 0x01u;
    CURRENT_LATCHES.EXC = 0; /* Initial value of EXC */
    CURRENT_LATCHES.PRIV = 1; /*initialize the lc3 into User Mode */
    CURRENT_LATCHES.TRAP_FLAG = 1; /*Initialize the Trap Flag to 1*/

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
    if (argc < 4) {
	printf("Error: usage: %s <micro_code_file> <page table file> <program_file_1> <program_file_2> ...\n",
	       argv[0]);
	exit(1);
    }

    printf("LC-3b Simulator\n\n");

    initialize(argv[1], argv[2], argv[3], argc - 3);

    if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
	printf("Error: Can't open dumpsim file\n");
	exit(-1);
    }

    while (1)
	get_command(dumpsim_file);

}

/***************************************************************/
/* Do not modify the above code, except for the places indicated 
   with a "MODIFY:" comment.
   You are allowed to use the following global variables in your
   code. These are defined above.

   CONTROL_STORE
   MEMORY
   BUS

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */
/***************************************************************/

/* High and low bytes */
#define Low8bits(x)  (x & 0x00FFu)
#define High8bits(x) ((x & 0xFF00u) >> 8)

/* Useful constants */
#define NUM_INSTR 16
#define WORD    16
#define LR      7

/* Bits used for steering or otherwise important values */
#define SIGN_BIT(x) ((x & 0x8000u) >> 15)

#define BIT_11(x) ((x & 0x0800u) >> 11)
#define BIT_10(x) ((x & 0x0400u) >> 10)
#define BIT_9(x)  ((x & 0x0200u) >> 9)
#define BIT_5(x)  ((x & 0x0020u) >> 5)
#define BIT_4(x)  ((x & 0x0010u) >> 4)

/* Opcode */
#define DECODE(x) ((x & 0xF000u) >> 12)

/* Operands within instruction */
#define OP1(x)    ((x & 0x0E00u) >> 9) /* [11,9]*/
#define OP2(x)    ((x & 0x01C0u) >> 6) /* [8,6]*/
#define OP3(x)     (x & 0x0007u)       /* [ 2,0]*/

/* Immediate values at end of instruction */
#define CONST_11(x) (x & 0x07FFu)
#define CONST_9(x)  (x & 0x01FFu)
#define CONST_8(x)  (x & 0x00FFu)
#define CONST_6(x)  (x & 0x003Fu)
#define CONST_5(x)  (x & 0x001Fu)
#define CONST_4(x)  (x & 0x000Fu)

int sext32(int input, int num_bits) {
  int sign = (1 << (num_bits - 1)) & input;
  
  input |= -1 * sign;     

  return input;
}

void eval_micro_sequencer() {

  /* 
   * Evaluate the address of the next state according to the 
   * micro sequencer logic. Latch the next microinstruction.
   */ 
    if(GetIRD(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        NEXT_LATCHES.STATE_NUMBER = 0 + DECODE(CURRENT_LATCHES.IR);
    }
    
    else if(GetCOND(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        NEXT_LATCHES.STATE_NUMBER = (GetJ(CURRENT_LATCHES.MICROINSTRUCTION) & 0xFD) ^ (CURRENT_LATCHES.READY<<1);
    }
    
    else if(GetCOND(CURRENT_LATCHES.MICROINSTRUCTION) == 2) {
        NEXT_LATCHES.STATE_NUMBER = (GetJ(CURRENT_LATCHES.MICROINSTRUCTION) & 0xFB) ^ (CURRENT_LATCHES.BEN<<2);
    }
    
    else if(GetCOND(CURRENT_LATCHES.MICROINSTRUCTION) == 3) {
        NEXT_LATCHES.STATE_NUMBER = (GetJ(CURRENT_LATCHES.MICROINSTRUCTION) & 0xFE) ^ BIT_11(CURRENT_LATCHES.IR);
    }
    else if(GetCOND(CURRENT_LATCHES.MICROINSTRUCTION) == 4) {
        NEXT_LATCHES.STATE_NUMBER = (GetJ(CURRENT_LATCHES.MICROINSTRUCTION)) ^ ((CURRENT_LATCHES.EXC ^ CURRENT_LATCHES.INT_Signal)<<3);
    }
    else if(GetCOND(CURRENT_LATCHES.MICROINSTRUCTION) == 6) {
        NEXT_LATCHES.STATE_NUMBER = (GetJ(CURRENT_LATCHES.MICROINSTRUCTION)) ^ (CURRENT_LATCHES.PRIV << 4);
    }
    else {
        NEXT_LATCHES.STATE_NUMBER = GetJ(CURRENT_LATCHES.MICROINSTRUCTION);
    }

    if(GetCOND(CURRENT_LATCHES.MICROINSTRUCTION) == 5) {
        if(CURRENT_LATCHES.EXC == 1) {
            NEXT_LATCHES.STATE_NUMBER = 0x3F;
        }
    }


    memcpy(NEXT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[NEXT_LATCHES.STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);


}
int DATA_IN_MEM;
int MEM_CYC_LEFT;
int MEMBUSY = 0;
int pagenumber;
int PTEaddr;

void cycle_memory() {
 
  /* 
   * This function emulates memory and the WE logic. 
   * Keep track of which cycle of MEMEN we are dealing with.  
   * If fourth, we need to latch Ready bit at the end of 
   * cycle to prepare microsequencer for the fifth cycle.  
   */
    NEXT_LATCHES.READY = 0;
    if(GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        
        if(MEMBUSY == 0) {
            MEMBUSY = 1;
            NEXT_LATCHES.READY = 0;
            MEM_CYC_LEFT = MEM_CYCLES-1;
        }
        
        if(MEM_CYC_LEFT == 1 ) {
            NEXT_LATCHES.READY = 1;
            int byteaddr = CURRENT_LATCHES.MAR;
            
            if(GetR_W(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {

                /* if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
                    
                     DATA_IN_MEM = 0;
                     DATA_IN_MEM |= MEMORY[byteaddr >> 1][byteaddr % 2];
                    
                 }
                 else {
                    DATA_IN_MEM = 0;
                    DATA_IN_MEM |= MEMORY[byteaddr >> 1][1] << 8;  
                    DATA_IN_MEM |= MEMORY[byteaddr >> 1][0];
                    
                 }*/
                    DATA_IN_MEM = 0;
                    DATA_IN_MEM |= MEMORY[byteaddr >> 1][1] << 8;  
                    DATA_IN_MEM |= MEMORY[byteaddr >> 1][0];
            }

            else {
        
                if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
                    MEMORY[byteaddr >> 1][byteaddr % 2] = Low8bits(CURRENT_LATCHES.MDR >> (8*(CURRENT_LATCHES.MAR & 0x01u)));
                }
                else {
                    MEMORY[byteaddr >> 1][1] = High8bits(CURRENT_LATCHES.MDR);
                    MEMORY[byteaddr >> 1][0] = Low8bits(CURRENT_LATCHES.MDR);
                }

                PTEaddr |= 0x02;

            }

            MEMORY[CURRENT_LATCHES.PTBR + (pagenumber << 1) >> 1][1] = High8bits(PTEaddr);
            MEMORY[CURRENT_LATCHES.PTBR + (pagenumber << 1) >> 1][0] = Low8bits(PTEaddr & 0x00FF);

        }

        MEM_CYC_LEFT = MEM_CYC_LEFT-1;
        if(MEM_CYC_LEFT == -1)
            MEMBUSY = 0;
    }
}


int adder_ADDR1_ADDR2() {
    int ADDR2 = 0;
    int ADDR1 = 0;
    switch(GetADDR2MUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
        case 0: ADDR2 = 0; break;
        case 1: ADDR2 = sext32(CONST_6(CURRENT_LATCHES.IR), 6); break;
        case 2: ADDR2 = sext32(CONST_9(CURRENT_LATCHES.IR), 9); break;
        case 3: ADDR2 = sext32(CONST_11(CURRENT_LATCHES.IR),11); break;
    }

    if(GetADDR1MUX(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
        ADDR1 = CURRENT_LATCHES.PC;
    }
    else {
        if(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
            ADDR1 = CURRENT_LATCHES.REGS[OP2(CURRENT_LATCHES.IR)];
        }
    }
    
    ADDR2 = ADDR2 << GetLSHF1(CURRENT_LATCHES.MICROINSTRUCTION);

    return Low16bits(ADDR2 + ADDR1); 

}


int op_add(int instr) {
  int result;
  if (BIT_5(instr)) {       /* immediate value */
    result = CURRENT_LATCHES.REGS[OP2(instr)] + sext32(CONST_5(instr), 5);
  }
  else {              /* register */
    result = CURRENT_LATCHES.REGS[OP2(instr)] + CURRENT_LATCHES.REGS[OP3(instr)];
  }

  result = Low16bits(result);
  return result;
}
int op_and(int instr) {
  int result;

  if (BIT_5(instr)) {       /* immediate value */
    result = CURRENT_LATCHES.REGS[OP2(instr)] & sext32(CONST_5(instr), 5);
  }
  else {              /* register */
    result = CURRENT_LATCHES.REGS[OP2(instr)] & CURRENT_LATCHES.REGS[OP3(instr)];
  }

  result = Low16bits(result);
  return result;
}

int op_xor(int instr) {
  int result;

  if (BIT_5(instr)) {       /* immediate value */
    result = CURRENT_LATCHES.REGS[OP2(instr)] ^ sext32(CONST_5(instr), 5);
  }
  else {              /* register */
    result = CURRENT_LATCHES.REGS[OP2(instr)] ^ CURRENT_LATCHES.REGS[OP3(instr)];
  }

  result = Low16bits(result);
  return result;
}

int pass(int instr) {
    if(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
        return CURRENT_LATCHES.REGS[OP1(instr)];
    }
    else if(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        return CURRENT_LATCHES.REGS[OP2(instr)];
    }
    else return CURRENT_LATCHES.REGS[6];
}

int ALU_logic() {
    switch(GetALUK(CURRENT_LATCHES.MICROINSTRUCTION)) {
        case 0: return op_add(CURRENT_LATCHES.IR);break;
        case 1: return op_and(CURRENT_LATCHES.IR);break;
        case 2: return op_xor(CURRENT_LATCHES.IR);break;
        case 3: return pass(CURRENT_LATCHES.IR);break;
    }
    return 0;

}

int TEMP;

void eval_bus_drivers() {

  /* 
   * Datapath routine emulating operations before driving the bus.
   * Evaluate the input of tristate drivers 
   *             GATE_MARMUX,
   *         GATE_PC,
   *         GATE_ALU,
   *         GATE_SHF,
   *         GATE_MDR.
   */
    if(GetGATE_SHF(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        int result;
        int instr = CURRENT_LATCHES.IR;
        
        if (BIT_4(instr)) {       /* right shift */
            if (BIT_5(instr)) {     /* arithmetic */
                result = sext32(CURRENT_LATCHES.REGS[OP2(instr)], WORD) >> CONST_4(instr);
            }
            else {            /* logical */
                result = CURRENT_LATCHES.REGS[OP2(instr)] >> CONST_4(instr);
            }
        }     
        else {              /* left shift: */
        result = CURRENT_LATCHES.REGS[OP2(instr)] << CONST_4(instr);
        }

        TEMP = Low16bits(result);
    }

    else if(GetGATE_MARMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(GetMARMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
            int result = 0;
            result |= (CURRENT_LATCHES.IR & 0x007Fu);
            result = result << 1;
            TEMP = result;
        }
        else {
            TEMP = adder_ADDR1_ADDR2();
        }
    }
    
    else if(GetGATE_ALU(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        TEMP = Low16bits(ALU_logic());
    }
    
    else if(GetGATE_MDR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
            TEMP = sext32(Low8bits(CURRENT_LATCHES.MDR >> 8*(CURRENT_LATCHES.MAR % 2)), 8);
            //TEMP = Low16bits(TEMP);
        }
        else
            TEMP = CURRENT_LATCHES.MDR;
    }
    
    /* new Gate*/
    else if(GetGATEPP(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(GetPP(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
            TEMP = CURRENT_LATCHES.REGS[6] + 2;
        }
        else {
            TEMP = CURRENT_LATCHES.REGS[6] - 2;
        }
    }
    
    else if (GetGATESP(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(GetSP(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
            TEMP = CURRENT_LATCHES.SSP;
        }
        else {
            TEMP = CURRENT_LATCHES.USPSAVED;
        }
    }
    
    else if (GetGATEPSR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        TEMP = (CURRENT_LATCHES.PRIV << 15) + (CURRENT_LATCHES.N << 2) + (CURRENT_LATCHES.Z << 1) + (CURRENT_LATCHES.P); 
    }

    else if (GetGATEVector(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(CURRENT_LATCHES.EXC == 1) {
            TEMP = 0x0200 + (CURRENT_LATCHES.EXCV<<1);
        }
        else {
            TEMP = 0x0200 + (CURRENT_LATCHES.INTV<<1);
        }
    }

}


void drive_bus() {

  /* 
   * Datapath routine for driving the bus from one of the 9 possible 
   * tristate drivers. 
   */
   if(GetGATE_SHF(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATE_ALU(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATE_MARMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATE_MDR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATE_PC(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = CURRENT_LATCHES.PC;
   }
   else if(GetGATEPSR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATEVector(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATESP(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }
   else if(GetGATEPP(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = TEMP;
   }

}

void setConditionCodes(System_Latches* latches, int value, int logic) {
    if(logic == 0) {
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
    else {
        latches->N = (value & 0x04) >> 2;
        latches->Z = (value & 0x02) >> 1;
        latches->P = (value & 0x01);
    }
}


	int pagenumber;
	int PTEaddr;

void latch_datapath_values() {

  /* 
   * Datapath routine for computing all functions that need to latch
   * values in the data path at the end of this cycle.  Some values
   * require sourcing the bus; therefore, this routine has to come 
   * after drive_bus.
   */
    if (GetLD_IR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        
        NEXT_LATCHES.IR = BUS;
        if(DECODE(BUS) == 0x0Fu) {
                	NEXT_LATCHES.TRAP_FLAG = 0;
                	NEXT_LATCHES.PRIV = 0;
                	CURRENT_LATCHES.PRIV = 0;
        }
    }
    
    if (GetLD_PC(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        switch(GetPCMUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
            case 0: NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2; break;
            case 1: NEXT_LATCHES.PC = BUS; break;
            case 2: NEXT_LATCHES.PC = adder_ADDR1_ADDR2(); break;
            case 3: NEXT_LATCHES.PC = CURRENT_LATCHES.PC - 2; break;
        }
    }

    if(GetLD_CC(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        setConditionCodes(&NEXT_LATCHES, BUS, GetCC_Logic(CURRENT_LATCHES.MICROINSTRUCTION));
    }

    if(GetLD_MDR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if (GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
            if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
                NEXT_LATCHES.MDR = BUS << (8 * (CURRENT_LATCHES.MAR & 0x01u));
            }
            else
            {
                NEXT_LATCHES.MDR = BUS;
            }
        }
        else {
            NEXT_LATCHES.MDR = DATA_IN_MEM;
        }
    }
    
    if(GetLD_REG(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(GetDRMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 0) {
            NEXT_LATCHES.REGS[OP1(CURRENT_LATCHES.IR)] = Low16bits(BUS);
        }
        else if(GetDRMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1){
            NEXT_LATCHES.REGS[LR] = Low16bits(BUS);
        }
        else if(GetDRMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 2){
            NEXT_LATCHES.REGS[6] = Low16bits(BUS);
        }
    }
    
    if(GetLD_BEN(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        int instr = CURRENT_LATCHES.IR;
        NEXT_LATCHES.BEN = (CURRENT_LATCHES.N && BIT_11(instr)) || (CURRENT_LATCHES.Z && BIT_10(instr)) || (CURRENT_LATCHES.P && BIT_9(instr));  
    }

    if(GetLD_MAR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
    	pagenumber = Low16bits(BUS) >> 9;

    	PTEaddr = 0;
    	PTEaddr |= MEMORY[CURRENT_LATCHES.PTBR + (pagenumber << 1) >> 1][1] << 8;
    	PTEaddr |= MEMORY[CURRENT_LATCHES.PTBR + (pagenumber << 1) >> 1][0];
    	PTEaddr |= 0x01;
    	int offset9 = BUS & 0x01FF;
    	int pfn = PTEaddr & 0x3E00;

        NEXT_LATCHES.MAR =  pfn + offset9;
    }


    if(GetLD_USPSAVED(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        NEXT_LATCHES.USPSAVED = CURRENT_LATCHES.REGS[6];
    }

    if(GetLD_PRIV(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        if(GetSetSS(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
            NEXT_LATCHES.PRIV = 0;
        }
        else
        {
            NEXT_LATCHES.PRIV = (BUS & 0x8000) >> 15;
        }
    }

    if(GetLD_CheckEXC(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        int Protection = 0;
        int Unalligned = 0;
        int UnknownOP = 0;
        int PageFault = 0;
        int Opcode = DECODE(CURRENT_LATCHES.IR);        


        if(Opcode == 0x0A || Opcode == 0x0B)
            UnknownOP = 1;
        if(GetLD_MAR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
			if(!(CURRENT_LATCHES.PRIV == ((PTEaddr & 0x08) >> 3))) {
				Protection = 1;
			}
			PageFault = !((PTEaddr >> 2) & 1);
			if(BUS %2 != 0 && GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
				Unalligned = 1;
			}
        }

        if(Protection == 1){
            NEXT_LATCHES.EXCV = 0x04;
            NEXT_LATCHES.EXC = 1;
        }
        else if(PageFault == 1){
        	NEXT_LATCHES.EXCV = 0x02;
        	NEXT_LATCHES.EXC = 1;
        }
        else if(Unalligned == 1) {
            NEXT_LATCHES.EXCV = 0x03;
            NEXT_LATCHES.EXC = 1;
        }
        else if(UnknownOP == 1) {
            NEXT_LATCHES.EXCV = 0x05;
            NEXT_LATCHES.IR = 0;
            NEXT_LATCHES.EXC = 1;
        }
    }

    if(GetCLR_TrapFLG(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        NEXT_LATCHES.TRAP_FLAG = 1;
        NEXT_LATCHES.PRIV = 1;
    }

    if(GetCLR_Check(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        NEXT_LATCHES.INT_Signal = 0;
        NEXT_LATCHES.EXC = 0;
    }


}
