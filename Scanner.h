/*	
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	Jialin Zheng
	Erik Bates

	Systems Software | COP3402-20Fall 0001
	

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>




void sanitizeInput(FILE *input,FILE *screenOutput);
void createLexemetable(FILE *screenOutput);
void createnumTokenList(FILE* screenOutput);
void writeToOutput(void);


void Scanner (FILE *input, FILE *screenOutput) {
    
    sanitizeInput(input, screenOutput);
    createLexemetable(screenOutput);
    writeToOutput();
    createnumTokenList(screenOutput);
    
}

/*int main (void) {
    finalOutput = fopen("finalOutput.txt", "w");
    FILE * input = fopen("input.txt", "r");
    FILE * screenOutput = fopen("screenOutput","w");
    sanitizeInput(input,screenOutput);
    createLexemetable(screenOutput);
    writeToOutput();
    createnumTokenList(screenOutput);
    return 0;
}*/


void writeToOutput(){
  FILE* _lexemetable = fopen("lexemeTable.txt","r");
  FILE* output = fopen("output.txt", "a");
  //char temp[12];
 
  char c;
  while((c = fgetc(_lexemetable)) != EOF){
    fputc(c, output);
  }
  fclose(_lexemetable);
  fclose(output);
}

//  Clean up comments
void sanitizeInput(FILE *input,FILE *screenOutput) {
    
    
    FILE *output = fopen("output.txt", "w");
    
    char c;
    int i = 0;
    char * str = calloc(2000 , sizeof(char));
    
    //fprintf(screenOutput,"Source Program:\n\n");
    //fprintf(finalOutput,"Source Program:\n\n");
    while ((c = fgetc(input)) != EOF)
    {
        str[i++] = c;
    }

    int flag=0;
    for (int j =0; j< i;j++)
    {
      if(j+1<i && str[j] == '/' && str[j+1] == '*')
      {
        j+=2;
        flag=1;
      }
      else if(j+1<i &&str[j] == '*' && str[j+1] == '/')
      {
        j+=2;
        flag=0;
      }
      else if(flag == 0)
      {    
          fprintf(screenOutput,"%c",str[j]);
          fprintf(finalOutput,"%c",str[j]);
          fprintf(output,"%c",str[j]);
          //printf("%c",str[j]);
      }
     
    }
    fclose(input);
    fclose(output);
}

