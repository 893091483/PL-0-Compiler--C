/*	
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	Jialin Zheng
	Erik Bates

	Systems Software | COP3402-20Fall 0001


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SYMBOL_TABLE_SIZE 300

// structure to keep track of symbols in symbol table list
typedef struct
{
	int kind; 		// const = 1, var = 2, proc = 3
	char name[12];	// name up to 11 chars
	int val; 		// number (ASCII value)
	int level; 		// L level
	int addr; 		// M address
	int mark;
} symbol;

// another structure for the recent tokens
typedef struct
{
	char kind[5];
	char value[12];
} token;

// current token value
token recent_token_value;
// symbol table array
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int table_size = 0;

// character tracker
char checker[5];

// number of Procedures and procedure position for x procedures
int numProcedures;
int currProcedure;
int procPos[50];

// recent token index and file pointer
int recent_token;
FILE *ifp;

// structure for codegen
typedef struct{
	int op; //opcode.
	int r;  //register.
	int l;  //lexicographical level.
	int m;  //modifier.
} VM_code;


// codegen index
int CG_index=0;
// codgen instruction array
VM_code instrArr[300];
// current register
int regi = 0;

// current token iterator for lexeme list
int currTokenIter = 0;
int maxTokenIter = 0;

// current identifier index
int ident_index = 0;

// max level
int maxLevel = 0;
// current level
int currLevel = 0;

// list of tokens
token_list allTokens[300];

// if any error, close all the currently open files and exit
void error(int error_num, FILE *screenOutput)
{
  if (printLex == 1)
  {
  	printf("\n%s\n", error_messages[error_num]);
  	fprintf(screenOutput, "\n%s\n", error_messages[error_num]);
  }
  else
  {
  	printf("%s\n", error_messages[error_num]);
  	fprintf(screenOutput, "%s\n", error_messages[error_num]);

  }
  fprintf(finalOutput, "\n%s", error_messages[error_num]);
  fclose(ifp);
  fclose(screenOutput);
  remove_end(DEBUGFAIL);
  exit(EXIT_SUCCESS);
}

// get the current token
void getToken(FILE *ifp)
{
	// if checker >0, then get token = checker
	if(strlen(checker)>0)
	{
		recent_token = atoi(checker);
		checker[0] = '\0';
	}
	// in any other case, token comes from file
	else
	{
		// if end of file, set to null term
		if(fscanf(ifp, "%s", recent_token_value.kind) == EOF)
		{
	    	recent_token_value.kind[0] = '\0';
	    	recent_token_value.value[0] = '\0';
	   		recent_token = 0;
		}
		// in any other case, set the value of kind.
		else
		{
			recent_token = atoi(recent_token_value.kind);
		}
	}
	// if the recent token is an identifier or number, get the value/name
	if(recent_token == identsym || recent_token == numbersym)
	{
		fscanf(ifp, "%s", recent_token_value.value);
	}
	// otherwise set to null terminator
	else
	{
		recent_token_value.value[0] = '\0';
	}
}

// add a symbol to the table
void insert_symbol_table(int kind, char *name, int val, int address, int level)
{
  symbol_table[table_size].kind = kind; // 1 or 2
  strcpy(symbol_table[table_size].name, name); // name of symbol
  symbol_table[table_size].val = val; // value
  symbol_table[table_size].level = level;
  symbol_table[table_size].addr = address; //address
  symbol_table[table_size].mark = 0; // 0 = available, 1 = unavailable
  table_size++; // increase table iterator
}

// searches through the symbol table for the symbol
// returns index the symbol is at, else error.
int search(char *name, FILE *screenOutput, int level)
{
  int flag = 0;
  int current;

  // for entire table, look for symbol
  // go one level at a time to ensure finding right symbol
  while (level >= 0)
  {
  	for (current = table_size; current >= 0; current--)
  	{
  		// if found symbol, return index
   		if(strcmp(symbol_table[current].name, name) == 0 && symbol_table[current].level == level && symbol_table[current].mark != 1)
  		{
  			ident_index = current;
   			flag = 1;
    		return current;
    	}
  	}
  	level--;
  }

  // if never found symbol, error
  if(flag == 0)
  {
    error(11, screenOutput);
  }
  return 1;
}

// invalidates old symbols by setting mark = 1
int invalidate(int level)
{
	int current;
	// look through entire table for anything to invalidate
	for (current = 0; current < table_size; current++)
	{
		if (symbol_table[current].level == level && symbol_table[current].kind != 3 && symbol_table[current].mark != 1)
		{
			symbol_table[current].mark == 1;
		}
	}
}

// outputs the assembly code for the program
void emit(int newOp, int newR, int newL, int newM)
{
	instrArr[CG_index].l = newL;
	instrArr[CG_index].m = newM;
	instrArr[CG_index].op = newOp;
	instrArr[CG_index].r = newR;
	CG_index++;
}

// finds if the operator is a relational operator or not
int rel_op(int token_type)
{
  // relational operators can be = <> < <= > >= and nothing else
  switch(token_type)
  {
  		// =
		case eqlsym:
			return 17;
		// <>
		case neqsym:
			return 18;
		// <
		case lessym:
			return 19;
		// <=
		case leqsym:
			return 20;
		// >
		case gtrsym:
			return 21;
		// >=
		case geqsym:
			return 22;
		// if not one, can never be 0
		default:
			return 0;
  }
}

// finds the factor
void factor(FILE *ifp, FILE *screenOutput)
{
  int index12, value;
  // if identifier, then search for it in table and output assembly for correct kind
  if (recent_token == identsym)
  {
  	// search for identifier in table
    index12 = search(recent_token_value.value, screenOutput, currLevel);
    // if constant, then emit for constant. else emit for variables.
    if (symbol_table[index12].kind == 1)
	{
		// LIT
		emit(1, regi, 0, symbol_table[ident_index].val);
	}
	else if (symbol_table[index12].kind == 2)
	{
		// LOD
		emit(3, regi, currLevel - symbol_table[index12].level, symbol_table[index12].addr);
	}
	else
	{
		error(21, screenOutput);
	}
	// increment register
    regi++;

    getToken(ifp);
  }
  // if number, then get the value of the number and generate assembly for it
  else if (recent_token == numbersym)
  {
    value = atoi(recent_token_value.value);
    // LIT
    emit(1, regi, 0, value);
    // increment register
	regi++;

    getToken(ifp);
  }
  // if (, get tokens and expressions until )
  else if (recent_token == lparentsym)
  {
    getToken(ifp);

    expression(ifp, screenOutput);
    if(recent_token != rparentsym)
    {
      //22.	Right parenthesis missing.
      error(22, screenOutput);
    }

    getToken(ifp);
  }
  else if (recent_token == procsym)
  {
  	error(21, screenOutput);
  }
  else
  {
  	// 23.	The preceding factor cannot begin with this symbol.
    error(23, screenOutput);
  }
}

// get the term
void term(FILE *ifp, FILE *screenOutput)
{
  int mulop;
  // find the factor
  factor(ifp, screenOutput);

  // while * or /, keep going
  while (recent_token == multsym || recent_token == slashsym)
  {
    mulop = recent_token;

    // get next token and factor
    getToken(ifp);
    factor(ifp, screenOutput);

    // if mult, perform assembly for *, decrement register
    if (mulop == multsym)
    {
      // MUL
      emit(13, regi - 2, regi - 2, regi - 1);
      regi--;
    }
    // if /, perform assembly for /, decrement register
    else
    {
      // DIV
      emit(14, regi - 2, regi - 2, regi - 1);
      regi--;
    }
  }
}

// perform the expression
void expression(FILE *ifp, FILE *screenOutput)
{
  int addop;

  // if + or -, get next token
  if (recent_token == plussym || recent_token == minussym)
  {
    addop = recent_token;
    getToken(ifp);
  }
  // find the term
  term(ifp, screenOutput);

  // if negative, output for negative number
  if (addop == minussym)
  {
  		// NEG
		emit(10, regi - 1, regi - 1, 0);
  }
  // while + or -, output assembly code for either and decrement register
  while (recent_token == plussym || recent_token == minussym)
  {
    addop = recent_token;

    // get the next token and term
    getToken(ifp);
    term(ifp, screenOutput);

    // if add, assembly code for +
	if(addop == plussym)
    {
      // ADD
      emit(11, regi - 2, regi - 2, regi - 1);
	  regi--;
    }
    // if minus, assembly code for -
    else
    {
      // SUB
      emit(12, regi - 2, regi - 2, regi - 1);
	  regi--;
    }
  }
}

// perform the condition
void condition(FILE *ifp, FILE *screenOutput)
{
  // if token is odd, get token, do expression, and then output assembly for odd
  if (recent_token == oddsym)
  {
    getToken(ifp);
    expression(ifp, screenOutput);
    // ODD
    emit(15, regi - 1, regi - 1, 0);
  }
  // any other case, do expression and find relational operator
  // output assembly for relational operator
  else
  {
    expression(ifp, screenOutput);
    int opcode = rel_op(recent_token);
    // 20. Relational operator expected
    if (opcode == 0)
    {
      error(20, screenOutput);
    }
    getToken(ifp);
    expression(ifp, screenOutput);
    emit(opcode, regi - 2, regi - 2, regi - 1);
    // decrement register
    regi--;
  }
}

// perform the statement
void statement(FILE *ifp, FILE *screenOutput)
{
  int index12, index34, thisAddr, current_cx, cx1, cx2, oldregi, current_kind;
  // switch to find if identifier, becomes, begin, if, while, read, or write
  switch(recent_token)
  {
  	// if identifier
    case identsym:
      // ensure exists, find what kind it is and make sure = variable
      index12 = search(recent_token_value.value, screenOutput, currLevel);
      if(symbol_table[index12].kind != 2)
      {
      	// 12.	Assignment to constant or procedure is not allowed.
        error(12, screenOutput);
      }
      // get address of symbol for later, then get next token and check if :=
      thisAddr = symbol_table[index12].addr;
      getToken(ifp);
      if(recent_token != becomessym)
      {
      	if (recent_token == eqlsym)
      	{
      		// 1.	Use = instead of :=.
      		error(1, screenOutput);
		}
		if (recent_token == semicolonsym)
		{
			error(15, screenOutput);
		}
      	// 13.	Assignment operator expected.
        error(13, screenOutput);
      }
      // get next token and expression
      getToken(ifp);
      
      if (recent_token == semicolonsym)
	  {
	  	// 24. An expression cannot begin with this symbol.
		error(24, screenOutput);
	  }
      expression(ifp, screenOutput);
      
      // output assembly for identifier
      emit(4, regi-1, currLevel - symbol_table[index12].level, thisAddr-1);

      // if the token is a relational operator, error (only allowed in if/while)
      if(rel_op(recent_token) != 0)
      {
        error(8, screenOutput);
      }
      // decrement register
      regi--;
      break;
    // if begin
    case beginsym:
      // get token and find statement
      getToken(ifp);
      statement(ifp, screenOutput);

      // while ;, get token and find statement
      while(recent_token == semicolonsym)
      {
        getToken(ifp);
        statement(ifp, screenOutput);
      }

      // if not end, error (begin always has end)
      if(recent_token != endsym)
      {
      	// 19.	Incorrect symbol following statement.
        error(19, screenOutput);
      }
      // get token
      getToken(ifp);
      break;
    // if its and if statement
    case ifsym:
      // get token and find if condition
      getToken(ifp);
      condition(ifp, screenOutput);
      // if token not then, error
      if (recent_token != thensym)
      {
      	// 16. then expected.
        error(16, screenOutput);
      }
      // save index and get token
      getToken(ifp);
      cx1 = CG_index;

	  //assembly code for JPC and decrease register
	  emit(8, regi - 1, 0, CG_index+1);
	  regi--;
	  
	  //call statement and go next token
	  statement(ifp, screenOutput);

	  // the checker is gonna go to the next
	  // token, if the next token is else(33)
	  // it's gonna call getToken()
	  // getToken() is gonna take the token in checker
	  // more detail in getToken()
	  fscanf(ifp, "%s", checker);
	  if (strcmp(checker, "33") == 0)
	  {
		getToken(ifp);
	  }

	  // check if we have else
	  if (recent_token == elsesym)
      {
        // save the current index
        cx2 = CG_index;

        // call JMP(7), m wil be reset later,
        // so set it to CG_index now
        emit(7 , 0, 0, CG_index);

        // this is reset m for JPC(8)
        instrArr[cx1].m = CG_index;
        getToken(ifp);

        // call statement
        statement(ifp, screenOutput);
        instrArr[cx2].m = CG_index;
      }
      else
      {
          instrArr[cx1].m = CG_index;
      }
	  
	  break;
	// if a call(for procedure)
	case callsym:
		// ensure exists, find what kind it is and make sure = variable
		getToken(ifp);
		
		// if not identifier after call, 
		if (recent_token != identsym)
		{
			// 14. call must be followed by an identifier.
			error(14, screenOutput);
		}
		index34 = search(recent_token_value.value, screenOutput, currLevel);

		// check if we have procedure in symbol table
		if (symbol_table[index34].kind == 3)
		{
			emit(5, 0, currLevel - symbol_table[index34].level, symbol_table[index34].addr+1);
		}
		else
		{
			// 14. Call must be followed by an identifier.
			error(14, screenOutput);
		}
		getToken(ifp);
		break;
    // if a while loop
    case whilesym:
      // save index
      current_cx = CG_index;

	  // get token and run condition
      getToken(ifp);
      condition(ifp, screenOutput);

	  // get index and save register
      cx1 = CG_index;
      oldregi = regi-1;

	  // emit the assembly code for JPC
      emit(8, oldregi, 0, CG_index+1);
      regi--;

	  // if next token isnt do, error
      if(recent_token != dosym)
      {
      	// 18.	do expected.
        error(18, screenOutput);
      }

	  // get token and run next statement
      getToken(ifp);
	  statement(ifp, screenOutput);

	  // emit the assembly code for JMP
      emit(7, 0, 0, current_cx);

	  // correct the m value
      instrArr[cx1].m = CG_index;
      break;
    // if read in value
    case readsym:
      // get token and find in table
      getToken(ifp);
      index12 = search(recent_token_value.value, screenOutput, currLevel);
      current_kind = symbol_table[index12].kind;

      // assembly code for SIO - read input
	  emit(10, regi, 0, 2);

	  // if the token kind is var, STO assembly code
	  if(current_kind == 2)
      {
	    //  STO
        emit(4, regi, currLevel - symbol_table[index12].level, symbol_table[index12].addr);
      }
      // if token is const, error
      else if (current_kind == 1 || current_kind == 3)
      {
      	// 12.	Assignment to constant is not allowed.
        error(12, screenOutput);
      }
      // get next token
      getToken(ifp);
      break;
    // if write value
    case writesym:
      // get token and find in table
      getToken(ifp);
      index12 = search(recent_token_value.value, screenOutput, currLevel);
      // if not identifier, error
      if (recent_token != identsym)
      {
      	// 4.	const and var must be followed by identifier.
      	error(4, screenOutput);
	  	}
	  // get kind
      int current_kind2 = symbol_table[index12].kind;
      // if var
      if (current_kind2 == 2)
      {
      	// assembly code for LOD
        emit(3, regi, currLevel - symbol_table[index12].level, symbol_table[index12].addr);
      }
      // if const
      else if (current_kind2 == 1)
      {
      	// assembly code for LIT
        emit(1, regi, 0, symbol_table[index12].val);
      }
      // get token and do assembly code for SIO(write)
      getToken(ifp);
      emit(9, regi, 0, 1);
      break;
    }
}

// run the constant part of block
void block_const(FILE *ifp, FILE *screenOutput)
{
  char name[12];
  int value;
  // while still more constants
  while (recent_token == commasym)
  {
    getToken(ifp);
    if (recent_token != identsym)
    {
      // 4.	const and var must be followed by identifier.
      error(4, screenOutput);
    }
    // save the name and get next token
    strcpy(name, recent_token_value.value);
    getToken(ifp);

    // if not =, error
    if(recent_token != eqlsym)
    {
      // 3.	Identifier must be followed by =.
      error(3, screenOutput);
    }
    getToken(ifp);
    if(recent_token != numbersym)
    {
      // 2.	= must be followed by a number.
      error(2, screenOutput);
    }
    // save value and put into the symbol table. get next token
    value = atoi(recent_token_value.value);
    insert_symbol_table(1, name, value, 0, currLevel);
    getToken(ifp);
  }
  // if final isnt a ;, error
  if (recent_token != semicolonsym)
  {
  	// 5.	Semicolon or comma missing.
    error(5, screenOutput);
  }
  // next token
  getToken(ifp);
}

// run the variable part of block
int block_var(FILE *ifp, int currM, FILE *screenOutput)
{
  char name[12];
  // while still more variables
  while(recent_token == commasym)
  {
  	  // get token, make sure identifier
      getToken(ifp);
      if(recent_token != identsym)
      {
      	// 4.	const and var must be followed by identifier.
        error(4, screenOutput);
      }
      // copy name and put in symbol table
      strcpy(name, recent_token_value.value);
      insert_symbol_table(2, name, 0, currM+4, currLevel);

      // increment M and get next token
      currM++;
      getToken(ifp);
  }

  // if recent token not ;, error
  if(recent_token != semicolonsym)
  {
  	// 5.	Semicolon or comma missing.
    error(5, screenOutput);
  }
  getToken(ifp);
  return currM;
}

// run the block
void block(FILE *ifp, FILE *screenOutput)
{
  int currIterM, value, procIter, currCodeIter = CG_index;
  int currM = 0;
  char name[12];
  int index;
  int jumpLine = 0;

  // assembly code for jump
  emit(7, 0, 0, 0);

  // if constant
  if(recent_token == constsym)
  {
  	// get token, make sure identifier
    getToken(ifp);
    if(recent_token != identsym)
    {
      // 4.	const and var must be followed by identifier.
      error(4, screenOutput);
    }

    // copy the name
    strcpy(name, recent_token_value.value);
    getToken(ifp);

    // if not equal, error
    if(recent_token != eqlsym)
    {
      // 3.	Identifier must be followed by =.
      error(3, screenOutput);
    }
    getToken(ifp);

    // if not a number, error
    if(recent_token != numbersym)
    {
      // 2.	= must be followed by a number.
      error(2, screenOutput);
    }
    // get the value of the token and add to table with name
    value = atoi(recent_token_value.value);
    insert_symbol_table(1, name, value, 0, currLevel);

    // get next token and run constant part of block
    getToken(ifp);
    block_const(ifp, screenOutput);
  }

  // if variable
  if(recent_token == varsym)
  {
  	// get token, make sure identifier
    getToken(ifp);
    if(recent_token != identsym)
    {
      // 4.	const and var must be followed by identifier.
      error(4, screenOutput);
    }
    // store name and put in table
	strcpy(name, recent_token_value.value);
    insert_symbol_table(2, name, 0, currM+4, currLevel);
    
    // set M value, get token
    currM++;
    getToken(ifp);
    // set current M value to variable part of block
    currM=block_var(ifp, currM, screenOutput);
  }

  // if procedure
  while (recent_token == procsym)
  {
	getToken(ifp);

	if(recent_token != identsym)
    {
      // 4.	const, procedure, var must be followed by identifier.
      error(4, screenOutput);
    }

  	// store name and put in table
	strcpy(name, recent_token_value.value);
    insert_symbol_table(3, name, 0, CG_index, currLevel);
    
	// increase lexical level and check if max
	currLevel++;

	getToken(ifp);

	if(recent_token != semicolonsym)
    {
      // 6. Incorrect symbol after procedure declaration.
      error(6, screenOutput);
    }

	getToken(ifp);

	// run block
	block(ifp, screenOutput);

	// reduce lexical level
	currLevel--;

	if (recent_token != semicolonsym)
	{
		error(10, screenOutput);
	}

	getToken(ifp);
  }

  // fix the M value for JMP
  instrArr[currCodeIter].m = CG_index;
  
  // put out assembly code for LOD
  emit(6, 0, 0, currM+3);

  // run a statement
  statement(ifp, screenOutput);

  // if continuing, output return and invalidate old symbols 
  if (recent_token != periodsym && recent_token == semicolonsym)
  {
  	emit(2, 0, 0, 0);
  	invalidate(currLevel);
  }
}

// run the actual, full program
void program(FILE *ifp, FILE *screenOutput)
{
  // get token
  getToken(ifp);

  // run block
  block(ifp, screenOutput);

  // find recent token kind, make sure period. else error.
  recent_token = atoi(recent_token_value.kind);
  if(recent_token != periodsym)
  {
  	// 9.	Period expected.
    error(9, screenOutput);
  }
  emit(9, 0, 0, 3);
}

// used to print the symbol table if neeeded for debugging
void printSymbolTable(int printFinal)
{
  FILE *tableOut = fopen("symbolTable.txt", "w");
  int i;
  fprintf(tableOut, " _______________________________________________________________________\n");
  fprintf(tableOut, "|                              Attributes                               |\n");
  fprintf(tableOut, "|_______________________________________________________________________|\n");
  fprintf(tableOut, "|  NUM  | kind  |     name      |     value     | level |    address    |\n");
  fprintf(tableOut, "|-------|-------|---------------|---------------|-------|---------------|\n");

  // print to final file for debugging
  if (printFinal == 1)
  {
  	fprintf(finalOutput, "\nSymbol Table:\n");
    fprintf(finalOutput, " _______________________________________________________________________\n");
    fprintf(finalOutput, "|                              Attributes                               |\n");
    fprintf(finalOutput, "|_______________________________________________________________________|\n");
    fprintf(finalOutput, "|  NUM  | kind  |     name      |     value     | level |    address    |\n");
    fprintf(finalOutput, "|-------|-------|---------------|---------------|-------|---------------|\n");
  }

  for(i = 0; i < table_size; i++)
  {
      fprintf(tableOut, "|%d\t|%d\t|%s\t\t|%d\t\t|%d\t|%d\t\t|\n",
      i+1,
      symbol_table[i].kind,
      symbol_table[i].name,
      symbol_table[i].val,
      symbol_table[i].level,
      symbol_table[i].addr);

      // print to final file for debugging
      if (printFinal == 1)
	  {
      	fprintf(finalOutput, "|%d\t|%d\t|%s\t\t|%d\t\t|%d\t|%d\t\t|\n",
        i+1,
        symbol_table[i].kind,
        symbol_table[i].name,
        symbol_table[i].val,
        symbol_table[i].level,
        symbol_table[i].addr);
	  }
  }
  fprintf(tableOut, "|_______|_______|_______________|_______________|_______|_______________|");

  // print to final file for debugging
  if (printFinal == 1)
  {
  	fprintf(finalOutput, "|_______|_______|_______________|_______________|_______|_______________|");
  	fprintf(finalOutput, "\n");
  }
  fclose(tableOut);
}

// populates the symbolic lexeme list arrau
void populateArray(FILE *input)
{
	char currChar, nextChar;
	int fullId, negative = 0;
	char newStr[11];
	int i;

	// grab the first character in the file
	currChar = fgetc(input);
	nextChar = currChar;

	// While not at the end of the file
	while (nextChar != EOF)
	{
		// set curr char to the next char
		currChar = nextChar;

		// grab the next character in the file
		nextChar = fgetc(input);

		// if a space, ignore it
		if (currChar == ' ')
		{
			continue;
		}

		// set the current character as part of the full id
		fullId = currChar - '0';

		// if the next character is a space, keep going
		// if the next character is more #, add it to the full id and get the next nextchar
		if (nextChar != ' ' && isalpha(nextChar) == 0)
		{
			fullId = fullId*10 + (nextChar - '0');
			nextChar = fgetc(input);
		}

		// Add the token type to the array
		allTokens[maxTokenIter].tokenTypeInt = fullId;
		// Go through a switch and assign the type name to the token type name part of the array
		switch (fullId)
		{
			case 1:
				strcpy(allTokens[maxTokenIter].lexemeType, "nulsym");
				break;
			// if an identifier, make sure to grab its name
			case 2:
				strcpy(allTokens[maxTokenIter].lexemeType, "identsym");

				// FILL IN THE LEXEME NAME PORTION
				// ignore the space in temp and get the first identifier variable
				i = 0;
				nextChar = fgetc(input);
				// while not end of the identifier / file, concatenate it with the string in the array.
				while (nextChar != ' ' && nextChar != EOF){
					newStr[i] = nextChar;
					nextChar = fgetc(input);
					i++;
				}
				newStr[i] = '\0';
				strcat(allTokens[maxTokenIter].lexemeName, newStr);
				break;
			// if a number, make sure to grab the whole number
			case 3:
				strcpy(allTokens[maxTokenIter].lexemeType, "numbersym");

				// FILL IN THE LEXEME NUMBER PORTION
				// ignore the space in temp and get the first number
				i = 0;
				nextChar = fgetc(input);
				if (nextChar == '-')
				{
					negative = 1;
					nextChar = fgetc(input);
				}
				else
				{
					negative = 0;
				}
				allTokens[maxTokenIter].lexemeNumber = (nextChar - '0');
				nextChar = fgetc(input);

				// while not end of the identifier / file, concatenate the number
				while (nextChar != ' ' && nextChar != EOF){
					allTokens[maxTokenIter].lexemeNumber = allTokens[maxTokenIter].lexemeNumber*10 + (nextChar - '0');
					nextChar = fgetc(input);
					i++;
				}

				if (negative == 1)
				{
					allTokens[maxTokenIter].lexemeNumber = -allTokens[maxTokenIter].lexemeNumber;
				}
				break;
			// +
			case 4:
				strcpy(allTokens[maxTokenIter].lexemeType, "plussym");
				break;
			// -
			case 5:
				strcpy(allTokens[maxTokenIter].lexemeType, "minussym");
				break;
			// *
			case 6:
				strcpy(allTokens[maxTokenIter].lexemeType, "multsym");
				break;
			// /
			case 7:
				strcpy(allTokens[maxTokenIter].lexemeType, "slashsym");
				break;
			// %
			case 8:
				strcpy(allTokens[maxTokenIter].lexemeType, "oddsym");
				break;
			// =
			case 9:
				strcpy(allTokens[maxTokenIter].lexemeType, "eqsym");
				break;
			// <>
			case 10:
				strcpy(allTokens[maxTokenIter].lexemeType, "neqsym");
				break;
			// <
			case 11:
				strcpy(allTokens[maxTokenIter].lexemeType, "lessym");
				break;
			// <=
			case 12:
				strcpy(allTokens[maxTokenIter].lexemeType, "leqsym");
				break;
			// >
			case 13:
				strcpy(allTokens[maxTokenIter].lexemeType, "gtrsym");
				break;
			// >=
			case 14:
				strcpy(allTokens[maxTokenIter].lexemeType, "geqsym");
				break;
			// (
			case 15:
				strcpy(allTokens[maxTokenIter].lexemeType, "lparentsym");
				break;
			// )
			case 16:
				strcpy(allTokens[maxTokenIter].lexemeType, "rparentsym");
				break;
			// ,
			case 17:
				strcpy(allTokens[maxTokenIter].lexemeType, "commasym");
				break;
			// ;
			case 18:
				strcpy(allTokens[maxTokenIter].lexemeType, "semicolonsym");
				break;
			// .
			case 19:
				strcpy(allTokens[maxTokenIter].lexemeType, "periodsym");
				break;
			// :=
			case 20:
				strcpy(allTokens[maxTokenIter].lexemeType, "becomessym");
				break;
			// begin
			case 21:
				strcpy(allTokens[maxTokenIter].lexemeType, "beginsym");
				break;
			// end
			case 22:
				strcpy(allTokens[maxTokenIter].lexemeType, "endsym");
				break;
			// if
			case 23:
				strcpy(allTokens[maxTokenIter].lexemeType, "ifsym");
				break;
			// then
			case 24:
				strcpy(allTokens[maxTokenIter].lexemeType, "thensym");
			// while
			case 25:
				strcpy(allTokens[maxTokenIter].lexemeType, "whilesym");
				break;
			// do
			case 26:
				strcpy(allTokens[maxTokenIter].lexemeType, "dosym");
				break;
			// call
			case 27:
				strcpy(allTokens[maxTokenIter].lexemeType, "callsym");
				break;
			// constant
			case 28:
				strcpy(allTokens[maxTokenIter].lexemeType, "constsym");
				break;
			// variable
			case 29:
				strcpy(allTokens[maxTokenIter].lexemeType, "varsym");
				break;
			// procedure
			case 30:
				strcpy(allTokens[maxTokenIter].lexemeType, "procsym");
				break;
			// write
			case 31:
				strcpy(allTokens[maxTokenIter].lexemeType, "writesym");
				break;
			// read
			case 32:
				strcpy(allTokens[maxTokenIter].lexemeType, "readsym");
				break;
			// else
			case 33:
				strcpy(allTokens[maxTokenIter].lexemeType, "elsesym");
				break;
		}
		maxTokenIter++;
	}
}

// prints out all the smybollic lexeme names
void printLexemeNames(FILE *screenOutput)
{
	int i = 0;
	// opens the symbolic names own output
	FILE *symbollicOutput = fopen("symbolicLexemes.txt", "w");

	// if print = 1, print to screen and to an output file
	if (printLex == 1)
	{
		printf("\n");
		printf("Symbolic Representation:\n");
		fprintf(screenOutput, "\n");
		fprintf(screenOutput, "Symbolic Representation:\n");
	}
	fprintf(finalOutput, "\n");
	fprintf(finalOutput, "Symbolic Representation:\n");

	// run up until the maximum token
	for (i = 0; i < maxTokenIter; i++)
	{
		// print out the symbollic lexemes
		// if print = 1, print to screen and to an output file
		if (printLex == 1)
		{
			printf("%s ", allTokens[i].lexemeType);
			fprintf(screenOutput, "%s ", allTokens[i].lexemeType);
		}
		fprintf(finalOutput, "%s ", allTokens[i].lexemeType);
		fprintf(symbollicOutput, "%s ", allTokens[i].lexemeType);
		// if its a variable or procedure, print out the variable name
		if (allTokens[i].tokenTypeInt == 2)
		{
			// if print = 1, print to screen and to an output file
			if (printLex == 1)
			{
				printf("%s ", allTokens[i].lexemeName);
				fprintf(screenOutput, "%s ", allTokens[i].lexemeName);
			}
			fprintf(finalOutput, "%s ", allTokens[i].lexemeName);
			fprintf(symbollicOutput, "%s ", allTokens[i].lexemeName);
		}
		// if its a number, print out the number
		if (allTokens[i].tokenTypeInt == 3)
		{
			// if print = 1, print to screen and to an output file
			if (printLex == 1)
			{
				printf("%d ", allTokens[i].lexemeNumber);
				fprintf(screenOutput, "%d ", allTokens[i].lexemeNumber);
			}
			fprintf(finalOutput, "%d ", allTokens[i].lexemeNumber);
			fprintf(symbollicOutput, "%d ", allTokens[i].lexemeNumber);
		}
	}
	// adjusted formatting for the final token.
	i++;
	// if print = 1, print to screen and to an output file
	if (printLex == 1)
	{
		printf("%s", allTokens[i].lexemeType);
		fprintf(screenOutput, "%s", allTokens[i].lexemeType);
	}
	fprintf(finalOutput, "%s", allTokens[i].lexemeType);
	fprintf(symbollicOutput, "%s", allTokens[i].lexemeType);

	if (allTokens[i].tokenTypeInt == 2)
	{
		// if print = 1, print to screen and to an output file
		if (printLex == 1)
		{
			printf("%s ", allTokens[i].lexemeName);
			fprintf(screenOutput, "%s ", allTokens[i].lexemeName);
		}
		fprintf(finalOutput, "%s ", allTokens[i].lexemeName);
		fprintf(symbollicOutput, "%s ", allTokens[i].lexemeName);
	}
	if (allTokens[i].tokenTypeInt == 3)
	{
		// if print = 1, print to screen and to an output file
		if (printLex == 1)
		{
			printf("%d ", allTokens[i].lexemeNumber);
			fprintf(screenOutput, "%d ", allTokens[i].lexemeNumber);
		}
		fprintf(finalOutput, "%d ", allTokens[i].lexemeNumber);
		fprintf(symbollicOutput, "%d ", allTokens[i].lexemeNumber);
	}
	// formatting
	if (printLex == 1)
	{
		printf("\n");
		fprintf(screenOutput, "\n");
	}

	fclose(symbollicOutput);
}

// prints out the generated assembly code
void printemit(FILE *screenOutput)
{
	int printIter, i;
	// open file specifically for machine code (to go to VM)
	FILE *emitOutput = fopen("machineCode.txt", "w");

	// if print = 1, print to screen and to an output file
	if (printParser == 1)
	{
		printf("\n");
		printf("Generated Code:\n");
		fprintf(screenOutput, "\n");
		fprintf(screenOutput, "Generated Code:\n");
	}
	fprintf(finalOutput, "\n");
	fprintf(finalOutput, "Generated Code:\n");


	// print out all the generated code
    for (i = 0; i < CG_index - 1; i++){
    	// if print =1, print to screen and output file
    	if (printParser == 1)
		{
			printf("%d %d %d %d\n", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
			fprintf(screenOutput, "%d %d %d %d\n", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
		}
		fprintf(finalOutput, "%d %d %d %d\n", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
    	fprintf(emitOutput, "%d %d %d %d\n", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
    	printIter++;
	}
	// formatting for final print statement
	// if print = 1, print to screen and output file
	if (printParser == 1)
	{
		printf("%d %d %d %d", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
		fprintf(screenOutput, "%d %d %d %d", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
	}
	fprintf(finalOutput, "%d %d %d %d", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);
    fprintf(emitOutput, "%d %d %d %d", instrArr[i].op, instrArr[i].r, instrArr[i].l, instrArr[i].m);

	// formatting
	if (printParser == 1)
	{
		printf("\n");
		fprintf(screenOutput, "\n");
	}
	fclose(emitOutput);
}

// Run the full parser code
void parser(FILE *screenOutput)
{
  // open up a file pointer for the lexeme list file
  ifp = fopen("lexemeList.txt", "r");

  // character checker
  checker[0] = '\0';
  //setup procedures
  numProcedures = 0;
  currProcedure = 0;

  // if file null, exit parser
  if(ifp == NULL)
  {
  	error(0, screenOutput);
  }

  // populates the array for printing symbollic lexemes
  populateArray(ifp);

  // prints the symbolic representation to its own file and to the final output (if selected)
  printLexemeNames(screenOutput);

  // resets file pointer
  fseek(ifp, 0, SEEK_SET);

  // runs the program, which creates the symbol table and the codegen
  program(ifp, screenOutput);
  fclose(ifp);

  // prints the codegen to its own output and to the final output (if selected)
  if (printLex == 1)
  {
  	printf("\n");
  }
  fprintf(screenOutput, "\n");
  printf("No errors, program is syntactically correct.\n");
  fprintf(screenOutput, "No errors, program is syntactically correct.\n");
  fprintf(finalOutput, "\n\n");
  fprintf(finalOutput, "No errors, program is syntactically correct.\n");


  // prints the symbol table to a file
  printSymbolTable(1);
  
  // print generated machine code
  printemit(screenOutput);
}
