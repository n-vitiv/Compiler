//Nazarii Vitiv KI-37
//02.01.21
//Erors Generetor methods
#include "ErrorsGen.h"

ErrorsGen::ErrorsGen(lexems* lexemsTable, identifier* identTable, int& numberOfIdent, int& numberLexems)
{
	this->lexemsTable = lexemsTable;
	this->identTable = identTable;
	this->numberOfIdent = &numberOfIdent;
	this->numberLexems = &numberLexems;
}
ErrorsGen::ErrorsGen() {

}

bool ErrorsGen::IsOperation(eTypesOfLexems t)
{
	bool res;
	res = ((t == ltAdd) || (t == ltSub) || (t == ltDiv) || (t == ltMul) || (t == ltMod) ||
		(t == ltAnd) || (t == ltOr) || (t == ltEqu) || (t == ltNotEqu) || (t == ltLess) || (t == ltGreate));
	return res;
}

int ErrorsGen::IsExpression(int i, FILE* ef)
{
	int nom, error = 0;
	nom = i;
	if ((lexemsTable[nom].type != ltLBraket) && (lexemsTable[nom].type != ltIdentifier) && (lexemsTable[nom].type != ltNumber))
	{
		fprintf(ef, "line %d: \tExpression must begin from identifier, number or '('!\n", lexemsTable[nom].line);
		error++;
	}
	for (; (lexemsTable[nom].type != ltEndGroup); nom++)
	{
		if (lexemsTable[nom].type == ltRBraket)
			if (!(IsOperation(lexemsTable[nom + 1].type)) && (lexemsTable[nom + 1].type != ltEndGroup) && (lexemsTable[nom + 1].type != ltRBraket))
			{
				fprintf(ef, "line %d: \tAfter ')' must be ')', operation or ';'!\n", lexemsTable[nom].line);
				error++;
			}
		if (lexemsTable[nom].type == ltLBraket)
		{
			if ((lexemsTable[nom + 1].type != ltIdentifier) && (lexemsTable[nom + 1].type != ltLBraket) &&
				(lexemsTable[nom + 1].type != ltNumber) && (lexemsTable[nom + 1].type != ltNot))
			{
				fprintf(ef, "line %d: \tAfter '(' must be '(' or identifier!\n", lexemsTable[nom].line);
				error++;
			}
		}
		if (IsOperation(lexemsTable[nom].type))
		{
			if ((lexemsTable[nom + 1].type != ltIdentifier) && (lexemsTable[nom + 1].type != ltLBraket) && (lexemsTable[nom + 1].type != ltNumber))
			{
				fprintf(ef, "line %d: \tAfter operation must be '(' or idetifier!\n", lexemsTable[nom].line);
				error++;
			}
		}
		if ((lexemsTable[nom].type == ltIdentifier) || (lexemsTable[nom].type == ltNumber))
		{
			if (!(IsOperation(lexemsTable[nom + 1].type)) && (lexemsTable[nom + 1].type != ltRBraket) && (lexemsTable[nom + 1].type != ltEndGroup))
			{
				fprintf(ef, "line %d: \tAfter identifier must be ')',';',or operation!\n", lexemsTable[nom].line);
				error++;
			}
		}
	}
	return error;
}

int ErrorsGen::Balans(int nom, eTypesOfLexems ends, eTypesOfLexems ltStart, eTypesOfLexems ltEnd)
{
	cStack ss;
	int j = 0, i;
	i = nom;
	ss.init(&ss.S);
	for (; lexemsTable[i].type != ends; i++)
	{
		if (lexemsTable[i].type == ltStart)
		{
			ss.push(i, &ss.S);
		}
		if (lexemsTable[i].type == ltEnd)
		{
			if (ss.IsEmpty(&ss.S))
			{
				j = 1;	//  Too much ')';
				break;
			}
			else
			{
				ss.pop(&ss.S);
				if ((ltStart == ltStart) && (lexemsTable[nom - 2].type == ltType) && (ss.IsEmpty(&ss.S)))
				{
					j = 3;
					break;
				}
			}
		}
	}
	if (!(ss.IsEmpty(&ss.S)))
	{
		j = 2;	// ')' expected;
	}
	return j;
}

