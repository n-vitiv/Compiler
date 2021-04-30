//Nazarii Vitiv KI-37
//28.12.20
//main file
#include "Analizer.h"
#include "CodeGen.h"
#include "ErrorsGen.h"

FILE* fin,* fout;
char inFileName[20];
char outFileName[20];

lexems lexemsTable[MAX_LEX];			// lexems table
int numberLexems = 0;					// number of lexems	
int numberErrors = 0;
identifier identTable[MAX_IDENT];		// identifier table
int numberOfIdent = 0;

int main(int argc, char* argv[]) {

	std::string inFileName, outFileName;
	std::regex inReg("[\\w]+(\.)(v45)"), outReg("[\\w]+(\.)(asm)");

	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "Translator from .v45 to .asm" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;

	if (argc != 3) {
		std::cout << "Error. Incorrect parametr list" << std::endl;
		std::cout << "Example: SP_Course_Work.exe FileName.v45 NewFile.asm" << std::endl;
		system("pause");
		exit(1);
	}

	inFileName = argv[1];
	outFileName = argv[2];
	strcpy(::inFileName, argv[1]);
	strcpy(::outFileName, argv[2]);

	if (!(std::regex_match(inFileName, inReg))) {
		std::cout << "Incorect input file extension" << std::endl;
		std::cout << "Must be .v45" << std::endl;
		system("pause");
		exit(1);
	}
	if (!(std::regex_match(outFileName, outReg))) {
		std::cout << "Incorect output file extension" << std::endl;
		std::cout << "Must be .asm" << std::endl;
		system("pause");
		exit(1);
	}

	if (!(fin = fopen(::inFileName, "r"))) {
		std::cout << "Can not open " << inFileName << " file" << std::endl;
		system("pause");
		exit(1);
	}
	else {
		std::cout << "File : " << inFileName << " successfully opened." << std::endl;
	}
	if ((fout = fopen(::outFileName, "w")) == 0)
	{
		std::cout << "Error: Can not create file: " << outFileName << std::endl;
		system("pause");
		exit(1);
	}
	else {
		std::cout << "File : " << outFileName << " successfully opened." << std::endl;
	}

	Analizer analizer(fin, lexemsTable);

	std::cout << "Starting of analize " << std::endl;
	numberLexems = analizer.LexemsAnalize();
	analizer.PrintLexemsInFile(numberLexems);
	std::cout << "Breaking into lexems completed." << std::endl;
	std::cout << "A total : " << numberLexems << " lexems. Report file : lexems.txt" << std::endl;

	ErrorsGen errorGenerator(lexemsTable, identTable, numberOfIdent, numberLexems);
	numberErrors = errorGenerator.ErrorsCheck();
	std::cout << "Error checking is complete." << std::endl;
	std::cout << "There are " << numberErrors << " errors. Report file : errors.txt" << std::endl;

	if (numberErrors != 0)
	{
		std::cout << "Can not translate because of errors." << std::endl;
		std::cout << "Put changes into codefile and try again." << std::endl;
	}
	else
	{
		std::cout << "Start generating file" << std::endl;
		CodeGen codeGenerator(fout, lexemsTable, numberOfIdent, identTable);
		codeGenerator.GenerateCode();
		std::cout << "Done! Result file: " << outFileName << std::endl;
		std::cout << "******************************************************************************" << std::endl;
	}

	fclose(fin);
	fclose(fout);
	return 0;
}