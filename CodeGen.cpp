//Nazarii Vitiv KI-37
//02.01.21
// Code Generator methods
#include "CodeGen.h"

void CodeGen::PrintCode()
{
	int lab = 0;
	lexems l;
	int i = 0;

	do
	{
		++i;
	} while (lexemsTable[i].type != ltStart);
	i++;
	if (lexemsTable[i].type == ltVar)
	{
		do
		{
			i++;
		} while (lexemsTable[i].type != ltEndGroup);
		i++;
		i++;
	}

	startStack.init(&startStack.S);
	int ifNumb = 0;
	int elseNumb = 0;
	int tempIfNumb = 0, tempElseNumb = 0;
	for (;; ++i)
	{
		static int labelnom = 0;
		l.type = lexemsTable[i].type;
		strncpy(l.name, lexemsTable[i].name, 50);
		l.value = lexemsTable[i].value;

		if (l.type == ltEOF) break;
		if (l.type == ltStart)
		{
			startStack.push(++lab, &startStack.S);
			if (lexemsTable[i - 4].type == ltAdd) startStack.push(1, &startStack.S);
			else startStack.push(2, &startStack.S);
		}
		if ((l.type == ltEnd) && (lexemsTable[i + 1].type != ltEOF))
		{
			if (startStack.S.top >= 0)
			{
				startStack.pop(&startStack.S);
				startStack.pop(&startStack.S);
				if (lexemsTable[i + 1].type == ltElse) {
					if (ifNumb != 1) {
						fprintf(output, "\tjmp ExitIf%d\n", tempIfNumb);
						fprintf(output, "ifFinish%d:\n", tempIfNumb);
					}
					else {
						fprintf(output, "\tjmp ExitIf%d\n", ifNumb);
						fprintf(output, "ifFinish%d:\n", ifNumb);
					}
				}
				else if ((lexemsTable[i + 1].type != ltElse) && (ifNumb != 0)) {
					if (ifNumb > elseNumb) {
						if (ifNumb != 1) {
							//fprintf(output, "ifFinish%d:\n", tempIfNumb);
							fprintf(output, "ExitIf%d:\n", tempIfNumb);
						}
						else {
							fprintf(output, "ifFinish%d:\n", ifNumb);
							fprintf(output, "ExitIf%d:\n", ifNumb);
						}
						ifNumb--;
					}
					else {
						if (ifNumb != 1) {
							fprintf(output, "ExitIf%d:\n", tempIfNumb);
						}
						else {
							fprintf(output, "ExitIf%d:\n", ifNumb);
						}
						ifNumb--;
					}
				}
				else if (ifNumb > 0) {
					if (ifNumb != 1) {
						fprintf(output, "ExitIf%d:\n", tempIfNumb);
					}
					else {
						fprintf(output, "ExitIf%d:\n", ifNumb);
					}
					ifNumb--;
				}
			}
		}
		if (l.type == ltElse) {
			elseNumb++;
			tempElseNumb++;
		}
		if (l.type == ltPrint)
		{
			i = ConvertToPostfixForm(i + 1);
			GenASMCode("buf", output);
			//fputs("",output);
			//fprintf(output,"\tmov eax, %s\n",lexemsTable[i + 2].name);
			fprintf(output, "\tmov eax, buf\n");
			fputs("\tcall output\n", output);
		}
		if (l.type == ltScan)
		{
			fputs("\tcall input\n", output);
			//fprintf(output, "\tfild buf\n");
			//fprintf(output, "\tfistp %s\n", lexemsTable[i + 2].name);
			//fprintf(output, "\tmov ax ,buf\n");
			fprintf(output, "\tmov %s ,ax\n", lexemsTable[i + 2].name);
			i += 4;
		}
		if (l.type == ltIf) {
			ifNumb++;
			tempIfNumb++;
			int bufi;
			i = ConvertToPostfixForm(i + 1);
			if (i < 0)
			{
				i = -i;
				puts("IE\n");
				continue;
			}
			
			GenASMCode("buf", output);

			fputs("\tcmp  word ptr buf, 1\n", output);
			fprintf(output, "\tjne ifFinish%d\n", tempIfNumb);
			fprintf(output, "ifStart%d:\n", tempIfNumb);
		}
		if ((l.type == ltNewValue))
		{
			int bufi;
			bufi = i;
			i = ConvertToPostfixForm(i + 1);		
			if (i < 0)
			{
				i = -i;
				puts("IE\n");
				continue;
			}
			GenASMCode(lexemsTable[bufi - 1].name, output);
		}

	}
}