void createLexemetable(FILE *screenOutput) {
    FILE *input = fopen("output.txt", "r");
    FILE *output = fopen("lexemeTable.txt", "w");
    char c;
    //int line = 1;
  char * cache = calloc(20,sizeof(char));
  //printf("\nLexeme Table:\n");
  fprintf(screenOutput,"\nLexeme Table:\n");
  fprintf(finalOutput,"\nLexeme Table:\n");

  //printf("lexeme\t\t\tnumToken type\n\n");
  fprintf(screenOutput,"lexeme\t\t\tnumToken type\n\n");
  fprintf(finalOutput,"lexeme\t\t\tnumToken type\n\n");
    while ((c = fgetc(input)) != EOF) {
    
    if(isdigit(c)){
      int i =0;
      int counter =1;
      int flag =0;
      
      do {
                cache[i++] = c;
                c = fgetc(input);
                counter++;
                
            } while (isdigit(c));
      if (counter > 6) {
          flag =1;
          printf("%s",cache);
          printf("%s\n", error_messages[25]);

          fprintf(screenOutput,"Number %s is too long\n",cache);
          fprintf(screenOutput,"%s\n",error_messages[25]);
          fprintf(finalOutput,"Number %s is too long\n",cache);
          fprintf(finalOutput,"%s\n",error_messages[25]);
		    exit(0);
                }
      else if (isalpha(c)) {

        flag =1;
        while(isalpha(c) ){
         cache[i++] = c;
        c = fgetc(input);
        }
        printf("%s is invalid\n",cache);
        //printf("\t\t\tERROR: Invalid Variable Name\n");
        printf("%s\n", error_messages[27]);
        fprintf(screenOutput,"%s is invalid\n",cache);
        fprintf(screenOutput,"%s\n", error_messages[27]);
        fprintf(finalOutput,"%s is invalid\n",cache);
        fprintf(finalOutput,"%s\n", error_messages[27]);
		    exit(0);

        /*if(c == ','){
          numToken++;
          fprintf(output, ",\t\t\t\t\t%d\n", commasym);
          printf(",\t\t\t\t\t%d\n", commasym);
        }
        if (c == ';') {
          numToken++;
        fprintf(output, ";\t\t\t\t\t%d\n", semicolonsym);
        printf(";\t\t\t\t\t%d\n", semicolonsym);
         
        }*/
       
            }
      
      else if (flag == 0){
      fprintf(output, "%s", cache);
      fprintf(screenOutput, "%s", cache);
      fprintf(finalOutput, "%s", cache);
      //printf("%s",cache);

      numToken++;

      fprintf(output,"\t\t\t\t%d\n", numbersym);
      fprintf(screenOutput,"\t\t\t\t%d\n", numbersym);
      fprintf(finalOutput,"\t\t\t\t%d\n", numbersym);
      //printf("\t\t\t\t%d\n", numbersym);
      }
      if(c == ','){
          numToken++;
          fprintf(output, ",\t\t\t\t%d\n", commasym);
          fprintf(screenOutput, ",\t\t\t\t\t%d\n", commasym);
          fprintf(finalOutput, ",\t\t\t\t\t%d\n", commasym);
          //printf(",\t\t\t\t\t%d\n", commasym);
        }
      if (c == ';') {
          numToken++;
        fprintf(output, ";\t\t\t\t%d\n", semicolonsym);
        fprintf(screenOutput, ",\t\t\t\t\t%d\n", semicolonsym);
        fprintf(finalOutput, ",\t\t\t\t\t%d\n", semicolonsym);
        //printf(";\t\t\t\t%d\n", semicolonsym);
         
        }
      else{
        flag =0;
      }
      i=0;
      memset(cache,0 , sizeof(20));
    }
        // Checks to see if input is in the alphabet
        else if (isalpha(c)) {
            int i = 0;
            char temp[20];
            int alphaCounter = 1;
            
            int j,flag1=0;
            for (j = 0; j < 20; j++) {
                temp[j] = '\0';
            }

            do {
                temp[i++] = c;
                c = fgetc(input);
                alphaCounter++;
              
            } while (isalpha(c) || isdigit(c));

            if (alphaCounter > 12) {
                flag1=1;
                printf("Name %s is too long\n", temp);
                printf("%s\n", error_messages[26]);
                
                fprintf(screenOutput, "%s\n", error_messages[26]);
                fprintf(screenOutput, "Name %s is too long\n",temp);
                fprintf(finalOutput, "%s\n", error_messages[26]);
                fprintf(finalOutput, "Name %s is too long\n", temp);

                exit(0);
            }
            
            ungetc(c, input);
      

            if (!strcmp(temp, "odd")) {
          numToken++;
                fprintf(output, "odd\t\t\t\t%d\n", oddsym);
                fprintf(screenOutput, "odd\t\t\t\t%d\n", oddsym);
                fprintf(finalOutput, "odd\t\t\t\t%d\n", oddsym);
                //printf("odd\t\t\t\t\t%d\n", oddsym);
            }
            else if (!strcmp(temp, "begin")) {
          numToken++;
                fprintf(output, "begin\t\t\t%d\n", beginsym);
                fprintf(screenOutput, "begin\t\t\t%d\n", beginsym);
                fprintf(finalOutput, "begin\t\t\t%d\n", beginsym);
                //printf("begin\t\t\t\t%d\n", beginsym);
            }
            else if (!strcmp(temp, "end")) {
          numToken++;
                fprintf(output, "end\t\t\t\t%d\n", endsym);
                fprintf(screenOutput, "end\t\t\t\t%d\n", endsym);
                fprintf(finalOutput, "end\t\t\t\t%d\n", endsym);
                //printf("end\t\t\t\t%d\n", endsym);
            }
            else if (!strcmp(temp, "if")) {
          numToken++;
                fprintf(output, "if\t\t\t\t%d\n", ifsym);
                fprintf(screenOutput, "if\t\t\t\t%d\n", ifsym);
                fprintf(finalOutput, "if\t\t\t\t%d\n", ifsym);
                //printf("if\t\t\t\t%d\n", ifsym);
            }
            else if (!strcmp(temp, "then")) {
          numToken++;
                fprintf(output, "then\t\t\t%d\n", thensym);
                fprintf(screenOutput, "then\t\t\t%d\n", thensym);
                fprintf(finalOutput, "then\t\t\t%d\n", thensym);
                //printf("then\t\t\t\t%d\n", thensym);
            }
            else if (!strcmp(temp, "while")) {
          numToken++;
                fprintf(output, "while\t\t\t%d\n", whilesym);
                fprintf(screenOutput, "while\t\t\t%d\n", whilesym);
                fprintf(finalOutput, "while\t\t\t%d\n", whilesym);
                //printf("while\t\t\t\t%d\n", whilesym);
            }
            else if (!strcmp(temp, "do")) {
          numToken++;
                fprintf(output, "do\t\t\t\t%d\n", dosym);
                fprintf(screenOutput, "do\t\t\t\t%d\n", dosym);
                fprintf(finalOutput, "do\t\t\t\t%d\n", dosym);
                //printf("do\t\t\t\t%d\n", dosym);
            }
            else if (!strcmp(temp, "call")) {
          numToken++;
                fprintf(output, "call\t\t\t%d\n", callsym);
                fprintf(screenOutput, "call\t\t\t%d\n", callsym);
                fprintf(finalOutput, "call\t\t\t%d\n", callsym);
                printf("call\t\t\t\t%d\n", callsym);
            }
            else if (!strcmp(temp, "const")) {
          numToken++;
                fprintf(output, "const\t\t\t%d\n", constsym);
                fprintf(screenOutput, "const\t\t\t%d\n", constsym);
                fprintf(finalOutput, "const\t\t\t%d\n", constsym);
                //printf("const\t\t\t\t%d\n", constsym);
            }
            else if (!strcmp(temp, "var")) {
          numToken++;
                fprintf(output, "var\t\t\t\t%d\n", varsym);
                fprintf(screenOutput, "var\t\t\t\t%d\n", varsym);
                fprintf(finalOutput, "var\t\t\t\t%d\n", varsym);
                //printf("var\t\t\t\t%d\n", varsym);
            }
            else if (!strcmp(temp, "procedure")) {
          numToken++;
                fprintf(output, "procedure\t%d\n", procsym);
                fprintf(screenOutput, "procedure\t%d\n", procsym);
                fprintf(finalOutput, "procedure\t%d\n", procsym);
                printf("procedure\t\t\t%d\n", procsym);
            }
            else if (!strcmp(temp, "write")) {
          numToken++;
                fprintf(output, "write\t\t\t\t%d\n", writesym);
                fprintf(screenOutput, "write\t\t\t\t%d\n", writesym);
                fprintf(finalOutput, "write\t\t\t\t%d\n", writesym);
                //printf("write\t\t\t\t%d\n", writesym);
            }
            else if (!strcmp(temp, "read")) {
          numToken++;
                fprintf(output, "read\t\t\t\t%d\n", readsym);
                fprintf(screenOutput, "read\t\t\t\t%d\n", readsym);
                fprintf(finalOutput, "read\t\t\t\t%d\n", readsym);
                //printf("read\t\t\t\t%d\n", readsym);
        }
            else if (!strcmp(temp, "else")) {
          numToken++;
                fprintf(output, "else\t\t\t\t%d\n", elsesym);
                fprintf(screenOutput, "else\t\t\t\t%d\n", elsesym);
                fprintf(finalOutput, "else\t\t\t\t%d\n", elsesym);
                printf("else\t\t\t\t%d\n", elsesym);
            }
            else {
          numToken++;
                if(flag1 ==0){
                 fprintf(output, "%s\t\t\t\t%d\n", temp, identsym);
                 fprintf(screenOutput, "%s\t\t\t\t%d\n", temp, identsym);
                 fprintf(finalOutput, "%s\t\t\t\t%d\n", temp, identsym);

                 //printf("%s\t\t\t\t%d\n", temp, identsym);
                }
                else
                    flag1=0;
            }
        }

        // Checks to see if input is a symbol
        else if (c == '+') {
      numToken++;
        fprintf(output, "+\t\t\t\t%d\n", plussym);
        fprintf(screenOutput, "+\t\t\t\t%d\n", plussym);
        fprintf(finalOutput, "+\t\t\t\t%d\n", plussym);
        //printf("+\t\t\t\t%d\n", plussym);
        }
        else if (c == '-') {
      numToken++;
            fprintf(output, "-\t\t\t\t%d\n", minussym);
            fprintf(screenOutput, "-\t\t\t\t%d\n", minussym);
            fprintf(finalOutput, "-\t\t\t\t%d\n", minussym);
            //printf("-\t\t\t\t%d\n", minussym);
        }
        else if (c == '*') {
        numToken++;
            fprintf(output, "*\t\t\t\t%d\n", multsym);
            fprintf(screenOutput, "*\t\t\t\t%d\n", multsym);
            fprintf(finalOutput, "*\t\t\t\t%d\n", multsym);
            //printf("*\t\t\t\t%d\n", multsym);
        }
        else if (c == '/') {
        numToken++;
            fprintf(output, "/\t\t\t\t%d\n", slashsym);
            fprintf(screenOutput, "/\t\t\t\t%d\n", slashsym);
            fprintf(finalOutput, "/\t\t\t\t%d\n", slashsym);
            //printf("/\t\t\t\t%d\n", slashsym);
        }
        else if (c == '=') {
        numToken++;
            fprintf(output, "=\t\t\t\t%d\n", eqlsym);
            fprintf(screenOutput, "=\t\t\t\t%d\n", eqlsym);
            fprintf(finalOutput, "=\t\t\t\t%d\n", eqlsym);
            //printf("=\t\t\t\t%d\n", eqlsym);
        }
        else if (c == '<') {
            c = fgetc(input);
            if (c == '>') {
          numToken++;
                fprintf(output, "<>\t\t\t\t%d\n", neqsym);
                fprintf(screenOutput, "<>\t\t\t\t%d\n", neqsym);
                fprintf(finalOutput, "<>\t\t\t\t%d\n", neqsym);
                //printf("<>\t\t\t\t%d\n", neqsym);
            }
            else if (c == '=') {
          numToken++;
                fprintf(output, "<=\t\t\t\t%d\n", leqsym);
                fprintf(screenOutput, "<=\t\t\t\t%d\n", leqsym);
                fprintf(finalOutput, "<=\t\t\t\t%d\n", leqsym);
                //printf("<=\t\t\t\t%d\n", leqsym);
            }
            else {
          numToken++;
                ungetc(c, input);
                fprintf(output, "<\t\t\t\t%d\n", lessym);
                fprintf(screenOutput, "<\t\t\t\t%d\n", lessym);
                fprintf(finalOutput, "<\t\t\t\t%d\n", lessym);
                //printf("<\t\t\t\t%d\n", lessym);
            }
        }
        else if (c == '>') {
            c = fgetc(input);
            if (c == '=') {
          numToken++;
                fprintf(output, ">=\t\t\t\t%d\n", geqsym);
                fprintf(screenOutput, ">=\t\t\t\t%d\n", geqsym);
                fprintf(finalOutput, ">=\t\t\t\t%d\n", geqsym);
                //printf(">=\t\t\t\t%d\n", geqsym);
            }
            else {
          numToken++;
                ungetc(c, input);
                fprintf(output, ">\t\t\t\t%d\n", gtrsym);
                fprintf(screenOutput, ">\t\t\t\t%d\n", gtrsym);
                fprintf(finalOutput, ">\t\t\t\t%d\n", gtrsym);
                //printf(">\t\t\t\t%d\n", gtrsym);
            }
        }
        else if (c == '(') {
        numToken++;
            fprintf(output, "(\t\t\t\t%d\n", lparentsym);
            fprintf(screenOutput, "(\t\t\t\t%d\n", lparentsym);
            fprintf(finalOutput, "(\t\t\t\t%d\n", lparentsym);
            //printf("(\t\t\t\t%d\n", lparentsym);
        }
        else if (c == ')') {
        numToken++;
            fprintf(output, "(\t\t\t\t%d\n", lparentsym);
            fprintf(screenOutput, "(\t\t\t\t%d\n", lparentsym);
            fprintf(finalOutput, "(\t\t\t\t%d\n", lparentsym);
            //printf("(\t\t\t\t%d\n", lparentsym);
        }
        else if (c == ',') {
        numToken++;
            fprintf(output, ",\t\t\t\t%d\n", commasym);
            fprintf(screenOutput, ",\t\t\t\t%d\n", commasym);
            fprintf(finalOutput, ",\t\t\t\t%d\n", commasym);
            //printf(",\t\t\t\t%d\n", commasym);
        }
        else if (c == ';') {
        numToken++;
            fprintf(output, ";\t\t\t\t%d\n", semicolonsym);
            fprintf(screenOutput, ";\t\t\t\t%d\n", semicolonsym);
            fprintf(finalOutput, ";\t\t\t\t%d\n", semicolonsym);
            //printf(";\t\t\t\t%d\n", semicolonsym);
        }
        else if (c == '.') {
        numToken++;
            fprintf(output, ".\t\t\t\t%d\n", periodsym);
            fprintf(screenOutput, ".\t\t\t\t%d\n", periodsym);
            fprintf(finalOutput, ".\t\t\t\t%d\n", periodsym);
            //printf(".\t\t\t\t%d\n", periodsym);
        }
        else if (c == ':') {
        
            c = fgetc(input);
        if(c == '='){
          numToken++;
          fprintf(output, ":=\t\t\t\t%d\n", becomessym);
          fprintf(screenOutput, ":=\t\t\t\t%d\n", becomessym);
          fprintf(finalOutput, ":=\t\t\t\t%d\n", becomessym);
          //printf(":=s\t\t\t\t%d\n", becomessym);
        }
        else{
          printf("%c is invalid \t\t\t%s\n",c,error_messages[27]);
          fprintf(screenOutput, "%s\n", error_messages[27]);
          fprintf(finalOutput, "%s\n", error_messages[27]);
		      exit(0);
          }
            
        }
        else if (!(isspace(c))){
        printf("%c is invalid \t\t\t%s\n",c,error_messages[27]);
        fprintf(screenOutput, "%s\n", error_messages[27]);
        fprintf(finalOutput, "%s\n", error_messages[27]);
		    exit(0);
            
        }
    }
  
    fclose(input);
    fclose(output);
    
}

