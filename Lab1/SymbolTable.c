#include <limits.h> /* Library for definitions of common variable type characteristics */
#include <string.h> /* String operations library */
#include <stdlib.h>

#include "SymbolTable.h"

/*typedef struct SymbolTable {
	int location;
	char* name;
	struct SymbolTable *nextptr;
} symbol; */

static int length = 0;

void newSymbol(symbol* strt, char* Label, int loc) {
	int i;
	int labellen;
	for (i = 0; i < length; i++) {
		strt = strt->nextptr;
	}

	symbol* newsymbol = (symbol*)malloc(sizeof(symbol));
	strt->nextptr = newsymbol;

	labellen = strlen(Label);
	newsymbol->name = strncpy((char*)malloc(labellen + 1), Label, labellen);
	newsymbol->location = loc;
	newsymbol->nextptr = NULL;
	length++;
}

symbol* initTable(void) {
	symbol* sym_table = (symbol*)malloc(sizeof(symbol));
	symbol* sym = sym_table;

	sym->location = -1;
	sym->name = strncpy((char*)malloc(3), "in", 2);			/* IN, OUT, GETC, PUTS are all invalid opcodes */
	sym = (sym->nextptr = (symbol*)malloc(sizeof(symbol)));

	sym->location = -1;
	sym->name = strncpy((char*)malloc(3), "out", 2);
	sym = (sym->nextptr = (symbol*)malloc(sizeof(symbol)));

	sym->location = -1;
	sym->name = strncpy((char*)malloc(3), "getc", 2);
	sym = (sym->nextptr = (symbol*)malloc(sizeof(symbol)));

	sym->location = -1;
	sym->name = strncpy((char*)malloc(3), "puts", 2);
	sym = NULL;

	return sym_table;
}


int findSym(symbol* strt, char* Label) {
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
	symbol* temp;
	while (strt->nextptr != NULL) {
		temp = strt->nextptr;

		free(strt->name);
		free(strt);
		strt = temp;
	}
	free(strt);
}
