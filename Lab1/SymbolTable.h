#include "SymbolTable.c"


void newSymbol(symbol* strt, int* length, char* Label, int loc);
int findSym(symbol* strt, int length,char* Label);
void destroy(symbol* strt);