void createnumTokenList(FILE* screenOutput) {
    //Open input file
    FILE *input = fopen("lexemeTable.txt", "r");
    FILE *lexemeOutput = fopen("lexemeList.txt", "w");

    if (printLex == 1)
  {
    printf("\nLexeme List:\n");
    fprintf(screenOutput, "\nLexeme List:\n");

  }
  // always print to overall output file.
  fprintf(finalOutput, "\n");
  fprintf(finalOutput, "\nLexeme List:\n");
    char temp[50], temp2[50];
    
    //Scans through entire file
    for(int i =0; i<numToken;i++){
        fscanf(input, "%s", temp);
        fscanf(input, "%s", temp2);
        if (!strcmp(temp2, "2")) {
        
            if(printLex == 1){
            printf("%s ", temp2);
            fprintf(screenOutput,"%s ", temp2);
            }
            fprintf(lexemeOutput,"%s ", temp2);
            fprintf(finalOutput,"%s ", temp2);
            
            if(printLex == 1){
            printf("%s ", temp);
            fprintf(screenOutput,"%s ", temp);
            }
            fprintf(lexemeOutput,"%s ", temp);
            fprintf(finalOutput,"%s ", temp);
        }
        else if (!strcmp(temp2, "3")) {
            
            if(printLex == 1){
            printf("%s ", temp2);
            fprintf(screenOutput,"%s ", temp2);        
            }
            fprintf(lexemeOutput,"%s ", temp2);
            fprintf(finalOutput,"%s ", temp2);
            
             if(printLex == 1){
              printf("%s ", temp);
               fprintf(screenOutput,"%s ", temp);
             }
              fprintf(lexemeOutput,"%s ", temp);
              fprintf(finalOutput,"%s ", temp);
        }  
        else{
              if(printLex == 1){
              printf("%s ", temp2);
              fprintf(screenOutput,"%s ", temp2);
              }
              fprintf(lexemeOutput,"%s ", temp2);
              fprintf(finalOutput,"%s ", temp2);
        }
        
    }
    //Close files
    fclose(input);
}


