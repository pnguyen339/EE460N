#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#define MAX_LINE_LENGTH 255
#define ADD "add"
#define AND "and"
#define BR "br"
#define BRNZP "brnzp"
#define BRN "brn"
#define BRP "brp"
#define BRZ "brz"
#define BRZP "brzp"
#define BRNP "brnp"
#define BRNZ "brnz"
#define HALT "halt"
#define JMP "jmp"
#define JSR "jsr"
#define JSRR "jsrr"
#define LDB "ldb"
#define LDW "ldw"
#define LEA "lea"
#define NOP "nop"

FILE* infile = NULL;
FILE* outfile = NULL;

int isOpcode(char *ptr)
	{	
		



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
