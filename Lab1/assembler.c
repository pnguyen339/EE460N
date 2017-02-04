#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#include "SymbolTable.c"
#define MAX_LINE_LENGTH 255

FILE* infile = NULL;
FILE* outfile = NULL;

int toNume(char* pStr) {
	int num;
	long lnum;

	if (*pStr == '#') {					/* base 10 */
		pStr++;
		lnum = strtol(pStr, NULL, 10);
	}
	else if (*pStr == "x") {			/* base 16 */
		pStr++;
		lnum = strtol(pStr, NULL, 16);
	}
	else {
		printf("Error: invalid operand, %s\n", pStr);
		exit(4);
	}

	if (lnum > INT_MAX) { lnum = INT_MAX; } /* saturation on int max and min */
	else if (lnum < INT_MIN) { lnum = INT_MIN; }

	num = (int) lnum;					/* cast and return */
	return num;
}

enum code {
		ADD=1, AND=5,
		BR =0,
		HALT=15, JMP=12, JSR=4, JSRR=4, LDB=2, LDW=6,
		LEA=14, NOP=0, NOT=9, RET=12, 
		LSHF=13, RSHFL=13, RSHFA=13, 
		RTI, STB=3, STW=7, TRAP=15, XOR=9
	};

int findOpcode(char *ptr) {

	if(strcmp(ptr,"add") == 0) {
		return ADD;
	}
	else if(strcmp(ptr,"and") == 0) {
		return AND;
	}
	else if(strcmp(ptr,"br") == 0) {
		return BR;
	}
	else if(strcmp(ptr,"halt") == 0) {
		return HALT;
	}
	else if(strcmp(ptr,"jmp") == 0) {
		return JMP;
	}
	else if(strcmp(ptr,"jsr") == 0) {
		return JSR;
	}
	else if(strcmp(ptr,"jsrr") == 0) {
		return JSRR;
	}
	else if(strcmp(ptr,"ldb") == 0) {
		return LDB;
	}
	else if(strcmp(ptr,"ldw") == 0) {
		return LDW;
	}
	else if(strcmp(ptr,"lea") == 0) {
		return LEA;
	}
	else if(strcmp(ptr,"nop") == 0) {
		return NOP;
	}
	else if(strcmp(ptr,"not") == 0) {
		return NOT;
	}
	else if(strcmp(ptr,"ret") == 0) {
		return RET;
	}
	else if(strcmp(ptr,"lshf") == 0) {
		return LSHF;
	}
	else if(strcmp(ptr,"rshfl") == 0) {
		return RSHFL;
	}
	else if(strcmp(ptr,"rshfa") == 0) {
		return RSHFA;
	}
	else if(strcmp(ptr,"rti") == 0) {
		return RTI;
	}
	else if(strcmp(ptr,"stb") == 0) {
		return STB;
	}
	else if(strcmp(ptr,"stw") == 0) {
		return STW;
	}
	else if(strcmp(ptr,"trap") == 0) {
		return TRAP;
	}	
	else if(strcmp(ptr,"xor") == 0) {
		return XOR;
	}
	else
		return -1;
}

int isOpcode(char *ptr)
	{	
		return findOpcode(ptr);		

	}

enum
	{
	   DONE, OK, EMPTY_LINE
	};


int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char
	** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
	)
	{
	   char * lRet, * lPtr;
	   int i;
	   if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
		return( DONE );
	   for( i = 0; i < strlen( pLine ); i++ )
		pLine[i] = tolower( pLine[i] );
	   
           /* convert entire line to lowercase */
	   *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	   /* ignore the comments */
	   lPtr = pLine;

	   while( *lPtr != ';' && *lPtr != '\0' &&
	   *lPtr != '\n' ) 
		lPtr++;

	   *lPtr = '\0';
	   if( !(lPtr = strtok( pLine, "\t\n ," ) ) ) 
		return( EMPTY_LINE );

	   if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
	   {
		*pLabel = lPtr;
		if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) {
			printf("Invalid opcode: %s", pLabel);
			exit(2);
		}
	   }
	   
           *pOpcode = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   
           *pArg1 = lPtr;
	   
           if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg2 = lPtr;
	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg3 = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg4 = lPtr;

	   return( OK );
	}
void Add(FILE *pOutfile, char *A1, char *A2,char *A3, char *A4){

}

void setSymbol(FILE *pInfile, symbol* ptr, int* len){
	int lineNum = 0;
	char *line;
	char *Opcode;
	char *Label=NULL;
	char *Arg1;
	char *Arg2;
	char *Arg3;
	char *Arg4;
	while(readAndParse( pInfile, line, &*Label, &*Opcode, &*Arg1,  &*Arg2, &*Arg3, &*Arg4) !=DONE){
		if(*Label!=NULL){
			if(findSym(ptr,*len,Label) == -1){
				newSymbol(ptr,len,Label,lineNum);
				*Label = NULL;
			}
			else{
				printf("Invalid label: %s", Label);
				exit(4);	
			}
		} 

	}
} 
int main(int argc, char* argv[]) {
	char *Opcode;
	char *Label;
	char *Arg1;
	char *Arg2;
	char *Arg3;
	char *Arg4;
	symbol* Table= NULL;
	int length = 0;
	if (argc != 4) { 
		printf("Incorrect number of arguments (found %d, expected 3)\n", (argc - 1));
		exit(4);
	}

/*	char* prgName = argv[1];
	char* iFileName = argv[2];
	char* oFileName = argv[3];

	printf("program name = '%s'\n", prgName);
	printf("i/o files are '%s' input and '%s' output", iFileName, oFileName); */

	infile = fopen(argv[2], "r");
	outfile = fopen(argv[3],"w");

	if (!infile) {
		printf("Error: Cannot open file %s\n", argv[2]);
		exit(4);
	}

	if (!outfile) {
		printf("Error: Cannot open file %s\n", argv[3]);
		exit(4);
	}

	fclose(infile);
	fclose(outfile);
}
