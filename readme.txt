Jailin Zheng & Erik Bates

This tutorial is for running on a unix-based system.

Compiling
------------------------------------------------------------------------
To compile the compiler, enter the following program in your command prompt or terminal:
gcc -o compiler hw4compiler.c

 *Run the jewels

Back in your terminal input:
	./compile input.txt -compilerDirective
		-l	print the list of lexemes/tokens
		-a	print the generated assembly code
		-v	print the virtual machine execution trace

Screen Output
------------------------------------------------------------------------
Depending on the compiler objectives, this program will output to the screen each objective.
If there are no objectives, the program will only output “in” on starting, any read/write output, and then finally “out” upon ending the program.
If there are any errors, then the program will stop upon reaching any errors.

File Output
------------------------------------------------------------------------
A file containing all possible output is also created for debugging purposes, finalOutput.txt. This file includes:
*	Source program
*	Lexeme table
*	Lexeme list
*	Symbolic lexeme representation
*	Any errors / lack of errors
*	Symbol Table
*	Generated assembly code
*	Interpreted assembly code
*	Virtual machine execution trace
*	Any screen printing from the virtual machine execution