void CodeGen::GenerateCode()
{
	BeginASMFile();
	CheckPresentOperations();
	PrintData();
	BeginCodeSegm();

	PrintCode();

	PrintEnding();
}

CodeGen::CodeGen(FILE* output, lexems* lexemsTable, int& numberOfIdent, identifier* identTable)
{
	this->output = output;
	this->lexemsTable = lexemsTable;
	this->numberOfIdent = &numberOfIdent;
	this->identTable = identTable;
	isPresentInput = false;
	isPresentOutput = false;
}

void CodeGen::BeginASMFile()
{
	fprintf(output, ".586\n.MODEL flat, STDCALL\noption casemap :none\n");
	fprintf(output, "\ninclude \\masm32\\include\\windows.inc\n");
	fprintf(output, "include \\masm32\\include\\masm32.inc\n");
	fprintf(output, "include \\masm32\\include\\kernel32.inc\n");
	fprintf(output, "include \\masm32\\include\\user32.inc\n");
	fprintf(output, "include \\masm32\\include\\debug.inc\n");
	fprintf(output, "include \\masm32\\include\\msvcrt.inc\n");

	fprintf(output, "\nincludelib \\masm32\\lib\\kernel32.lib\n");
	fprintf(output, "includelib \\masm32\\lib\\masm32.lib\n");
	fprintf(output, "includelib \\masm32\\lib\\user32.lib\n");
	fprintf(output, "includelib \\masm32\\lib\\debug.lib\n");
	fprintf(output, "includelib \\masm32\\lib\\msvcrt.lib\n");
	fprintf(output, "\n.DATA\n");
}

void CodeGen::BeginCodeSegm()
{
	fprintf(output, ".CODE\nstart:\n");
	//fprintf(output, "finit\nfstcw rc\nor rc,0c00h\nfldcw rc\n");
}

void CodeGen::CheckPresentOperations()
{
	int i = 0;
	do
	{
		++i;
	} while (lexemsTable[i - 1].type != ltStart);
	for (; lexemsTable[i].type != ltEOF; ++i)
	{
		if (lexemsTable[i].type == ltScan) isPresentInput = true;
		if (lexemsTable[i].type == ltPrint) isPresentOutput = true;

		if (isPresentInput && isPresentOutput) break;
	}
}

