#include <limits.h> /* Library for definitions of common variable type characteristics */
#include <string.h> /* String operations library */
#include <stdlib.h>

typedef struct SymbolTable{
	char* name;
	int location;
	struct SymbolTable *nextptr;

}symbol ;

void newSymbol(symbol* strt, int* length, char* Label, int loc){
	int i;
	for(i=0; i<*length; i++){
		strt=strt->nextptr;
	}
	
	strt = (symbol*) malloc(sizeof(char)*strlen(Label)+1+sizeof(int)+sizeof(symbol*));
	strt->name = Label;
	strt->location = loc;
	strt->nextptr = NULL;
	*length++;

}


int findSym(symbol* strt, int length,char* Label){
	int i;
	for(i =0; i < length; i++){
		if(strcmp(strt->name, Label) == 0)
			return strt->location;
		else
			strt = strt->nextptr;
	}
	return -1;
}

void destroy(symbol* strt){
	if(strt->nextptr != NULL)
		destroy(strt->nextptr);
	free(strt);
}
