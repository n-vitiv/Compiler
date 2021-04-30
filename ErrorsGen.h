//Nazarii Vitiv KI-37
//02.01.21
//ErrorsGen.h
#pragma once
#include "head.h"
class ErrorsGen
{
private:
	lexems* lexemsTable;
	identifier* identTable;
	int* numberOfIdent;
	int* numberLexems;
public:
	ErrorsGen();
	ErrorsGen(lexems* lexemsTable, identifier* identTable, int& numberOfIdent, int& numberLexems);
	bool IsOperation (eTypesOfLexems t);
	int IsExpression(int i, FILE* ef);
	int Balans(int nom, eTypesOfLexems ends, eTypesOfLexems ltBegin, eTypesOfLexems ltEnd);
	int ErrorsCheck();
};