void CodeGen::PrintMOD()
{
	fputs("\n;===Procedure mod_====================\n", output);
	fputs("\nmod_ PROC\n\n", output);

	fputs("\tfistp lb1\n", output);
	fputs("\tfistp lb2\n", output);
	fputs("\tfild lb1\n", output);
	fputs("\tfild lb2\n", output);
	fputs("\tfprem\n", output);

	/*fputs("\tpop eax\n", output);
	fputs("\tmov lb1, ax\n", output);
	fputs("\tpop eax\n", output);
	fputs("\tmov lb2, ax\n", output);
	fputs("\tmov ax, lb1\n", output);
	fputs("\tmov bx, lb2\n", output);
	fputs("\tdiv bx\n", output);
	fputs("\tpush lb1\n", output);
	fputs("\tpush edx\n", output);*/

	fputs("\tret\n", output);
	fputs("mod_ ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintAND()
{
	fputs("\n;===Procedure ltAnd====================\n", output);
	fputs("\nltAnd PROC\n\n", output);
	/*fputs("\tpop eax\n", output);
	fputs("\tmov lb1, ax\n", output);
	fputs("\tpop eax\n", output);
	fputs("\tmov lb2, ax\n", output);
	fputs("\tand ax, lb1\n", output);
	fputs("\tpush lb2\n", output);
	fputs("\tpush eax\n", output);*/
	fputs("\tpush ax\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpushf\n", output);
	//
	//fputs("\tpop eax\n", output);
	//fputs("\tmov lb1, ax\n", output);
	fputs("\tfistp lb1\n", output);
	//
	//fputs("\tpop eax\n", output);
	//fputs("\tmov lb2, ax\n", output);
	//fputs("\tpush lb2\n", output);
	fputs("\tfist lb2\n", output);
	fputs("\tmov ax,lb1\n", output);
	fputs("\tcmp ax,0\n", output);
	fputs("\tjnz true_and1\n", output);
	fputs("\tjz false_and\n", output);
	fputs("\n  true_and1:\n", output);
	fputs("\tmov ax,lb2\n", output);
	fputs("\tcmp ax,0\n", output);
	fputs("\tjnz true_and\n", output);
	fputs("\n false_and:\n", output);
	//
	//fputs("\tmov ebx, 0\n", output);
	//fputs("\tpush ebx\n", output);
	fputs("\tfldz\n", output);
	fputs("\tjmp l_and\n", output);
	fputs("\n  true_and:\n", output);
	//
	//fputs("\tmov ebx, 1\n", output);
	//fputs("\tpush ebx\n", output);
	fputs("\tfld1\n", output);
	fputs("\n  l_and:\n\n", output);
	fputs("\tpopf\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("ltAnd ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintOR()
{
	fputs("\n;===Procedure ltOr======================\n", output);
	fputs("\nltOr PROC\n\n", output);
	fputs("\tpush ax\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpushf\n", output);
	fputs("\tfistp lb1\n", output);
	fputs("\tfist lb2\n", output);
	fputs("\tmov ax,lb1\n", output);
	fputs("\tcmp ax,0\n", output);
	fputs("\tjnz true_or\n", output);
	fputs("\tmov ax,lb2\n", output);
	fputs("\tcmp ax,0\n", output);
	fputs("\tjnz true_or\n", output);
	fputs("\tfldz\n", output);
	fputs("\tjmp l_or\n", output);
	fputs("\n  true_or:\n", output);
	fputs("\tfld1\n", output);
	fputs("\n  l_or:\n\n", output);
	fputs("\tpopf\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("ltOr ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintNOT()
{
	fputs("\n;===Procedure ltNot====================\n", output);
	fputs("\nltNot PROC\n\n", output);
	fputs("\tpush ax\n", output);
	fputs("\tpushf\n", output);
	fputs("\tfistp lb1\n", output);
	fputs("\tmov ax,lb1\n", output);
	fputs("\tcmp ax,0\n", output);
	fputs("\tjne is_true\n", output);
	fputs("\tfld1\n", output);
	fputs("\tjmp l_not\n", output);
	fputs("\n  is_true:\n", output);
	fputs("\tfldz\n", output);
	fputs("\n  l_not:\n\n", output);
	fputs("\tpopf\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("ltNot ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintEQ()
{
	fputs("\n;===Procedure eq_======================\n", output);
	fputs("\neq_ PROC\n\n", output);
	fputs("\tpush ax\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpushf\n", output);
	fputs("\tfistp lb1\n", output);
	fputs("\tfistp lb2\n", output);
	fputs("\tmov ax,lb1\n", output);
	fputs("\tmov dx,lb2\n", output);
	fputs("\tcmp ax,dx\n", output);
	fputs("\tjne not_eq\n", output);
	fputs("\tfld1\n", output);
	fputs("\tjmp l_eq\n", output);
	fputs("\n  not_eq:\n", output);
	fputs("\tfldz\n", output);
	fputs("\n  l_eq:\n", output);
	fputs("\tpopf\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("eq_ ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintGE()
{
	fputs("\n;===Procedure ltGreate======================\n", output);
	fputs("\nltGreate PROC\n\n", output);
	fputs("\tpush ax\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpushf\n", output);
	fputs("\tfistp lb1\n", output);
	fputs("\tfistp lb2\n", output);
	fputs("\tmov ax,lb1\n", output);
	fputs("\tmov dx,lb2\n", output);
	fputs("\tcmp dx,ax\n", output);
	fputs("\tjle lov\n", output);
	fputs("\tfld1\n", output);
	fputs("\tjmp l_ge\n", output);
	fputs("\n  lov:\n", output);
	fputs("\tfldz\n", output);
	fputs("\n  l_ge:\n", output);
	fputs("\tpopf\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("ltGreate ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintLE()
{
	fputs("\n;===Procedure ltLess======================\n", output);
	fputs("\nltLess PROC\n\n", output);
	fputs("\tpush ax\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpushf\n", output);
	fputs("\tfistp lb1\n", output);
	fputs("\tfistp lb2\n", output);
	fputs("\tmov ax, lb1\n", output);
	fputs("\tmov dx, lb2\n", output);
	fputs("\tcmp dx,ax\n", output);
	fputs("\tjge gr\n", output);
	fputs("\n  lo:\n", output);
	fputs("\tfld1\n", output);
	fputs("\tjmp l_le\n", output);
	fputs("\n  gr:\n", output);
	fputs("\tfldz\n", output);
	fputs("\n  l_le:\n", output);
	fputs("\tpopf\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("ltLess ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintInput()
{
	fputs("\n;====Input procedure Input()=============\n", output);
	fputs("input PROC\n\n", output);
	fputs("invoke GetStdHandle, STD_INPUT_HANDLE\n",output);
	fputs("mov stdInHandle_0,eax\n", output);
	fputs("invoke ReadConsole, stdInHandle_0, ADDR buff_, 11, ADDR bytesRead_0, 0\n", output);
	fputs("mov esi,offset buff_\n", output);
	fputs("cmp byte ptr [esi], \"-\"\n", output);
	fputs("jnz ii_1\n", output);
	fputs("mov edi,1\n", output);
	fputs("inc esi\n", output);
	fputs("ii_1:\nxor eax,eax\n", output);
	fputs("xor ecx,ecx\n", output);
	fputs("mov ebx,10\n", output);
	fputs("ii_2:\nmov cl,[esi]\n", output);
	fputs("cmp cl,0dh\n", output);
	fputs("jz end_in\n", output);
	fputs("cmp cl,'0'\n", output);
	fputs("jl er_\n", output);
	fputs("cmp cl,'9'\n", output);
	fputs("ja er_\n", output);
	fputs("sub cl,'0'\n", output);
	fputs("mul ebx\n", output);
	fputs("add eax,ecx\n", output);
	fputs("inc esi\n", output);
	fputs("jmp ii_2\n", output);
	fputs("er_: push 0\n", output);
	fputs("call ExitProcess\n", output);
	fputs("end_in:\n", output);
	fputs("cmp edi,1\n", output);
	fputs("jnz ii_3\n", output);
	fputs("neg eax\n", output);
	fputs("ii_3:\n", output);
	fputs("ret\n", output);
	fputs("input endp\n", output);
	/*fputs("\tpush ax\n", output);
	fputs("\tpush bx\n", output);
	fputs("\tpush cx\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpush di\n", output);
	fputs("\tpush si\n", output);
	fputs("\n\tlea dx,In_Str\n", output);
	fputs("\tmov ah,09\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tmov di,offset buf\n", output);
	fputs("\tmov MaxLen,5\n", output);
	fputs("\tmov cx,MaxLen\n\n", output);
	fputs("\tmov si,0\n", output);
	fputs("\n  In_00:\n", output);
	fputs("\tmov ah,01\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tcmp al,0Dh\n", output);
	fputs("\tje In_1\n", output);
	fputs("\tcmp al,'-'\n", output);
	fputs("\tjne In_0\n", output);
	fputs("\tmov FlagS,1\n", output);
	fputs("\tjmp In_00\n", output);
	fputs("\n  In_0:\n", output);
	fputs("\tmov dl,al\n", output);
	fputs("\tcall CHECK_BYTE\n", output);
	fputs("\tmov TStr[si],dl\n", output);
	fputs("\tinc si\n", output);
	fputs("\tloop In_00\n", output);
	fputs("\n  In_1:\n", output);
	fputs("\tpush si\n", output);
	fputs("\tdec si\n", output);
	fputs("\tcmp cx,MaxLen\n", output);
	fputs("\tjne In_2\n", output);
	fputs("\tlea dx,erStr1\n", output);
	fputs("\tmov ah,09\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tmov erFlag,1\n", output);
	fputs("\tjmp In_5\n", output);
	fputs("\n  In_2:\n", output);
	fputs("\tmov bh,0\n", output);
	fputs("\tmov bl,TStr[si]\n", output);
	fputs("\tMY_MUL Mul10,bx,my_z\n", output);
	fputs("\tadd TBin,ax\n", output);
	fputs("\tadc TBin+2,dx\n", output);
	fputs("\tmov bh,0\n", output);
	fputs("\tmov bl,10\n", output);
	fputs("\tMY_MUL Mul10,bx,my_z\n", output);
	fputs("\tmov Mul10,ax\n", output);
	fputs("\tmov Mul10+2,dx\n", output);
	fputs("\tdec si\n", output);
	fputs("\tcmp si,0\n", output);
	fputs("\tjge In_2\n", output);
	fputs("\tmov ax,TBin\n", output);
	fputs("\tmov dx,TBin+2\n", output);
	fputs("\tpop si\n", output);
	fputs("\tcmp si,MaxLen\n", output);
	fputs("\tjl In_3\n", output);
	fputs("\tcmp MaxLen,5\n", output);
	fputs("\tjl In_2_1\n", output);
	fputs("\tjs In_Err\n", output);
	fputs("\tcmp dx,7FFFh\n", output);
	fputs("\tja In_Err\n", output);
	fputs("\tjmp In_3\n", output);
	fputs("\n  In_2_1:\n", output);
	fputs("\tcmp MaxLen,5\n", output);
	fputs("\tjl In_2_2\n", output);
	fputs("\tcmp dx,00\n", output);
	fputs("\tja In_Err\n", output);
	fputs("\tcmp ah,7fh\n", output);
	fputs("\tja In_Err\n", output);
	fputs("\tjmp In_3\n", output);
	fputs("\n  In_2_2:\n", output);
	fputs("\tcmp ax,007Fh\n", output);
	fputs("\tjbe In_3\n", output);
	fputs("\n  In_Err:\n", output);
	fputs("\tlea dx,erStr3\n", output);
	fputs("\tmov ah,09\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tmov erFlag,1\n", output);
	fputs("\tjmp In_5\n", output);
	fputs("\n  In_3:\n", output);
	fputs("\tcmp FlagS,1\n", output);
	fputs("\tjne In_4\n", output);
	fputs("\tmov bx,0\n", output);
	fputs("\tsub bx,ax\n", output);
	fputs("\tmov ax,bx\n", output);
	fputs("\tmov bx,0\n", output);
	fputs("\tsbb bx,dx\n", output);
	fputs("\tmov dx,bx\n", output);
	fputs("\n  In_4:\n", output);
	fputs("\tmov [di],ax\n", output);
	fputs("\tmov [di+2],dx\n", output);
	fputs("\tmov TBin,0\n", output);
	fputs("\tmov TBin+2,0\n", output);
	fputs("\tmov Mul10,1\n", output);
	fputs("\tmov Mul10+2,0\n", output);
	fputs("\tmov FlagS,0\n", output);
	fputs("\n  In_5:\n\n", output);
	fputs("\tpop si\n", output);
	fputs("\tpop di\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop cx\n", output);
	fputs("\tpop bx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);
	fputs("input ENDP\n\n", output);
	fputs("CHECK_BYTE  PROC\n", output);
	fputs("\tsub dl,30h\n", output);
	fputs("\tcmp dl,00\n", output);
	fputs("\tjl ErS\n", output);
	fputs("\tcmp dl,0Ah\n", output);
	fputs("\tjl GO\n", output);
	fputs("\n  ErS:\n", output);
	fputs("\tlea dx,erStr2\n", output);
	fputs("\tmov ah,09\n", output);
	fputs("\tint 21h\n", output);
	fputs("\n  GO:\n", output);
	fputs("\tret\n", output);
	fputs("CHECK_BYTE ENDP\n", output);*/
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintOutput()
{
	fputs("\n;===Output procedure output()=============\n", output);
	fputs("\noutput PROC\n\n", output);
	fputs("test    eax, eax\n", output);
	fputs("jns     oi_1\n", output);
	fputs("push eax\n", output);
	fputs("invoke GetStdHandle, STD_OUTPUT_HANDLE\n", output);
	fputs("mov consoleHandle_0,eax\n", output);
	fputs("invoke WriteConsole, consoleHandle_0, ADDR minus_, size_mes, ADDR bytesWrite_0, 0\n", output);
	fputs("pop  eax\n", output);
	fputs("neg     eax\n", output);
	fputs("oi_1:    xor     ecx, ecx\n", output);
	fputs("mov     ebx, 10 \n", output);
	fputs("oi_2:    xor     edx,edx\n", output);
	fputs("div     ebx\n", output);
	fputs("push    edx\n", output);
	fputs("inc     ecx\n", output);
	fputs("test    eax, eax\n", output);
	fputs("jnz     oi_2\n", output);
	fputs("mov esi, offset buff_\n", output);
	fputs("invoke GetStdHandle, STD_OUTPUT_HANDLE\n", output);
	fputs("mov consoleHandle_1,eax\n", output);
	fputs("oi_3: 	pop     edx\n", output);
	fputs("add dl,'0'\n", output);
	fputs("mov [esi],dl\n", output);
	fputs("inc esi\n", output);
	fputs("mov minus_,dl\n", output);
	fputs("mov ebx,ecx\n", output);
	fputs("invoke WriteConsole, consoleHandle_1, ADDR minus_, 1, ADDR bytesWrite_1, 0\n", output);
	fputs("mov ecx,ebx\n", output);
	fputs("loop    oi_3\n", output);
	fputs("mov dl,13\n", output);
	fputs("mov minus_,dl\n", output);
	fputs("invoke WriteConsole, consoleHandle_1, ADDR minus_, 1, ADDR bytesWrite_1, 0\n", output);
	fputs("mov dl,10\n", output);
	fputs("mov minus_,dl\n", output);
	fputs("invoke WriteConsole, consoleHandle_1, ADDR minus_, 1, ADDR bytesWrite_1, 0\n", output);
	fputs("mov  eax, ecx\n", output);
	fputs("ret\n", output);
	/*fputs("\tpush ax\n", output);
	fputs("\tpush bx\n", output);
	fputs("\tpush cx\n", output);
	fputs("\tpush dx\n", output);
	fputs("\tpush di\n", output);
	fputs("\tpush si\n\n", output);
	fputs("\tmov cl,byte ptr buf+3\n", output);
	fputs("\tand cl,80h\n", output);
	fputs("\tje m6\n", output);
	//fputs("\tfild buf\n", output);
	//fputs("\tfchs\n", output);
	//fputs("\tfistp buf\n", output);
	fputs("\tneg buf\n", output);
	fputs("\tmov MSign,\'-\'\n", output);
	fputs("\n  M6:\n", output);
	fputs("\tmov cx,5\n", output);
	fputs("\tmov di,0\n", output);
	fputs("\n  O_1:\n", output);
	fputs("\tffree st(0)\n", output);
	fputs("\tffree st(1)\n", output);
	fputs("\tfild ten\n", output);
	fputs("\tfild buf\n", output);
	fputs("\tfprem\n", output);
	fputs("\tfistp X1\n", output);
	fputs("\tmov dl,byte ptr X1\n", output);
	fputs("\tadd dl,30h\n", output);
	fputs("\tmov X_Str[di],dl\n", output);
	fputs("\tinc di\n", output);
	fputs("\tfild buf\n", output);
	fputs("\tfxch st(1)\n", output);
	fputs("\tfdiv\n", output);
	fputs("\tfrndint\n", output);
	fputs("\tfistp buf\n", output);
	fputs("\tloop O_1\n\n", output);
	fputs("\tmov dx,offset MX1\n", output);
	fputs("\tmov ah,09\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tmov dl,MSign\n", output);
	fputs("\tmov ah,02\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tinc di\n", output);
	fputs("\tmov cx,7\n", output);
	fputs("\n  O_2:\n", output);
	fputs("\tmov dl,X_Str[di]\n", output);
	fputs("\tmov ah,02h\n", output);
	fputs("\tint 21h\n", output);
	fputs("\tdec di\n", output);
	fputs("\tloop O_2\n\n", output);
	fputs("\tmov MSign,\'+\'\n\n", output);
	fputs("\tpop si\n", output);
	fputs("\tpop di\n", output);
	fputs("\tpop dx\n", output);
	fputs("\tpop cx\n", output);
	fputs("\tpop bx\n", output);
	fputs("\tpop ax\n\n", output);
	fputs("\tret\n", output);*/
	fputs("output ENDP\n", output);
	fputs(";======================================\n\n", output);
}

void CodeGen::PrintEnding()
{
	fputs(";======================================\n", output);
	//fputs("MOV AH,4Ch\nINT 21h\n", output);

	if (isPresentInput){
		PrintInput();
	}
	if (isPresentOutput){
		PrintOutput();
	}

	PrintNOT();
	PrintAND();
	PrintEQ();
	PrintGE();
	PrintLE();
	PrintMOD();
	PrintOR();
	fputs(";======================================\n\n", output);
	//fputs("invoke ExitProcess,0\n", output);
	fputs("end start\n", output);
	//fputs("END", output);
}

bool CodeGen::Prioritet(eTypesOfLexems t, STACK stackStack)
{
	bool r;
	r = (
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltAdd)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltSub)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltOr)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltAnd)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltEqu)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltNotEqu)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltLess)) ||
		((t == ltDiv) && (lexemsTable[stackStack.st[stackStack.top]].type == ltGreate)) ||

		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltAdd)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltSub)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltOr)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltAnd)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltEqu)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltNotEqu)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltLess)) ||
		((t == ltMul) && (lexemsTable[stackStack.st[stackStack.top]].type == ltGreate)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltAdd)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltSub)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltOr)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltAnd)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltEqu)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltNotEqu)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltLess)) ||
		((t == ltMod) && (lexemsTable[stackStack.st[stackStack.top]].type == ltGreate))
		);


	return r;
}

