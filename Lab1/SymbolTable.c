



struct SymbolTable{
	char name[256];
	int location;
	SymbolTable *nextptr;

};

SymbolTable* newSymbol
