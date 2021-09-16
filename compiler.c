/*	
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	Jialin Zheng
	Erik Bates

	Systems Software | COP3402-20Fall 0001
	HW3 

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
*/


#include "header.h"
#include "Scanner.h"
#include "parser.h"
#include "PL0.h"

// removes all the old files at start
void remove_start()
{
	remove("screenOutput.txt");
	remove("finalOutput.txt");
}

// run the main compiler program
int main(int argc, char **argv)
{
	// removes any old files before running the new code. Clean slate.
	remove_start();
	
	FILE *screenOutput;
	int currInput = 0;

	// Open input and ensure exists
	FILE *input = fopen(argv[1], "r");
	if (input == NULL)
	{
		printf("%s\n", error_messages[0]);
		return 1;
	}

	// read in all the options
	while (currInput < argc)
	{
		// see if print lexeme list and symbollic representation
		if (strcmp(argv[currInput], "-l") == 0)
		{
			printLex = 1;
		}
		// see if print out the codegen assembly code
		if (strcmp(argv[currInput], "-a") == 0)
		{
			printParser = 1;
		}
		// see if print virtual machine execution trace
		if (strcmp(argv[currInput], "-v") == 0)
		{
			printVm = 1;
		}
		currInput++;
	}
	
	// once all options obtained, open the overarching output files
	screenOutput = fopen("screenOutput.txt", "w");
	finalOutput = fopen("finalOutput.txt", "w");

	// If no compiler directives, print in
	if (printLex == 0 && printParser == 0 && printVm == 0)
	{
		printf("in\n");
	}
   
	// run lexer, parser, and vm
	Scanner(input, screenOutput);
	fclose(input);
	parser(screenOutput);
	PL0(screenOutput);
	
	// if no compiler directives, print out
	if (printLex == 0 && printParser == 0 && printVm == 0)
	{
		printf("out\n");
	}
  fclose(finalOutput);
  fclose(screenOutput);
	remove_end(DEBUG);

	// return 0 for successul execution
	return 0;
}