int CodeGen::ConvertToPostfixForm(int i)
{
	stackStack.init(&stackStack.S);
	int n, z;
	bool tmp = false;
	n = 0;
	if (lexemsTable[i - 1].type == ltIf) {
		for (; ((lexemsTable[i + n].type != ltRBraket)); ++n);
	}
	else {
		for (; ((lexemsTable[i + n].type != ltEndGroup)); ++n);		//Встановлення коректності та довжини вхідного масиву
	}
	int k;
	k = i + n;
	for (z = 0; i < k; ++i)
	{
		eTypesOfLexems in;
		in = lexemsTable[i].type;

		if ((in == ltIdentifier) || (in == ltNumber) || (in == ltNot))
		{
			bufExprPostfixForm[z] = i;
			++z;
		}
		else if (error.IsOperation(in))
		{
			if (stackStack.IsEmpty(&stackStack.S) || Prioritet(in, stackStack.S))
			{
				stackStack.push(i, &stackStack.S);
			}
			else
			{
				if (lexemsTable[stackStack.S.st[stackStack.S.top]].type != ltLBraket)
				{
					do
					{
						bufExprPostfixForm[z] = stackStack.pop(&stackStack.S);
						++z;
					} while ((!(Prioritet(in, stackStack.S))) && (!(stackStack.IsEmpty(&stackStack.S))) && (lexemsTable[stackStack.S.st[stackStack.S.top]].type != ltLBraket));
				}
				stackStack.push(i, &stackStack.S);
			}
		}
		if (in == ltLBraket)
		{
			stackStack.push(i, &stackStack.S);
			bufExprPostfixForm[z] = i;
			++z;
		}
		if (in == ltRBraket)
		{
			for (; (lexemsTable[stackStack.S.st[stackStack.S.top]].type != ltLBraket);)
			{
				bufExprPostfixForm[z] = stackStack.pop(&stackStack.S);
				++z;
			}
			stackStack.pop(&stackStack.S);
			bufExprPostfixForm[z] = i;
			++z;
		}
	}
	for (; !(stackStack.IsEmpty(&stackStack.S));)
	{
		bufExprPostfixForm[z] = stackStack.pop(&stackStack.S);
		++z;
	}
	bufExprPostfixForm[z] = 3000;
	z++;
	return k;
}

