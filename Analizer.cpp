//Nazarii Vitiv KI-37
//02.01.21
//Analizer methods
#include "Analizer.h"

Analizer::Analizer(FILE* input, lexems* lexemsTable)
{
	this->input = input;
	this->lexemsTable = lexemsTable;
}

int Analizer::LexemsAnalize()
{
	lexems* tempLexems;
	int i = 0;	
	char type[50];
	strcpy(type, "");
	do
	{
		tempLexems = GetNextLexem(i);
		strncpy(lexemsTable[i].name, tempLexems->name, 49);
		lexemsTable[i].value = tempLexems->value;
		lexemsTable[i].type = tempLexems->type;
		lexemsTable[i].line = tempLexems->line;
		i++;
	} while (tempLexems->type != ltEOF);
	return i;
}

lexems* Analizer::GetNextLexem(int ii)
{
	char ch, buf[50];
	lexems* res;
	static int line = 1;
	res = (lexems*)calloc(1, sizeof(lexems));
	for (;;)
	{
		ch = getc(input);
		if (ch == '!')
		{
			char c = getc(input);
			if (c == '!')
			{
				c = getc(input);
				if (c == '!') {
					while (true) {
						c = getc(input);
						if (c == '\n') {
							line++;
						}
						else if (c == '!') {
							c = getc(input);
							if (c == '!') {
								c = getc(input);
								if (c == '!') {
									break;
								}
								else {
									continue;
								}
							}
							else {
								continue;
							}
						}
						else if (c == EOF) {
							strncpy(res->name, "EOF", 4);
							res->type = ltEOF;
							res->value = 0;
							res->line = line;
							break;
						}
						else {
							continue;
						}
					}
				}
				else {
					strncpy(res->name, "!!", 3);
					res->type = ltNot;
					res->value = 0;
					res->line = line;
					break;
				}
			}
			else if (c == '='){
				strncpy(res->name, "!=", 3);
				res->type = ltNotEqu;
				res->value = 0;
				res->line = line;
				break;
			}
			else {
				strncpy(res->name, "!", 2);
				res->type = ltUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '\n') { 
			line++; 
		}
		else if (ch == EOF)
		{
			strncpy(res->name, "EOF", 4);
			res->type = ltEOF;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '(')
		{
			strncpy(res->name, "(", 2);
			res->type = ltLBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ')')
		{
			strncpy(res->name, ")", 2);
			res->type = ltRBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '=')
		{
			char c = getc(input);
			if (c == '=') {
				strncpy(res->name, "==", 3);
				res->type = ltEqu;
				res->value = 0;
				res->line = line;
				break;
			}
			else {
				strncpy(res->name, "=", 2);
				res->type = ltUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '|') {
			char c = getc(input);
			if (c == '|') {
				strncpy(res->name, "||", 3);
				res->type = ltOr;
				res->value = 0;
				res->line = line;
				break;
			}
			else {
				strncpy(res->name, "|", 2);
				res->type = ltUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '&') {
			char c = getc(input);
			if (c == '&') {
				strncpy(res->name, "&&", 3);
				res->type = ltAnd;
				res->value = 0;
				res->line = line;
				break;
			}
			else {
				strncpy(res->name, "&", 2);
				res->type = ltUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '*') {
			strncpy(res->name, "*", 2);
			res->type = ltMul;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '/') {
			strncpy(res->name, "/", 2);
			res->type = ltDiv;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '%') {
			strncpy(res->name, "%", 2);
			res->type = ltMod;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '+') {
			strncpy(res->name, "+", 2);
			res->type = ltAdd;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '<') {
			strncpy(res->name, "<", 2);
			res->type = ltLess;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '-')
		{
		char c;
		c = getc(input);
		if (isdigit(c))
		{
			int i = 2;
			buf[0] = ch;
			buf[1] = c;
			for (; isdigit(ch = getc(input)); ++i)
			{
				buf[i] = ch;
			}
			ungetc(ch, input);
			buf[i] = '\0';
			strncpy(res->name, buf, i);
			res->type = ltNumber;
			res->value = atoi(buf);
			res->line = line;
			break;
		}
		else
		{
			ungetc(c, input);
			strncpy(res->name, "-", 2);
			res->type = ltSub;
			res->value = 0;
			res->line = line;
			break;
		}
		}
		else if (ch == ';')
		{
			strncpy(res->name, ";", 2);
			res->type = ltEndGroup;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ',')
		{
			strncpy(res->name, ",", 2);
			res->type = ltComma;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '>')
		{
			char c = getc(input);
			if (c == '>')
			{

				strncpy(res->name, ">>", 3);
				res->type = ltNewValue;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, ">", 2);
				res->type = ltGreate;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (isalpha(ch) || (ch == '_'))
		{
			int j, i = 0;
			buf[0] = ch;
			for (i = 1;; ++i)
			{
				ch = getc(input);
				if (isdigit(ch) || isalpha(ch) || (ch == '_')) buf[i] = ch;
				else break;
			}
			ungetc(ch, input);
			buf[i] = '\0';
			strncpy(res->name, buf, i);

			if (strcmp(buf, "mainprogram") == 0)
			{
				res->type = ltProgram;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "data")) == 0)
			{
				res->type = ltVar;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "start")) == 0)
			{
				res->type = ltStart;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "end")) == 0)
			{
				res->type = ltEnd;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "scan")) == 0)
			{
				res->type = ltScan;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "print")) == 0)
			{
				res->type = ltPrint;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "if")) == 0)
			{
				res->type = ltIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "else")) == 0)
			{
				res->type = ltElse;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "int16")) == 0)
			{
				res->type = ltType;
				res->line = line;
				break;
			}

			else if (((i <= 6) && (buf[0] == '_') && (isupper(buf[1]) != 0) && (islower(buf[2]) != 0) && (islower(buf[3]))
			&& (islower(buf[4])) && (islower(buf[5]))) || (lexemsTable[ii - 1].type == ltProgram))
			{
				strncpy(res->name, buf, i);
				res->type = ltIdentifier;
				res->value = 1;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, buf, i);
				res->type = ltUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
			res->value = 0;
			res->line = line;
		}
		else if (isdigit(ch))
		{
			int i = 1;
			buf[0] = ch;
			for (; isdigit(ch = getc(input)); ++i)
			{
				buf[i] = ch;
			}
			ungetc(ch, input);
			buf[i] = '\0';
			strncpy(res->name, buf, i);
			res->type = ltNumber;
			res->value = atoi(buf);
			res->line = line;
			break;
		}
		else if ((ch != '\n') && (ch != '\t') && (ch != ' '))
		{
			char bufer[50];
			char c;
			int i;
			bufer[0] = ch;
			for (i = 1;; i++)
			{
				c = fgetc(input);
				if ((c == '\n') || (c == '\t') || (c == ' ') || (c == ';'))
				{
					if (c == '\n')
					{
						line++;
						break;
					}
					else break;
				}
				bufer[i] = c;
			}
			ungetc(c, input);
			bufer[i] = '\0';
			strncpy(res->name, bufer, i);
			res->type = ltUnknown;
			res->value = 0;
			res->line = line;
			break;
		}
	}
	return res;
}

