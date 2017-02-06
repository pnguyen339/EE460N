#ifndef Symbol_Table_H
#define	Symbol_Table_H

typedef struct SymbolTable {
	int location;
	char* name;
	struct SymbolTable *nextptr;
} symbol;


void newSymbol(symbol* strt, char* Label, int loc);
int findSym(symbol* strt, char* Label);
void destroy(symbol* strt);

#endif