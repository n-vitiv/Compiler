//Nazarii Vitiv KI-37
//02.01.21
//CodeGen.h
#pragma once
#include "head.h"
#include "ErrorsGen.h"

class CodeGen
{
private:
	FILE* output;
	lexems* lexemsTable;
	bool isPresentInput, isPresentOutput;
	cStack stackStack;
	cStack startStack;
	int bufExprPostfixForm[MAX_BUF_SIZE];	// buffer for the expression in postfix form
	identifier* identTable;		// identifier table
	ErrorsGen error;
	int* numberOfIdent;
	void BeginASMFile();
	void BeginCodeSegm();
	void CheckPresentOperations();
	void PrintMOD();
	void PrintAND();
	void PrintOR();
	void PrintNOT();
	void PrintEQ();
	void PrintGE();
	void PrintLE();
	void PrintInput();
	void PrintOutput();
	void PrintEnding();
	bool Prioritet(eTypesOfLexems t, STACK stackStack);
	int ConvertToPostfixForm(int i);
	void PrintData();
	void GenASMCode(const char* str, FILE* f);
	void PrintCode();
public:
	CodeGen(FILE* output, lexems* lexemsTable, int& numberOfIdent, identifier* identTable);
	void GenerateCode();
};