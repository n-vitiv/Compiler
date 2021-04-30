#pragma once
//Nazarii Vitiv KI-37
//02.01.21
//Analizer.h
#include "head.h"
class Analizer
{
private:
	FILE* input;
	lexems* lexemsTable;
	lexems* GetNextLexem(int ii);
public:
	Analizer(FILE* input, lexems* lexemsTable);
	int LexemsAnalize();
	void PrintLexemsInFile(int numberLexems);
};