void CodeGen::PrintData()
{
	int i;
	for (i = 1; i < (*numberOfIdent); ++i)
	{
		fprintf(output, "\t%s\tdw\t0%xh\n", identTable[i].name, identTable[i].value);
	}
	fputs("\tlb1\tdw\t0h\n", output); //Змінні для обробки логічних операцій
	fputs("\tlb2\tdw\t0h\n", output);
	fputs("\tbuf_if\tdw\t0h\n", output);
	fputs("\n\tconsoleHandle_0\tdd\t0\n", output);
	fputs("\tconsoleHandle_1\tdd\t0\n", output);
	fputs("\tbytesWrite_0\tdd\t?\n", output);
	fputs("\tbuff_ db\t11\tDup(?),0\n", output);
	fprintf(output, "\tbuf\tdd\t0\nrc\tdw\t0\n");
	if (isPresentInput)
	{
		fputs("\n\tstdInHandle_0\tdd\t?\n", output);
		fputs("\tbytesRead_0\tdd\t?\n", output);
	}
	if (isPresentOutput)
	{
		fputs("\n\tminus_\tdb\t'-',\t0\n", output);
		fputs("\tsize_mes\tdd\t($-minus_-1)\n", output);
		fputs("\tbytesWrite_1\tdd\t?\n", output);
	}
}

