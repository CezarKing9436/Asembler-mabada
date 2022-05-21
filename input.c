#include "Defenitions.h"

int isWhiteOnly(char* line);

/*--------------------------------------------------------------------------------
this function gets a line
input: the file we get the line from
return: the string of the line
----------------------------------------------------------------------------------*/
char * getLine(FILE *file)
{
	int c,i;
	char* line = malloc(MAX_LINE_LEN + 2);
	checkIfAllocated(line);/*tested malloc*/

	c = fgetc(file); /*get the first char*/
	if(c == EOF)/*check if we got to EOF*/
	{
		free(line);
		return NULL;
	}
	else if(c == '\n')
	{
		free(line);
		line = malloc(2*sizeof(char));
		checkIfAllocated(line);
		line[0] = '\n';
		line[1] = '\0';
		return line;
	}
	for(i = 0;i < MAX_LINE_LEN+1 && c != '\n' && c != EOF&&c!='\0';i++)
	/* we go over the line until we get more chars than needed or we get EOF or we get \n*/
	{
		line[i] = (char)c;
		c = fgetc(file);
	}
	if(i == MAX_LINE_LEN + 1)/*the line is longer than 80 chars*/
	{
		while(c != '\n')
			c = fgetc(file);
		line[MAX_LINE_LEN+1] = '\0';
		return line;
	}
	line[i] = '\n';
	line[i+1] = '\0';
	return line;
}
/*--------------------------------------------------------------------------------
this function gets a pointer to a char in a line and returns the first char that isnt "white space"
input: the char we start with
return: the first char that isnt "white space"
----------------------------------------------------------------------------------*/
char* getCharAfterWhiteSpace(char* c)
{
	while((*c == '\t') || (*c == '\n') || (*c == ' '))
	{
		c++;
	}
	return c;
}
/*--------------------------------------------------------------------------------
this function gets a pointer to a char in a line and returns the begining of that line
input: the char
return: char at the biginning of the line
----------------------------------------------------------------------------------*/
char* getBeginningOfLine(char* c){
	while(*c!='\n')
	{
		c--;/*doesn't work*/
	}
	c++;
	return c;
}
/*--------------------------------------------------------------------------------
this function returns the size of a given line
input: the line we check the length for (string)
return: the size (int)
----------------------------------------------------------------------------------*/
int sizeOfLine(char* line){
	int counter=0;
	while(*(line+counter)!='\0'&&*(line+counter)!='\n'&&*(line+counter)!=EOF){
		counter++;
	}
	return counter;
}

char* moveToWhiteSpace(char* line){
	int counter=0;
	while(*(line+counter)!=' '&&*(line+counter)!='\n'&&*(line+counter)!='\t'&&*(line+counter)!=EOF &&*(line+counter)!='\0'){
		counter++;
	}
	return line+counter;
}

int isWhiteOnly(char* line){
	while(*line!='\n'&&*line!='\0'&&*line!=EOF){
		if((*line)!=' '&&(*line)!='\t'){
			return 0;
		}
		line++;
	}
	return 1;
}