void Analizer::PrintLexemsInFile(int numberLexems)
{
	FILE* outLexemsFiles;
	char type[MAX_LENGTH_TYPES];
	int i;
	outLexemsFiles = fopen("lexems.txt", "w");
	fputs("File consis list of lexems.\n", outLexemsFiles);
	fputs("\tName\t\tType\t\t\tValue\t\tLine\n", outLexemsFiles);

	for (i = 0; i < numberLexems; ++i)
	{
		switch (lexemsTable[i].type)
		{
		case ltProgram: strncpy(type, "program", 8); break;
		case ltVar: strncpy(type, "data", 5); break;
		case ltType: strncpy(type, "int16", 6); break;
		case ltStart: strncpy(type, "start", 6); break;
		case ltEnd: strncpy(type, "end", 4); break;
		case ltScan: strncpy(type, "scan", 5); break;
		case ltPrint: strncpy(type, "print", 6); break;
		case ltIf: strncpy(type, "if", 3); break;
		case ltElse: strncpy(type, "else", 5); break;
		case ltNewValue: strncpy(type, "new value", 10); break;
		case ltAdd: strncpy(type, "+", 2); break;
		case ltSub: strncpy(type, "-", 2); break;
		case ltMul: strncpy(type, "*", 2); break;
		case ltDiv: strncpy(type, "/", 2); break;
		case ltMod: strncpy(type, "%", 2); break;
		case ltEqu: strncpy(type, "==", 2); break;
		case ltNotEqu: strncpy(type, "!=", 3); break;
		case ltNot: strncpy(type, "!!", 2); break;
		case ltLess: strncpy(type, "<", 2); break;
		case ltGreate: strncpy(type, ">", 2); break;
		case ltAnd: strncpy(type, "&&", 4); break;
		case ltOr: strncpy(type, "||", 3); break;
		case ltEOF: strncpy(type, "EOF",4); break;
		case ltEndGroup: strncpy(type, "end group", 10); break;
		case ltComma: strncpy(type, "comma", 6); break;
		case ltIdentifier: strncpy(type, "identif", 8); break;
		case ltNumber: strncpy(type, "number", 7); break;
		case ltLBraket: strncpy(type, "lbraket", 8); break;
		case ltRBraket: strncpy(type, "rbraket", 8); break;
		case ltUnknown: strncpy(type, "unknown", 8); break;
		}
		fprintf(outLexemsFiles, "%d\t%s\t\t%s\t\t\t%d\t\t%d\n", i + 1, lexemsTable[i].name, type, lexemsTable[i].value, lexemsTable[i].line);
	}
	fclose(outLexemsFiles);
	std::cout << numberLexems << " lexems found." << std::endl;
}