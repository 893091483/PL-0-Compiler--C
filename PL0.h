/*	
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	Jialin Zheng
	Erik Bates

	Systems Software | COP3402-20Fall 0001
	

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500



typedef struct instruction
{
	int op; // opcode
	int r; // R
	int l; // L
	int m; // M
}instruction;

typedef struct record
{
	int *array;
	int size;
	int capacity;
}record;

int lc = 0;
int line = 0;
int pc = 0;
int ir = 0;

int stack[MAX_STACK_HEIGHT];
int sp = MAX_STACK_HEIGHT;
int bp = MAX_STACK_HEIGHT - 1;

int registers[8];
instruction *readFile(FILE *input);

void showInstruction(FILE* output,FILE *screenOutput,instruction *instr, instruction *instrReg);
void printExecution(FILE * output,FILE *screenOutput, record *spArray, instruction instr);
void showInstructionName(FILE * output,FILE *screenOutput,instruction instr, int opiton);
record *createStackArray(int total_size);
int *expand(record *spArray);
void insert(record *spArray, int data);
int fetch_instruction(FILE* output,FILE *screenOutput,record *spArray, instruction *instr, instruction *instrReg, int halt);
int base(int l, int base);

// Reads in the provided input file and returns an array of instructions
instruction *readFile(FILE *input)
{
	instruction *instr = calloc(MAX_CODE_LENGTH, sizeof(instruction));
	while (!feof(input))
	{
    	fscanf(input, "%d %d %d %d", &instr[lc].op, &instr[lc].r, &instr[lc].l, &instr[lc].m);
  		lc++;
    
  		if (lc > MAX_CODE_LENGTH)
  			printf("ERROR: %d is the maximum length", MAX_CODE_LENGTH);	
	}
 
	return instr;
}

// Function that creates the stack arry
record *createStackArray(int total_size)
{
	record *arr = malloc(sizeof(record));
	arr->array = malloc(sizeof(int)*total_size);
	arr->size = 0;
	arr->capacity = total_size;
	return arr;
}

// Function to show the instruction's execution result
void showInstruction(FILE *output,FILE *screenOutput, instruction *instr, instruction *instrReg)
{
	int line;
	fprintf(output,"Line\tOP\t\tR\tL\tM\n");
  fprintf(screenOutput,"Line\tOP\t\tR\tL\tM\n");
  fprintf(finalOutput,"Line\tOP\t\tR\tL\tM\n");
  
	for (line = 0; line < lc; line++)
	{
   
		if (line < 10)
		{
			fprintf(output," ");
      fprintf(finalOutput, " ");
		}
		fprintf(output,"%d\t\t", line);
    fprintf(screenOutput,"%d\t\t", line);
    fprintf(finalOutput,"%d\t\t", line);
		showInstructionName(output,screenOutput,instr[line],0);
		fprintf(output,"\t\t%d\t%d\t%d\n", instr[line].r, instr[line].l, instr[line].m);
    fprintf(screenOutput,"\t\t%d\t%d\t%d\n", instr[line].r, instr[line].l, instr[line].m);
    fprintf(finalOutput,"\t\t%d\t%d\t%d\n", instr[line].r, instr[line].l, instr[line].m);
	}
}

void showInstructionName(FILE * output,FILE*screenOutput,instruction instr, int option)
{  
  
	switch(instr.op)
	{
		case 1:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("lit ");
				fprintf(screenOutput, "lit ");
			}
			fprintf(finalOutput, "lit ");
			fprintf(output, "lit ");
			break;
		case 2:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("rtn ");
				fprintf(screenOutput, "rtn ");
			}
			fprintf(finalOutput, "rtn ");
			fprintf(output, "rtn ");
			break;
		case 3:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("lod ");
				fprintf(screenOutput, "lod ");
			}
			fprintf(finalOutput, "lod ");
			fprintf(output, "lod ");
			break;
		case 4:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("sto ");
				fprintf(screenOutput, "sto ");
			}
			fprintf(finalOutput, "sto ");
			fprintf(output, "sto ");
			break;
    case 5:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("cal ");
				fprintf(screenOutput, "cal ");
			}
			fprintf(finalOutput, "cal ");
			fprintf(output, "cal ");
			break;  
		case 6:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("inc ");
				fprintf(screenOutput, "inc ");
			}
			fprintf(finalOutput, "inc ");
			fprintf(output, "inc ");
			break;
		case 7:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("jmp ");
				fprintf(screenOutput, "jmp ");
			}
			fprintf(finalOutput, "jmp ");
			fprintf(output, "jmp ");
			break;
		case 8:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("jpc ");
				fprintf(screenOutput, "jpc ");
			}
			fprintf(finalOutput, "jpc ");
			fprintf(output, "jpc ");
			break;
		case 9:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("sio ");
				fprintf(screenOutput, "sio ");
			}
			fprintf(finalOutput, "sio ");
			fprintf(output, "sio ");
			break;

		case 10:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("neg ");
				fprintf(screenOutput, "neg ");
			}
			fprintf(finalOutput, "neg ");
			fprintf(output, "neg ");
			break;
		case 11:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("add ");
				fprintf(screenOutput, "add ");
			}
			fprintf(finalOutput, "add ");
			fprintf(output, "add ");
			break;
		case 12:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("sub ");
				fprintf(screenOutput, "sub ");
			}
			fprintf(finalOutput, "sub ");
			fprintf(output, "sub ");
			break;
		case 13:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("mul ");
				fprintf(screenOutput, "mul ");
			}
			fprintf(finalOutput, "mul ");
			fprintf(output, "mul ");
			break;
		case 14:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("div ");
				fprintf(screenOutput, "div ");
			}
			fprintf(finalOutput, "div ");
			fprintf(output, "div ");
			break;
		case 15:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("odd ");
				fprintf(screenOutput, "odd ");
			}
			fprintf(finalOutput, "odd ");
			fprintf(output, "odd ");
			break;
		case 16:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("mod ");
				fprintf(screenOutput, "mod ");
			}
			fprintf(finalOutput, "mod ");
			fprintf(output, "mod ");
			break;
		case 17:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("eql ");
				fprintf(screenOutput, "eql ");
			}
			fprintf(finalOutput, "eql ");
			fprintf(output, "eql ");
			break;
		case 18:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("neq ");
				fprintf(screenOutput, "neq ");
			}
			fprintf(finalOutput, "neq ");
			fprintf(output, "neq ");
			break;
		case 19:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("lss ");
				fprintf(screenOutput, "lss ");
			}
			fprintf(finalOutput, "lss ");
			fprintf(output, "lss ");
			break;
		case 20:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("leq ");
				fprintf(screenOutput, "leq ");
			}
			fprintf(finalOutput, "leq ");
			fprintf(output, "leq ");
			break;
		case 21:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("gtr ");
				fprintf(screenOutput, "gtr ");
			}
			fprintf(finalOutput, "gtr ");
			fprintf(output, "gtr ");
			break;
		case 22:
			// prints to screen if selected
			if (printVm == 1 && option == 1)
			{
				printf("geq ");
				fprintf(screenOutput, "geq ");
			}
			fprintf(finalOutput, "geq ");
			fprintf(output, "geq ");
			break;
    default:
      //printf("invalid\n");
      break;
	}
}

// Function that expands a record from the stack and returns an array
int *expand(record *spArray)
{
	int cap = 2 * spArray->capacity + 1;
	int *array = malloc(sizeof(int) * cap);
	int sizeOfArray = spArray->size;
	spArray->capacity = cap;
	int i;
	for(i = 0; i < sizeOfArray; i++)
	{
		array[i] = spArray->array[i];
	}
	free(spArray->array);
	return array;
}

// Function to insert records into the stack array
void insert(record *spArray, int data)
{
	if(spArray->size >= spArray->capacity)
	{
		spArray->array = expand(spArray);
	}
	spArray->array[spArray->size] = sp;
	spArray->size++;
}

int fetch_instruction(FILE* output, FILE *screenOutput,record *spArray, instruction *instr, instruction *instrReg, int halt)
{
	int j;
	// Setting our halt
	halt = 0;

  if (printVm == 1)
	{
		if (printParser == 1 || printLex == 1)
		{
			printf("\n");
		}
		printf("VM Execution Trace:\n");
		fprintf(screenOutput, "\n");
		fprintf(screenOutput, "VM Execution Trace:\n");
	}
	fprintf(finalOutput, "\n");
	fprintf(finalOutput, "VM Execution Trace:\n");	

  if(printVm == 1){
    printf("\n\n");
    printf("\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
    printf("Initial values");
    printf("\t\t\t\t\t%d\t %d\t %d\t\n", pc, bp, sp);
    printf("Registers: %d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
    printf(" %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
    printf("Stack: ");

   fprintf(screenOutput,"\n\n");
	 fprintf(screenOutput,"\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
	 fprintf(screenOutput,"Initial values");
	 fprintf(screenOutput,"\t\t\t\t\t%d\t %d\t %d\t\n", pc, bp, sp);
	 fprintf(screenOutput,"Registers: %d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	 fprintf(screenOutput," %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
	 fprintf(screenOutput,"Stack: ");

  }

  fprintf(finalOutput,"\n\n");
	fprintf(finalOutput,"\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
	fprintf(finalOutput,"Initial values");
	fprintf(finalOutput,"\t\t\t\t\t%d\t %d\t %d\t\n", pc, bp, sp);
	fprintf(finalOutput,"Registers: %d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	fprintf(finalOutput," %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
	fprintf(finalOutput,"Stack: ");

  fprintf(output,"\n\n");
	fprintf(output,"\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
	fprintf(output,"Initial values");
	fprintf(output,"\t\t\t\t\t%d\t %d\t %d\t\n", pc, bp, sp);
	fprintf(output,"Registers: %d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	fprintf(output," %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
	fprintf(output,"Stack: ");
	for (j = MAX_STACK_HEIGHT; j > MAX_STACK_HEIGHT - 40; j--)
	{
		if (printVm == 1)
			{
				printf("%d", stack[j]);	
				fprintf(screenOutput, "%d", stack[j]);
			}
			fprintf(finalOutput, "%d", stack[j]);
			fprintf(output, "%d", stack[j]);
		
		
	}
  if (printVm == 1){
    	printf("\n\n");
	    printf("\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
      fprintf(screenOutput,"\n\n");
	    fprintf(screenOutput,"\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
  }
  fprintf(finalOutput,"\n\n");
	fprintf(finalOutput,"\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
	fprintf(output,"\n\n");
	fprintf(output,"\t\t\t\t\t\t\t\tpc\t bp\t sp\n");
  int lc_counter= lc; 

	// When not in halted state, fetch, execute, and print
	while (halt == 0 ){
  
    if(lc >0 && pc != lc_counter){
      lc--;
     
		if (pc < 10)
		{ 
			if (printVm == 1)
			{
				printf("%d ", pc);
				fprintf(screenOutput, "%d ", pc);
			}
			fprintf(finalOutput, "%d ", pc);
			fprintf(output, "%d ", pc);
		}
		else
		{
			if (printVm == 1)
			{
				printf("%d ", pc);
				fprintf(screenOutput, "%d ", pc);
			}
			fprintf(finalOutput, "%d ", pc);
			fprintf(output, "%d ", pc);
		}
		showInstructionName(output,screenOutput,instr[pc],1);

    if (printVm == 1){
      if(instr[pc].r != 0 &&  instr[pc].l!=0 && instr[pc].m!=0)
      {
        printf(" %d %d %d", instr[pc].r, instr[pc].l, instr[pc].m);
        printf("\t\t");
        fprintf(screenOutput," %d %d %d", instr[pc].r, instr[pc].l, instr[pc].m);
        fprintf(screenOutput,"\t\t");
      }
      
    }
    if(instr[pc].r != 0 &&  instr[pc].l!=0 && instr[pc].m!=0)
    {
      fprintf(finalOutput," %d %d %d", instr[pc].r, instr[pc].l, instr[pc].m);
      fprintf(finalOutput,"\t\t");
      fprintf(output," %d %d %d", instr[pc].r, instr[pc].l, instr[pc].m);
      fprintf(output,"\t\t");
    }
    
  
    }
		// Fetch cycle
		instrReg->op = instr[pc].op;
		instrReg->r = instr[pc].r;
		instrReg->l = instr[pc].l;
		instrReg->m = instr[pc].m;
		// Advance program counter following each instruction
		pc = pc + 1;
		// Execution cycle - with case for each instruction
		switch(instrReg->op)
		{
			// LIT
			case 1:
				registers[instrReg->r] = instrReg->m;
				break;
			// RTN
			case 2:
				insert(spArray, sp);
				sp = bp + 1;
				bp = stack[sp - 2];
				pc = stack[sp - 3];
				break;
			// LOD
			case 3:
				registers[instrReg->m] = stack[base(instrReg->l, bp)-instrReg->m];
				break;
			// STO
			case 4:
				stack[base(instrReg->l, bp) - instrReg->m] = registers[instrReg->r];
				break;
			// CAL
			case 5:
				
				stack[sp - 1] = base(instrReg->l, bp);
				stack[sp - 2] = bp;
				stack[sp - 3] = pc;
				bp = sp - 1;
				pc = instrReg->m;
				break;
			// INC
			case 6:
				insert(spArray, sp);
				sp = sp - instrReg->m;
				break;
			// JMP
			case 7:
				pc = instrReg->m;
				line = instrReg->m;
				break;
			// JPC
			case 8:
				if (registers[instrReg->r] == 0)
				{
					pc = instrReg->m;
				}
				break;
			// sys (1)
			case 9:
        if(instrReg->m == 1){
          
				  fprintf(output,"register value is: %d  ", registers[instrReg->r]);
          fprintf(finalOutput,"register value is:%d  ", registers[instrReg->r]);
          printf("register value is: %d  ", registers[instrReg->r]);
				break;
        }
        else if(instrReg->m == 2){
         
				  printf("Please enter the number:\n");
				  scanf("%d", &registers[instrReg->r]);
				break;
        }
        else if(instrReg->m == 3){
        
				halt = 0;
        break;
        }
			
			// NEG
			case 10:
				registers[instrReg->r] = -(registers[instrReg->l]);
				break;
			// ADD
			case 11:
				registers[instrReg->r] = (registers[instrReg->l] + registers[instrReg->m]);
				break;
			// SUB
			case 12:
				registers[instrReg->r] = (registers[instrReg->l] - registers[instrReg->m]);
				break;
			// MUL
			case 13:
				registers[instrReg->r] = (registers[instrReg->l] * registers[instrReg->m]);
				break;
			// DIV
			case 14:
				registers[instrReg->r] = (registers[instrReg->l] / registers[instrReg->m]);
				break;
			// ODD
			case 15:
				registers[instrReg->r] = registers[instrReg->l]%2;
				break;
			// MOD
			case 16:
				registers[instrReg->r] = registers[instrReg->l] % registers[instrReg->m];
				break;
			// EQL
			case 17:
				registers[instrReg->r] = (registers[instrReg->l] == registers[instrReg->m]);
				break;
			// NEQ
			case 18:
				registers[instrReg->r] = (registers[instrReg->l] != registers[instrReg->m]);
				break;
			// LSS
			case 19:
				registers[instrReg->r] = (registers[instrReg->l] < registers[instrReg->m]);
				break;
			// LEQ
			case 20:
				registers[instrReg->r] = (registers[instrReg->l] <= registers[instrReg->m]);
				break;
			// GTR
			case 21:
				registers[instrReg->r] = (registers[instrReg->l] > registers[instrReg->m]);
				break;
			// GEQ
			case 22:
				registers[instrReg->r] = (registers[instrReg->l] >= registers[instrReg->m]);
				break;
			default:
				halt = 1;
				break;
		}
		// Print the execution values for the program counter, base pointer, stack pointer, registers, and the stack
    	if(instrReg->op > 0 && instrReg->op <23 )
    	{
    		printExecution(output,screenOutput,spArray, instr[pc]);
    	}
	}   
}
// Prints out the execution instruction
void printExecution(FILE* output,FILE*screenOutput,record *spArray, instruction instr)
{
	int i;
	int j=1;	


  

  if (printVm == 1){
    printf("\t\t\t%d\t", pc);
    printf("%d\t", bp);
    printf(" %d\t\n", sp);
	  printf("Registers:");
	  printf("%d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	  printf(" %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
    printf("Stack: ");

    fprintf(screenOutput,"\t\t\t%d\t", pc);
    fprintf(screenOutput,"%d\t", bp);
    fprintf(screenOutput," %d\t\n", sp);
	  fprintf(screenOutput,"Registers:");
	  fprintf(screenOutput,"%d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	  fprintf(screenOutput," %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
    fprintf(screenOutput,"Stack: ");
  }
  fprintf(finalOutput,"\t\t\t%d\t", pc);
  fprintf(finalOutput,"%d\t", bp);
  fprintf(finalOutput," %d\t\n", sp);
	fprintf(finalOutput,"Registers:");
	fprintf(finalOutput,"%d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	fprintf(finalOutput," %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
  fprintf(finalOutput,"Stack: ");

 	fprintf(output,"\t\t\t%d\t", pc);
  fprintf(output,"%d\t", bp);
  fprintf(output," %d\t\n", sp);
	fprintf(output,"Registers:");
	fprintf(output,"%d %d %d %d", registers[0], registers[1], registers[2], registers[3]);
	fprintf(output," %d %d %d %d\n", registers[4], registers[5], registers[6], registers[7]);
  fprintf(output,"Stack: ");
	// Shows the contents of the stack
	for (i = MAX_STACK_HEIGHT-1; i >= sp; i--)
	{
		if (i+1 == spArray->array[j] )
		{
			if (printVm == 1)
			{
				printf("| ");
				fprintf(screenOutput, "| ");
			}
			fprintf(finalOutput, "| ");
	    	fprintf(output, "| ");
			j++;
		}
			if (printVm == 1)
			{
				printf("%d ", stack[i]);
				fprintf(screenOutput, "%d ", stack[i]);
			}
			fprintf(finalOutput, "%d ", stack[i]);
			fprintf(output, "%d ", stack[i]);
	}
	if (printVm == 1)
		{
			printf("\n\n");
			fprintf(screenOutput, "\n\n");
		}
		fprintf(output, "\n\n");
		fprintf(finalOutput, "\n\n");
}

// Given function, by Dr. Euripides Montagne at UCF.
// Finds lexographical level
int base(int l,int base) 
{  
  int b1; 
  b1 = base; 
  while (l > 0)
  {
    b1 = stack[b1];
    l--;
  }
  return b1;
}

void PL0(FILE *screenOutput)
{ 
  
  
  
  
	int halt = 0;
	instruction *instr = NULL;
	instruction instrReg;
	// Open file, with error check
  
	FILE *inputFile = fopen("machineCode.txt", "r");
	record *spArray = createStackArray(200);
	
	FILE * output = fopen("vmOutput.txt","w");
	instr = readFile(inputFile);
  
	showInstruction(output,screenOutput,instr, &instrReg);
  
	fprintf(output, "\n");
 
   for (int j = 0; j <= MAX_STACK_HEIGHT; j++)
	{
				stack[j] =0;
	}
  
	fetch_instruction(output,screenOutput, spArray, instr, &instrReg, halt);
	fclose(inputFile);
	fclose(output);
	free(instr);
  
	
}