/*--------------------------------------------------------------------------------
this function checks if a line is a comment, or all "white space"
	input: char* line-the line we check.
	return: int. 1 if it is comment\empty, 0 if not.
----------------------------------------------------------------------------------*/
int isCommentOrEmpty(char* line){
  char* firstChar;
  if(isWhiteOnly(line)){/*if the line is all "white space"*/
    return 1;
  }
	/*if after the skip white spaces we got to the end of the line it means it is empty or maybe a comment*/
  firstChar = getCharAfterWhiteSpace(line);
  if(*firstChar==';'||*firstChar=='\0'||*firstChar==EOF||*firstChar=='\n'){
    return 1;
  }
  return 0;
}
/*--------------------------------------------------------------------------------
this function comapres the first chars of the line to the names of the commands 
and returns the index of command if exits
	input:char* line - the line checked
	return: int. the index of the command
----------------------------------------------------------------------------------*/
int findCommand(char* line){
  int i;
  char* commands[] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
  if(sizeOfLine(line)>=3){
    for(i=0;i<15;i++){
      if(strncmp(line,commands[i],3)==0){
        return i;/*here if the first letter of the command match somthing in commands list we return it's index*/
      }
    }
  }
  if(sizeOfLine(line)>=4){
    if(strncmp(line,commands[15],4)==0){
      return 15;
    }
  }
  return -1;
}
/*--------------------------------------------------------------------------------
The function gets a String,and moves to the closest comma.
	input: char *line
	return: char*. the closest comma or the end of the line
----------------------------------------------------------------------------------*/
char* moveToComma(char* line)
{

	line = getCharAfterWhiteSpace(line);
	while (*(line) != ',' && *line != EOF && *line != '\0' && *line != '\n')
		line++;
	return line;

}
/*--------------------------------------------------------------------------------
The function gets a String,and checks if it ends with a comma.
	input: char *line
	return: 1 if yes, 0 if no.
----------------------------------------------------------------------------------*/
int endsInComma(char* line)
{
	while (*getCharAfterWhiteSpace(line) != '\n' && *getCharAfterWhiteSpace(line) != '\0' &&*getCharAfterWhiteSpace(line) != EOF)
	{
		line = getCharAfterWhiteSpace(line);
		line = moveToWhiteSpace(line);
	}
	line--;
	if (*line == ',')
		return 1;
	return 0;

}
/*--------------------------------------------------------------------------------
The function gets a String,and checks if it has 2 commas by eachother.
	input: char *line
	return: 1 if yes, 0 if no.
----------------------------------------------------------------------------------*/
int has2CommasByEachOther(char* line)
{

	while (*line != '\n' && *line != '\0'&& *line != EOF)
	{
		if (*line == ',')
		{
			line = getCharAfterWhiteSpace(line);
			if (*(line + 1) == ',')
				return 1;
		}
		line++;
	}
	return 0;
}
/*--------------------------------------------------------------------------------
The function gets a String,and returns a pointer to the next word.
	input: char *line
	return: char*. the pointer
----------------------------------------------------------------------------------*/
char* getNextWord(char *line)
{
	line = getCharAfterWhiteSpace(line);
	line = moveToWhiteSpace(line);
	line = getCharAfterWhiteSpace(line);
	return line;
}
/*--------------------------------------------------------------------------------
The function gets a String, and returns the number of words.
	input: char *line
	return: int. the number of words.
----------------------------------------------------------------------------------*/
int numOfWordsInLine(char *line)
{
	int i = 0;
	line = getCharAfterWhiteSpace(line);
	while (line[0] != '\0')
	{
		i++;
		line = moveToWhiteSpace(line);
		line = getCharAfterWhiteSpace(line);
	}
	return i;

}

/*--------------------------------------------------------------------------------
The function gets a String, and returns the size of the first word.
	input: char *line
	return: int. the size.
----------------------------------------------------------------------------------*/
int getSizeFirstWord(char* line)
{
	int i = 0;
	line = getCharAfterWhiteSpace(line);
	while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF && line[i] != '\0')
		i++;
	return i;
}
/*--------------------------------------------------------------------------------
this function checks if we have a legal number until white space or a comma
	input:char* line - the line checked
	return: int. 1 if yes, 0 if no.
----------------------------------------------------------------------------------*/
int isNumUntilWhiteSpace(char* line)
{
	/*I DONT KNOW IF TO MOVE THIS TO INPUT.C*/
	int isLegal = 1;
	int i = 0;
	if(has2CommasByEachOther(line))
	{
		return 0;
	}
	line = getCharAfterWhiteSpace(line);
	if(*line == '\0' || *line == EOF || *line == '\n')
		isLegal = 0;
	if(*line == ',')
		line++;
	line = getCharAfterWhiteSpace(line);
	if(*line == '+' || *line == '-')
		line++;
	while(*(line+i) != EOF && *(line+i) != '\n' && *(line+i) != ' ' && *(line+i) != '\t' && *(line+i) != '\0'&& *(line+i) != ',')
	{
		if(*(line+i) > '9' || *(line+i) < '0')
			isLegal = 0;

		i++;
	}
	return isLegal;

}

int numFromLine(char** line)
{
  int num =0;
  int temp;
  while(**line!=','&&**line!='\n'&&**line!=0&&**line!= ' '){
      temp = **line-'0';
      num = num*10+temp;
      (*line)++;
  }
  return num;
}
/*--------------------------------------------------------------------------------
The function gets a String, and checks if it is a legal operand. if yes return 1
	input: char *line
	return: 1 if legal operand. 0 if not.
----------------------------------------------------------------------------------*/
int isLegalOperand(char* line)
{
	line = getCharAfterWhiteSpace(line);
	if((*line < 'A' || *line >'Z') && (*line < 'a' || *line >'z'))
		return 0;
	return 1;
}

/*--------------------------------------------------------------------------------
The function gets a String, and moves to a comma or a "whitespace"
	input: char *line
	return: a pointer to the comma or white space, if none then the end of the
	line.
----------------------------------------------------------------------------------*/
char* moveToCommaOrWhite(char* line)
{
	while (*line != ' ' && *line !=  '\n' && *line !=  ',' && *line !=  '\t' && *line !=  EOF && *line !=  '\0')
		line++;
	return line;
}

