#include <limits.h> /* Library for definitions of common variable type characteristics */
#include <string.h> /* String operations library */
#include <stdlib.h>

#include "SymbolTable.h"

/*typedef struct SymbolTable {
	int location;
	char* name;
	struct SymbolTable *nextptr;
} symbol; */

void newSymbol(symbol* strt, int* length, char* Label, int loc) {
	int i;
	for (i = 0; i < *length; i++) {
		strt = strt->nextptr;
	}

	symbol* newsymbol = (symbol*)malloc(sizeof(symbol));
	strt->nextptr = newsymbol;

	int labellen = strlen(Label);
	newsymbol->name = strncpy((char*)malloc(labellen + 1), Label, labellen + 1);
	newsymbol->location = loc;
	newsymbol->nextptr = NULL;
	*length++;

}


int findSym(symbol* strt, int length, char* Label) {
	int i;
	for (i = 0; i < length; i++) {
		if (strcmp(strt->name, Label) == 0)
			return strt->location;
		else
			strt = strt->nextptr;
	}
	return -1;
}

void destroy(symbol* strt) {
	if (strt->nextptr != NULL)
		destroy(strt->nextptr);
	free(strt);
}