void CodeGen::GenASMCode(const char* str, FILE* f)
{
	int n;
	for (n = 0; bufExprPostfixForm[n] != 3000; ++n)
	{
		stackStack.init(&stackStack.S);

		if ((!error.IsOperation(lexemsTable[bufExprPostfixForm[n]].type)) && (lexemsTable[bufExprPostfixForm[n]].type != ltNot) && (lexemsTable[bufExprPostfixForm[n - 1]].type != ltNot))
		{
			if (lexemsTable[bufExprPostfixForm[n]].type == ltIdentifier){
				fprintf(f, "\tfild %s\n", lexemsTable[bufExprPostfixForm[n]].name);
				//fprintf(f, "\tpush %s\n", lexemsTable[bufExprPostfixForm[n]].name);
			}
			else if (lexemsTable[bufExprPostfixForm[n]].type == ltNumber)
			{
				char buf[10];
				buf[0] = '0';
				sprintf(&buf[1], "%04x", lexemsTable[bufExprPostfixForm[n]].value);		buf[6] = '\0';
				fprintf(f, "\tmov word ptr buf,%sh\n", buf);
				fputs("\tfild buf\n", f);
				//fputs("\tpush buf\n", f);
			}
			else if ((lexemsTable[bufExprPostfixForm[n]].type == ltLBraket) || (lexemsTable[bufExprPostfixForm[n]].type == ltRBraket))
			{
				continue;
			}
		}
		else if (lexemsTable[bufExprPostfixForm[n]].type == ltNot)
		{
			if (lexemsTable[bufExprPostfixForm[n + 1]].type == ltNumber)
			{
				char buf[10];

				buf[0] = '0';
				sprintf(&buf[1], "%04x", lexemsTable[bufExprPostfixForm[n + 1]].value);
				buf[6] = '\0';

				fprintf(f, "\tmov word ptr buf,%sh\n", buf);
				fputs("\tfild buf\n", f);
				//fputs("\tpush buf\n", f);
				fputs("\tcall ltNot\n", f);
			}
		}
		else
		{
			switch (lexemsTable[bufExprPostfixForm[n]].type)
			{
			case ltAdd: fputs("\tfadd\n", f);
				break;
			case ltSub: fputs("\tfsub\n", f);
				break;
			case ltDiv: fputs("\tfdiv\n", f);
				break;
			case ltMod: fputs("\tcall mod_\n", f);
				break;
			case ltMul: fputs("\tfmul\n", f);
				break;
			case ltAnd: fputs("\tcall ltAnd\n", f);
				break;
			case ltOr: fputs("\tcall ltOr\n", f);
				break;

			case ltEqu: fputs("\tcall eq_\n", f); break;
			case ltNotEqu: fputs("\tcall eq_\n", f);
				fputs("\tcall ltNot\n", f); break;
			case ltLess: fputs("\tcall ltLess\n", f); break;
			case ltGreate: fputs("\tcall ltGreate\n", f); break;
			}
		}
	}
	fprintf(f, "\tfistp %s\n", str);
	/*if (str == "buf") {
		fputs("\tpop eax\n", output);
		fprintf(f, "\tmov %s,eax\n", str);
	}
	else {
		fputs("\tpop eax\n", output);
		fprintf(f, "\tmov %s,ax\n", str);
	}*/
}