/*--------------------------------------------------------------------------------
The function gets a String, and returns the string untill the first white space, in
other words a substring strating at the beggining of the line and ending at the first
"white space".
	input: char *line
	return: a pointer to the substing.
----------------------------------------------------------------------------------*/
char* GetStringTilWhite(char *line){
 	int counter = 0,i=0;
  	char* subString;
  	/* to begin with we check the amount of letters we are going to be using*/
  	while((*(line+counter) != '\t') && (*(line+counter)  != '\n') && (*(line+counter) != ' ')&& (*(line+counter) != '\n')&& (*(line+counter) != '\0')&& (*(line+counter) != EOF)&&(*(line+counter) != '[')&&(*(line+counter) != ',')){
    		counter++;
  	}
  	/*we delacre a string with the wanted size and copy the letters*/
  	subString = malloc(counter);
		checkIfAllocated(subString);/*tested malloc*/


  	while(i<counter){
    		subString[i] = line[i];
    		i++;
  	}
  	subString[i] = '\0';
  	return subString;
}
/*--------------------------------------------------------------------------------
this function gets a line and returns the number of quotes (") in it.
	input:char* line- the line checked.
	return: int. the number of quotes.
----------------------------------------------------------------------------------*/
int numOfQuotes(char* line)
{
	/*variable decelerations*/
	int count = 0;
	int i = 0;
	while (*(line + i) != '\0' && *(line + i) != '\n' && *(line + i) != EOF)
	{
		if (*(line + i) == '"')
			count++;
		i++;

	}


	return count;
}
/*--------------------------------------------------------------------------------
this function gets a line and returns its last char.
	input:char* line- the line checked.
	return: char*. the last char. 
----------------------------------------------------------------------------------*/
char* getLastChar(char* line)
{
	/*variable decelerations*/
	int i = 0;
	while (*(line + i) != '\0' && *(line + i) != '\n' && *(line + i) != EOF)
	{
		i++;


	}
	i--;
	while (i >= 0 && (*(line + i) == ' ' || *(line + i) == '\t'))
	{
		i--;

	}
	return line + i;
}
/*--------------------------------------------------------------------------------
this function gets a line and finds the number commas in the line
	input:char* line- the line checked.
	return: int. the number of commas.
----------------------------------------------------------------------------------*/
int getNumOfCommas(char* line)
{
	/*variable decelerations*/
	int num = 0;
	int i;
	
	for (i = 0; *(line + i) != '\n' && *(line + i) != '\0' && *(line + i) != EOF; i++)
	{
		if (*(line + i) == ',')
			num++;
	}

	return num;
}
/*--------------------------------------------------------------------------------
this function checks if the next word in line is a register.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int isRegister(char* line)
{
	/*variable decelerations*/
	int isRegister = 0;
	int errorType = 0;
	line = getCharAfterWhiteSpace(line);

	if (*line == 'r')
	{
		line++;
		if (*line == '1' && (*(line + 1) <= '9' && *(line + 1) >= '0'))
		{
			line += 2;
			if (*line == ',' || *line == ']' || *line == '\n' || *line == EOF || *line == '\0' || *line == '\t' || *line == ' ')
			{
				isRegister = 1;
			}
			else
				errorType = 1;
		}

		if (*line <= '9' && *line >= '0' && !errorType)
		{
			line++;
			if (*line == ',' || *line == ']' || *line == '\n' || *line == EOF || *line == '\0' || *line == '\t' || *line == ' ')
			{
				isRegister = 1;
			}
		}
	}
	return isRegister;

}

/*--------------------------------------------------------------------------------
this function checks if the next word in line has a SINGLE comma in the middle. if so
we return 1.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int hasCommaInMid(char* line)
{
	/*variable decelerations*/
	int hasComma = 0;
	int i = 1;
	
	line = getCharAfterWhiteSpace(line);
	while (*(line + i) != '\n' && *(line + i) != EOF && *(line + i) != '\0'&& *(line + i) != '\t'&&*(line + i) != ' ')
	{
		if (hasComma && *(line + i) == ',')
			return 0;
		if (*(line + i) == ',' && (*(line + i + 1) != '\n' && *(line + i + 1) != EOF && *(line + i + 1) != '\0'&& *(line + i + 1) != '\t'&&*(line + i + 1) != ' '))
		{
			hasComma = 1;
		}
		i++;

	}
	return hasComma;
}
/*--------------------------------------------------------------------------------
this function checks if we have a lone comma (with no surounding values). if so
we return 1.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int hasAloneComma(char* line)
{
	/*variable decelerations*/
	int hasAloneComma = 0;
	
	if(*line == ',' && (*(line+1) == ' '|| *(line+1) == '\t' ||*(line+1) == '\0'||*(line+1) == '\n' ||*(line+1) == EOF))
		return 1;
	while(*moveToComma(line) != EOF && *moveToComma(line) != '\0' && *moveToComma(line) != '\n')
	{
		line = moveToComma(line);
		if((*(line-1) == ' '||*(line-1) == '\t') && (*(line+1) == ' '||*(line+1) == '\t'||*(line+1) == '\0'||*(line+1) == '\n'||*(line+1) == EOF))
			hasAloneComma = 1;
		line++;
	}
	return hasAloneComma;

}




