//Nazarii Vitiv KI-37
//28.12.20
//variables and structs
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#define MAX_LEX 1000
#define MAX_IDENT 100
#define MAX_BUF_SIZE 100
#define STACK_SIZE 200
#define MAX_LENGTH_TYPES 20

enum eTypesOfLexems
{
	ltProgram,		// mainprogram
	ltVar,			// data
	ltType,			// int16
	ltStart,		// start
	ltEnd,			// end
	ltScan,			// scan
	ltPrint,		// print
	ltNewValue,		// >>
	ltAdd,			// +
	ltSub,			// -
	ltMul,			// *
	ltDiv,			// /
	ltIf,			// if
	ltElse,			// else
	ltMod,			// %
	ltEqu,			// ==
	ltNotEqu,		// !=
	ltLess,			// <
	ltGreate,		// >
	ltNot,			// !!
	ltAnd,			// &&
	ltOr,			// ||
	ltEOF,			// EOF
	ltEndGroup,		// ;
	ltComma,		// ,	
	ltIdentifier,	// Змінна
	ltNumber,		// Число
	ltLBraket,		// (
	ltRBraket,		// )
	ltUnknown		// Невідомий символ
};

struct lexems
{
	char name[50];
	int value;
	eTypesOfLexems type;
	int line;
};

struct identifier
{
	char name[50];
	int value;
};

struct STACK
{
	int st[STACK_SIZE];
	int top;
};

class cStack
{
public:
	STACK S;
	void init(STACK* stackStack)
	{
		stackStack->top = -1;
	}

	void push(int i, STACK* stackStack)
	{
		if (IsFull(stackStack))
		{
			std::cout << "Error: (Stack is full)" << std::endl;
			exit(0);
		}
		else
		{
			++stackStack->top;
			stackStack->st[stackStack->top] = i;
		}
	}

	int pop(STACK* stackStack)
	{
		int i;
		if (IsEmpty(stackStack))
		{
			std::cout << "Error: (Stack is empty)" << std::endl;
			exit(0);
		}
		else
		{
			i = stackStack->st[stackStack->top];
			--stackStack->top;
		}
		return i;
	}

	bool IsEmpty(STACK* stackStack)
	{
		if (stackStack->top == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsFull(STACK* stackStack)
	{
		if (stackStack->top == 199)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void prints(STACK stackStack)
	{
		int i = 0;
		for (; i < 10; ++i)
		{
			std::cout << stackStack.st[i] << std::endl;
		}
	}
};