#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#define MAX_LINE_LENGTH 255

FILE* infile = NULL;
FILE* outfile = NULL;

enum code{
		ADD, AND,
		BR,BRNZP,BRNP,BRNZ,BRZP,BRN,BRP,BRZ,
		HALT, JMP, JSR, JSRR, LDB, LDW,
		LEA, NOP, NOT, RET, 
		LSHF, RSHFL, RSHFA, 
		RTI, STB, STW, TRAP, XOR
	};

int findOpcode(char *ptr){

	if(strcmp(ptr,"add") == 0){
		return ADD;
	}
	else if(strcmp(ptr,"and") == 0){
		return AND;
	}
	else if(strcmp(ptr,"br") == 0){
		return BR;
	}
	else if(strcmp(ptr,"halt") == 0){
		return HALT;
	}
	else if(strcmp(ptr,"jmp") == 0){
		return jmp;
	}
	else if(strcmp(ptr,"jsr") == 0){
		return JSR;
	}
	else if(strcmp(ptr,"jsrr") == 0){
		return JSRR;
	}
	else if(strcmp(ptr,"ldb") == 0){
		return LDB;
	}
	else if(strcmp(ptr,"ldw") == 0){
		return LDW;
	}
	else if(strcmp(ptr,"lea") == 0){
		return LEA;
	}
	else if(strcmp(ptr,"nop") == 0){
		return NOP;
	}
	else if(strcmp(ptr,"not") == 0){
		return NOT;
	}
	else if(strcmp(ptr,"ret") == 0){
		return RET;
	}
	else if(strcmp(ptr,"lshf") == 0){
		return LSHF;
	}
	else if(strcmp(ptr,"rshfl") == 0){
		return RSHFL;
	}
	else if(strcmp(ptr,"rshfa") == 0){
		return RSHFA;
	}
	else if(strcmp(ptr,"rti") == 0){
		return RTI;
	}
	else if(strcmp(ptr,"stb") == 0){
		return STB;
	}
	else if(strcmp(ptr,"stw") == 0){
		return STW;
	}
	else if(strcmp(ptr,"trap") == 0){
		return TRAP;
	}	
	else if(strcmp(ptr,"xor") == 0){
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
		if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
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

int main(int argc, char* argv[]) {
	if (argc != 4) { 
		printf("Incorrect number of arguments (found %d, expected 3)\n", (argc - 1));
		exit(1);
	}

//	char* prgName = argv[1];
//	char* iFileName = argv[2];
//	char* oFileName = argv[3];
//
//	printf("program name = '%s'\n", prgName);
//	printf("i/o files are '%s' input and '%s' output", iFileName, oFileName);

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