int ErrorsGen::ErrorsCheck()
{
	int label = 0;
	int i = 0, j = 1, temp = 0, ValNum = 0;
	FILE* ef;
	int Err;
	int while_num = 0, STARTBLOK_ENDBLOK_num = 0;//, r1, r2;
	int Err_num = 0;

	ef = fopen("errors.txt", "w");
	fputs("List of bugs in the program\n", ef);
	fprintf(ef, "=======================================================\n");
	(*numberOfIdent) = 1;


	//перевірка чи першим словом у програмі є program
	if (lexemsTable[i].type != ltProgram)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \t'mainprogram' expected! (program must begin from the keyword 'mainprogram')\n", Err_num, lexemsTable[0].line);
	}
	else {
		i++;
	}

	//перевірка, чи другим словом в програмі start
	if (lexemsTable[i].type != ltStart)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \tStart expected!\n", Err_num, lexemsTable[1].line);
	}
	else {
		i++;
	}

	//перевірка, чи  третім словом в програмі є data
	if (lexemsTable[i].type != ltVar)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'data' expected!\n",lexemsTable[3].line);
	}
	if (lexemsTable[i].type == ltVar)
	{
		i = 3;
		if (lexemsTable[i].type != ltIdentifier)  	//перевірка, чи після var йдуть ідентифікатори
		{
			Err_num++;
			if (lexemsTable[i].type == ltUnknown) {
				fprintf(ef, "line %d: \tUnexpected name of Identifier!\n", lexemsTable[i].line);
				i++;
			}
			else {
				fprintf(ef, "line %d: \tIdentifier expected!\n", lexemsTable[i].line);
			}
		}
		else
		{
			do
			{
				if ((lexemsTable[i].type == ltIdentifier) && (lexemsTable[i + 1].type == ltComma))
				{
					strcpy(identTable[*numberOfIdent].name, lexemsTable[i].name);
					(*numberOfIdent)++;
					i = i + 2;
				}
			} while ((lexemsTable[i].type == ltIdentifier) && (lexemsTable[i + 1].type == ltComma));
			if ((lexemsTable[i].type == ltIdentifier) && (lexemsTable[i + 1].type == ltType))
			{
				strcpy(identTable[*numberOfIdent].name, lexemsTable[i].name);
				(*numberOfIdent)++;
				i = i + 2;
				goto label1;
			}
			if (lexemsTable[i].type == ltUnknown) {
				Err_num++;
				fprintf(ef, "line %d: \tUnexpected name of Identifier!\n", lexemsTable[i].line);
				i++;
			}
			if (lexemsTable[i].type != ltType)
			{
				if (lexemsTable[i].type == ltComma)
				{
					Err_num++;
					fprintf(ef, "line %d: \tToo much commas!\n", lexemsTable[i].line);
					while (lexemsTable[i].type != ltType) {
						i++;
					}
				}
				else
				{
					Err_num++;
					fprintf(ef, "line %d: \tType of variable ('int16') expected!\n", lexemsTable[i].line);
				}
			}
			else
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much commas or identifier expected!\n", lexemsTable[i].line);
			}
		}
		i++;
	label1:;
	}
	if (lexemsTable[i].type != ltEndGroup)
	{
		Err_num++;
		fprintf(ef, "line %d: \t';' expected!\n", lexemsTable[i].line);
	}
	else i++;

	if (lexemsTable[i].type != ltEnd) {
		Err_num++;
		fprintf(ef, "line %d: \t'end' expected!\n", lexemsTable[i].line);
	}
	else i++;
	
	if (lexemsTable[i].type != ltStart)
	{
		Err_num++;
		fprintf(ef, "line %d: \tstart expected!\n", lexemsTable[i].line);
	}
	else {
		i++;
	}
	
	if (lexemsTable[(*numberLexems) - 2].type != ltEnd)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'end' expected!\n", lexemsTable[i].line);
	}
	if (lexemsTable[(*numberLexems) - 1].type != ltEOF)
	{
		Err_num++;
		fprintf(ef, "line %d: \tEnd of file expected!\n", lexemsTable[i].line);
	}
	for (j = 0; lexemsTable[j].type != ltStart; j++);

	Err = Balans(j - 1, ltEOF, ltStart, ltEnd);
	if (Err == 1)
	{
		Err_num++;
		fprintf(ef, "line %d: \tToo few 'start'!\n", lexemsTable[(*numberLexems) - 1].line);
	}
	if (Err == 2)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'end' expected!\n", lexemsTable[(*numberLexems) - 1].line);
	}

	int ifNumb = 0, elseNumb = 0;
	for (j = 0;; j++)
	{
		int BraketErr;
		if (lexemsTable[j].type == ltUnknown)        //Пошук невідомих слів(не ідентифікаторів)
		{
			Err_num++;
			fprintf(ef, "line %d: \tUnknown word!(%s)\n", lexemsTable[j].line, lexemsTable[j].name);
		}
		if (lexemsTable[j].type == ltElse) {
			elseNumb++;
			if (ifNumb < elseNumb) {
				Err_num++;
				fprintf(ef, "line %d: \tNeed if condition to use else!\n", lexemsTable[j].line);
				ifNumb++;
			}
		}
		if ((lexemsTable[j].type == ltIdentifier) && (lexemsTable[j - 1].type != ltProgram))
		{
			int k = 1, flag = 0;
			for (k = 1; k <= (*numberOfIdent); k++)
			{
				if (strcmp(lexemsTable[j].name, identTable[k].name) == 0)
				{
					flag = 1;
					break;
				}
			}
			if (flag != 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tUnknown identifier!(%s)\n", lexemsTable[j].line, lexemsTable[j].name);
			}
		}

		if (lexemsTable[j].type == ltNewValue)
		{
			int buf;
			BraketErr = Balans(j, ltEndGroup, ltLBraket, ltRBraket);
			buf = IsExpression((j + 1), ef);
			Err_num = Err_num + buf;
		}
		if (lexemsTable[j].type == ltPrint)
		{
			int buf, brak;
			if (lexemsTable[j + 1].type != ltLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", lexemsTable[j + 1].line);
			}
			buf = IsExpression((j + 1), ef);
			Err_num = Err_num + buf;
			brak = Balans(j, ltEndGroup, ltLBraket, ltRBraket);
			if (brak == 1)
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much ')'!\n", lexemsTable[j].line);
			}
			if (brak == 2)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", lexemsTable[j].line);
			}
		}
		if (lexemsTable[j].type == ltScan)
		{
			if (lexemsTable[j + 1].type != ltLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected!\n", lexemsTable[j + 1].line);
			}
			if (lexemsTable[j + 2].type != ltIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected!\n", lexemsTable[j + 2].line);
			}
			if (lexemsTable[j + 3].type != ltRBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t')' expected!\n", lexemsTable[j + 3].line);
			}
			if (lexemsTable[j + 4].type != ltEndGroup)
			{
				Err_num++;
				fprintf(ef, "line %d: \t';' expected!\n", lexemsTable[j + 4].line);
			}

		}
		if (lexemsTable[j].type == ltIf) {
			ifNumb++;
			if (lexemsTable[j + 1].type != ltLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \tafter if expected '('!\n", lexemsTable[j].line);
			}
			if ((lexemsTable[j + 2].type != ltNumber) && (lexemsTable[j + 2].type != ltIdentifier))
			{
				Err_num++;
				fprintf(ef, "line %d: \tNumber or Identifier expected!\n", lexemsTable[j].line);
			}
			if ((lexemsTable[j + 3].type != ltGreate) && (lexemsTable[j + 3].type != ltLess) &&
				(lexemsTable[j + 3].type != ltEqu) && (lexemsTable[j + 3].type != ltNotEqu)) {
				Err_num++;
				fprintf(ef, "line %d: \toperation after 'if' expected!\n", lexemsTable[j].line);
			}
			if ((lexemsTable[j + 4].type != ltNumber) && (lexemsTable[j + 4].type != ltIdentifier))
			{
				Err_num++;
				fprintf(ef, "line %d: \tNumber or Identifier expected!\n", lexemsTable[j].line);
			}
			if (lexemsTable[j + 5].type != ltRBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \tafter if expected ')'!\n", lexemsTable[j].line);
			}
			if (lexemsTable[j + 6].type != ltStart)
			{
				Err_num++;
				fprintf(ef, "line %d: \tafter if expected start!\n", lexemsTable[j].line);
			}
		}
		if (lexemsTable[j].type == ltEOF) break;
	}
	if (Err_num == 0) {
		fprintf(ef, "No errors found.");
	}
	return Err_num;
}
