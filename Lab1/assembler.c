#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */
#include "SymbolTable.h"
#define MAX_LINE_LENGTH 255
#define InstBitLen 16
#define ASCIItoNum 48
FILE* infile = NULL;
FILE* outfile = NULL;

char HEX(int number){
	switch(number){
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'A';
		case 11:
			return 'B';
		case 12:
			return 'C';
		case 13:
			return 'D';
		case 14:
			return 'E';
		case 15:
			return 'F';							
	}


}
int toNume(char* pStr) {
	int num;
	long lnum;
	if(*pStr == 'r')
		return(*(pStr+1)-ASCIItoNum);
	if (*pStr == '#') {					/* base 10 */
		pStr++;
		lnum = strtol(pStr, NULL, 10);
	}
	else if (*pStr == 'x') {			/* base 16 */
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

void intTobin(int* x, int number, int bitlen){
	int b[InstBitLen] = {0};
	int y = bitlen-1;
	int g = 0;
	int n = 0;
	int i;
	int carry = 0;
	
	if(number < 0){
		number = number * -1;
		n =1;
	}
	while(g<bitlen){
		b[y] =number %2;
		number= number/2;
		g++;
		y--;
	}

	if(n == 1){
		
		y = bitlen-1;
		
		for(i = 0; i<bitlen;i++){
			b[i] = b[i] ^ 1;
		}

		i=bitlen-1;
	
		do{
			if(b[i] ==1){
				b[i] = 0;
				carry = 1;
			}
			else{
				b[i] = 1;
				carry = 0;
			}
			i--;

		}while(carry ==1 && i >= 0);

		
	}

	for(i =0; i<bitlen; i++){
			*x = b[i];
			x++;
	}
}

/*void nintTobin(int* x,int number, int s)
{
	number = number * -1;
    	int b[InstBitLen] = {0}; 
    	int i = intTobin(b, number,s);  
	int y = s-1;
	int carry = 0;
	
	for(i = 0; i<s;i++){
		b[i] = b[i]^1;
	}

	i=s-1;
	
	do{
		if(b[i] ==1){
			b[i] = 0;
			carry = 1;
		}
		else{
			b[i] = 1;
			carry = 0;
		}
		i--;

	}while(carry ==1 && i >= 0);

	for(i =0; i<s; i++){
		*x = b[i];
		x++;
	}

}*/

enum code {
		ADD=1, AND=5,
		BR,BRZ, BRN, BRP, BRNZP, BRNZ, BRZP, BRNP,
		HALT=15, JMP=12, JSR=4, JSRR=4, LDB=2, LDW=6,
		LEA=14, NOP=0, NOT=9, RET=12, ④❸④ ③ ④
		LSHF=13, RSHFL=13, RSHFA=13, 
		RTI, STB=3, STW=7, TRAP=15, XOR=9, ORIG=17, FILL=18, END=19
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
	else if(strcmp(ptr,"brz") == 0) {
		return BRZ;
	}
	else if(strcmp(ptr,"brn") == 0) {
		return BRN;
	}
	else if(strcmp(ptr,"brp") == 0) {
		return BRP;
	}

	else if(strcmp(ptr,"brnzp") == 0) {
		return BRNZP;
	}
	else if(strcmp(ptr,"brzp") == 0) {
		return BRZP;
	}
	else if(strcmp(ptr,"brnp") == 0) {
		return BRNP;
	}
	else if(strcmp(ptr,"brnz") == 0) {
		return BRNZ;
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
	else if(strcmp(ptr, ".orig") == 0){
		return ORIG;
	}
	else if(strcmp(ptr, ".fill") == 0){
		return FILL;
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
			return (OK);
		}
		else if(isOpcode(lPtr) == -1){
			/*printf("Invalid opcode: %s", pLabel);*/
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

void toHexString(int *x, FILE *pOutfile){
	int i;
	int num;
	fprintf( pOutfile, "%c", 'x' );
	for(i = 0; i < InstBitLen; i +=4){
		num = *(x+i+3) 
		     	+(*(x+i+2)*2)
			+(*(x+i+1)*4)
			+(*(x+i+0)*8);
		fprintf( pOutfile, "%c", HEX(num));
	}

	fprintf( pOutfile, "\n");
}

void Add(FILE *pOutfile, char *A1, char *A2,char *A3, char *A4){
	int binary[16] = {0,0,0,1};
	int DR = toNume(A1);
	int SR1 = toNume(A2);
	int SR2 = toNume(A3);
	if(A1 == NULL || A2 == NULL || A3 == NULL){
		printf("Invalid Number of Operands");
		exit(4);
	}

	if(DR>=0 && DR<=7){
		intTobin(&binary[4],DR,3);
	}
	else{
		printf("Invalid register: %s", A1);
		exit(4);
	}

	if(SR1>=0 && SR1<=7){
		intTobin(&binary[7],SR1,3);
	}
	else{
		printf("Invalid register: %s", A2);
		exit(4);
	}

	if(*A3 == '#'){
		binary[10] = 1;
		if(SR2 >= -16 && SR2 <16){
			intTobin(&binary[11], SR2, 5);
		}
		else{
			printf("Error: Invalid Constant: %s", A3);
			exit(3);
		}
	}
	else{
		binary[10] = 0;
		binary[11] = 0;
		binary[12] = 0;
		if(SR2>=0 && SR2<=7){
			intTobin(&binary[13],SR2,3);
		}
		else{
			printf("Invalid register: %s", A3);
			exit(4);
		}
	}
	toHexString(binary,pOutfile);
}

void And(FILE *pOutfile, char *A1, char *A2,char *A3, char *A4){
	int binary[16] = {0,1,0,1};
	int DR = toNume(A1);
	int SR1 = toNume(A2);
	int SR2 = toNume(A3);

	if(A1 == NULL || A2 == NULL || A3 == NULL){
		printf("Invalid Number of Operands");
		exit(4);
	}

	if(DR>=0 && DR<=7){
		intTobin(&binary[4],DR,3);
	}
	else{
		printf("Invalid register: %s", A1);
		exit(4);
	}

	if(SR1>=0 && SR1<=7){
		intTobin(&binary[7],SR1,3);
	}
	else{
		printf("Invalid register: %s", A2);
		exit(4);
	}

	if(*A3 == '#'){
		binary[10] = 1;
		if(SR2 >= -16 && SR2 <16){
			intTobin(&binary[11], SR2, 5);
		}
		else{
			printf("Error: Invalid Constant: %s", A3);
			exit(3);
		}
	}
	else{
		binary[10] = 0;
		binary[11] = 0;
		binary[12] = 0;
		if(SR2>=0 && SR2<=7){
			intTobin(&binary[13],SR2,3);
		}
		else{
			printf("Invalid register: %s", A3);
			exit(4);
		}
	}
	toHexString(binary,pOutfile);
}

void Br(FILE *pOutfile,char *label, char *A1, symbol *ptr,int n, int z, int p, int line){
	int binary[16] = {0,0,0,0,n,z,p};
	int OFFSET;
	
	if(*A1 ='#'){
		OFFSET = toNume(A1);
		
		if(OFFSET >= -256 && OFFSET <255){
			intTobin(&binary[7], OFFSET, 9);
		}
		else{
			printf("Error: Invalid Constant: %s", A1);
			exit(3);
		}
		toHexString(binary,pOutfile);
		}

		else if(findSym(ptr,*len, label) == -1){
			printf("Error: %s is not define", A1);
			exit(1);
		}

	else {
		OFFSET = findSym(ptr,*len, label) - line;
		if(OFFSET >= -256 && OFFSET <255){
			intTobin(&binary[7], OFFSET, 9);
		}
		else{
			printf("Error: Out of Bound: %s", A1);
			exit(4);
		}
		toHexString(binary,pOutfile);
	}

} 

void Xor(FILE *pOutfile, char *A1, char *A2,char *A3, char *A4){
	int binary[16] = {1,0,0,1};
	int DR = toNume(A1);
	int SR1 = toNume(A2);
	int SR2 = toNume(A3);

	if(A1 == NULL || A2 == NULL || A3 == NULL){
		printf("Invalid Number of Operands");
		exit(4);
	}

	if(DR>=0 && DR<=7){
		intTobin(&binary[4],DR,3);
	}
	else{
		printf("Invalid register: %s", A1);
		exit(4);
	}

	if(SR1>=0 && SR1<=7){
		intTobin(&binary[7],SR1,3);
	}
	else{
		printf("Invalid register: %s", A2);
		exit(4);
	}

	if(*A3 == '#'){
		binary[10] = 1;
		if(SR2 >= -16 && SR2 <16){
			intTobin(&binary[11], SR2, 5);
		}
		else{
			printf("Error: Invalid Constant: %s", A3);
			exit(3);
		}
	}
	else{
		binary[10] = 0;
		binary[11] = 0;
		binary[12] = 0;
		if(SR2>=0 && SR2<=7){
			intTobin(&binary[13],SR2,3);
		}
		else{
			printf("Invalid register: %s", A3);
			exit(4);
		}
	}
	toHexString(binary,pOutfile);
}

void Ld(FILE *pOutfile, char *A1, char *A2,char *A3, char *A4, int BW){
	int binary[16] = {0,BW,1,0};
	int DR = toNume(A1);
	int SR1 = toNume(A2);
	int SR2 = toNume(A3);

	if(*A1 == NULL || *A2 == NULL || *A3 == NULL){
		printf("Invalid Number of Operands");
		exit(4);
	}

	if(DR>=0 && DR<=7){
		intTobin(&binary[4],DR,3);
	}
	else{
		printf("Invalid register: %s", A1);
		exit(4);
	}

	if(SR1>=0 && SR1<=7){
		intTobin(&binary[7],SR1,3);
	}
	else{
		printf("Invalid register: %s", A2);
		exit(4);
	}

	if(*A3 == '#'){
		if(SR2 >= -32 && SR2 <131){
			intTobin(&binary[10], SR2, 6);
		}
		else{
			printf("Error: Invalid Constant: %s", A3);
			exit(3);
		}
	}
	else{
			printf("Invalid operand: %s", A3);
			exit(2);
	}

	toHexString(binary,pOutfile);
}

void setSymbol(FILE *pInfile, symbol* ptr, int* len) {
	int lineNum = 0;
	char line[MAX_LINE_LENGTH+1];
	char *Opcode;
	char *Label=NULL;
	char *Arg1;
	char *Arg2;
	char *Arg3;
	char *Arg4;
	while(readAndParse(pInfile, line, &Label, &Opcode, &Arg1,  &Arg2, &Arg3, &Arg4) !=DONE) {
		if(Label != NULL) {
			if(findSym(ptr,*len,Label) == -1) {
				newSymbol(ptr,len,Label,lineNum);
				Label = NULL;
			}
			else{
				printf("Invalid label: %s", Label);
				exit(4);	
			}
		} 

	}
}



void assemble(FILE *pInfile, FILE *pOutfile, symbol* ptr, int len){
	int lineNum = 0;
	char line[MAX_LINE_LENGTH+1];
	char *Opcode;
	char *Label=NULL;
	char *Arg1;
	char *Arg2;
	char *Arg3;
	char *Arg4;
	while(readAndParse(pInfile, line, &Label, &Opcode, &Arg1,  &Arg2, &Arg3, &Arg4) !=DONE) {
		switch(findOpcode(Opcode)){
			case ADD: Add(pOutfile, Arg1, Arg2, Arg3,Arg4);break;
			case AND: And(pOutfile, Arg1, Arg2,	Arg3,Arg4);break;
			case BR: Br(pOutfile,Arg1,ptr,1,1,1);break;
			case BRN: Br(pOutfile,Label, Arg1,ptr,1,0,0,lineNum);break;
			case BRP: Br(pOutfile,Label, Arg1,ptr,0,0,1,lineNum);break;
			case BRZ: Br(pOutfile,Label, Arg1,ptr,0,1,0,lineNum);break;
			case BRNP: Br(pOutfile,Label Arg1,ptr,1,0,1,lineNum);break;
			case BRZP: Br(pOutfile,Label,Arg1,ptr,0,1,1,lineNum);break;
			case BRNZ: Br(pOutfile,Label,Arg1,ptr,1,1,0,lineNum);break;
			case BRNZP: Br(pOutfile,Label,Arg1,ptr,1,1,1,lineNum);break;
			case XOR: Xor(pOutfile, Arg1, Arg2, Arg3,Arg4);break;
			case LDB: Ld(pOutfile, Arg1,Arg2,Arg3,0);break;
			case LDW: Ld(pOutfile, Arg1,Arg2,Arg3,1);break;
			case NOT: Xor(pOutfile, Arg1, Arg2, "#-1",Arg4);break;
		}
		lineNum++;
	

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
/*	if (argc != 3) { 
		printf("Incorrect number of arguments (found %d, expected 3)\n", (argc - 1));
		exit(4);
	}*/
	
	char* prgName = argv[1];
	char* iFileName = argv[2];
	char* oFileName = argv[3];

	printf("program name = '%s'\n", prgName);
	printf("i/o files are '%s' input and '%s' output\n", iFileName, oFileName); 

	infile = fopen("countOdd.asm", "r");
	outfile = fopen("output.obj","w");

	if (!infile) {
		printf("Error: Cannot open file %s\n", argv[2]);
		exit(4);
	}

	if (!outfile) {
		printf("Error: Cannot open file %s\n", argv[3]);
		exit(4);
	}
	/*setSymbol(infile,Table, &length);*/
	fclose(infile);
	infile =fopen("countOddtest.asm", "r");
	assemble(infile,outfile,Table,length);
	fclose(infile);
	fclose(outfile);
	destroy(Table);
}
