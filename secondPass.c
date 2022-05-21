#include <ctype.h>
#include "Defenitions.h"

int hasAloneComma(char* line);
int isInstruction(char* line);
int checkForErrorCommand(char* line, int lineNum);
int isRegister(char* line);
int isCommand(char* line);
int isAdressinMethod0(char* line);
int hasCommaInMid(char* line);
int isAdressinMethod3(char* line);
int checkIfData(char *line);
int isNumUntilWhiteSpace(char* line);
int numOfNums(char* line);
int getNumOfCommas(char* line);
char* getLastChar(char* line);
int numOfQuotes(char* line);
struct wordsList** procesLabel(char* line, struct wordsList** instructionList, int* DC, struct symbolNode** symbolList,int isInstructionResult,int lineCounter,int* hasError);
int checkIfExtern(char* line);
struct wordsList** procesCommand(char* line, struct wordsList** wordList, int* IC, struct symbolNode** symbolList,int lineCounter,int* hasError);
struct wordsList** analyzeOneOperand(struct wordsList** wordList, int* IC, int command, char* line);
void findAdressingMethod(char* line, int*);
int checkForErrorInstruction(char* line, int lineNum);
int checkIfString(char *line);

int secondPass(FILE* file,struct wordsList** wordList,struct wordsList** instructionList,struct symbolNode** symbolList,int* IC,int* DC)
{
	/*variable decelerations*/
	char* line;
  	int lineCounter=1;
  	int hasError = 0;
	int isInstructionResult;
	int errorInstruction;
	int errorCommand;

  	fseek(file, 0, SEEK_SET);
	while((line = getLine(file)))
	{

	/*this is where each line gets checked for errors*/
	errorCommand = checkForErrorCommand(line, lineCounter);
	errorInstruction = checkForErrorInstruction(line, lineCounter);

	if(errorInstruction == 1 || errorCommand == 1)
		hasError = 1;

	if(errorInstruction == -1 && errorCommand == -1)
	{
		if(*getCharAfterWhiteSpace(line) != '\n' && *getCharAfterWhiteSpace(line) != '\0' && *getCharAfterWhiteSpace(line) != EOF)
		{
			printf("ERROR! at line %d,this is not a known instruction, or command\n",lineCounter);
			hasError = 1;
		}

	}
  	if(isCommentOrEmpty(line))
	{
  	}
  else if(!hasError){
			isInstructionResult=isInstruction(line);
			if(isInstructionResult==1||isInstructionResult==2){
				instructionList = procesLabel(line,instructionList,DC,symbolList,isInstructionResult,lineCounter,&hasError);
			}
      else{
				wordList = procesCommand(line,wordList,IC,symbolList,lineCounter,&hasError);
			}
  }
  lineCounter++;
  free(line);
	}
  return hasError;
}

/*--------------------------------------------------------------------------------
this function proces the line, adds symbol if needs and calls the function thats
need to be called for the line
	input: char* line-the line,struct wordsList** wordList-the encoded list,
	int* IC-the counter,struct symbolNode** symbolList- the symbols list
	return: struct wordsList**. the last encoded line added
----------------------------------------------------------------------------------*/
struct wordsList** procesCommand(char* line,struct wordsList** wordList,int* IC,struct symbolNode** symbolList,int lineNumber,int* hasError)
{
	/*variable decelerations*/
	char* saveBeforeSymbol;
  	int command;
  	char* name = line;/*for the symbol we need to the save the start of the line*/
  	/*at the start we need to skip the Symbol, if has,if has symbol we add it and then we go to the command*/
	char* nameSymbol;
	symbolNode* Symbol=NULL;
  	/*The firs tpart of the function skips the label if it is declared by checking after we skip the first word.
  	if the char before it is ":" if yes we skip another word and add it to the symbolList*/

  	line = getCharAfterWhiteSpace(line);
  	saveBeforeSymbol = moveToWhiteSpace(line);
  	saveBeforeSymbol--;/*LINE IS NOT EMPTY*/

  	if(*(saveBeforeSymbol)==':')
	{
     		/*add symbol here and skip the word*/
		nameSymbol = GetStringTilWhite(line);
		Symbol= findEntry(symbolList,nameSymbol);
		if(Symbol==NULL)/*this is a new symbol*/
		{
    			addSymbol(symbolList,name,IC,CODE);
		}
		else
		{
			*hasError=1;
			printf("ERROR in line %d, an entry was declared twice\n",lineNumber );
		}
		/*if it as not already declared, we save the symbol*/
    		saveBeforeSymbol++;
    		saveBeforeSymbol = getCharAfterWhiteSpace(saveBeforeSymbol);
    		line = saveBeforeSymbol;
  	}
  	command = findCommand(line);
  	if(command == RTS || command==STOP)
	{
		/* in here we only need to add one word to the wordsList and so we just add it*/
      		wordList=addZeroOperand(wordList,IC,command);
      		(*IC)++;
  	}
	else if(command == CLR || command == NOT || command == INC || command == DEC || command == JMP || command == BNE || command == JSR || command == RED || command == PRN )
  	{
		/* an anayze to the line needs to be made for us to know the contex of the line*/
    		wordList = analyzeOneOperand(wordList,IC,command,line);/*this function is in bitfield.c*/
  	}
 	else
	{
		/* an anayze to the line needs to be made for us to know the contex of the line*/
    		wordList = analyzeTwoOperand(wordList,IC,command,line);/*this function is in bitfield.c*/
  	}
  	return wordList;
}


/*--------------------------------------------------------------------------------
this function adds the proper words to the instructionList depending on the line
	input:char* line - the line checked,struct wordsList** instructionList-
	the list we encode to,int* DC- the counter,struct symbolNode**
	symbolList- the symbolLIst we add to
	return: struct wordsList**. the last added instruction if added
----------------------------------------------------------------------------------*/
struct wordsList** procesLabel(char* line,struct wordsList** instructionList,int* DC,struct symbolNode** symbolList,int isInstructionResult,int lineNumber,int* hasError)
{
	/*variable decelerations*/
  	char* data = ".data";
  	char* string  = ".string";
  	char* exter = ".extern";
  	int number;
  	char* name;/*if a symbol has been delcared we can find the name with the start of the line(the first non white char)*/
	char* nameEntry;
  	char* saveBeforeSymbol = line;
	symbolNode* external=NULL;
	/*at the start we want to skip the label if it exits and add it also, if it doesn't exits we dont add nothing*/

  	line = getCharAfterWhiteSpace(line);
  	name = line;
  	saveBeforeSymbol=moveToWhiteSpace(line);
  	if(saveBeforeSymbol-name>=1)
	{
    		saveBeforeSymbol--;/*LINE IS NOT EMPTY*/
  	}
  	if(*(saveBeforeSymbol)==':')
	{
		if(isInstructionResult==1)
		{
			addSymbol(symbolList,name,DC,DATA);/*adds the symbol to the list if it isn't entry\extenral*/
		}
		else if(isInstructionResult==2)
		{
			printf("WARNING IN LINE %d, the symbole has no purpose\n",lineNumber );
		}
    		saveBeforeSymbol++;
    		saveBeforeSymbol = getCharAfterWhiteSpace(saveBeforeSymbol);/*skip the label*/
    		line = saveBeforeSymbol;
  	}
  	if(strncmp(line,data,5)==0 && strlen(line) >5)
	{
		/* if there is a match to the .data we start adding the numbers*/
    		line+=5;
    		while(*line!='\n'&&*line!='\0')
		{
      			line = getCharAfterWhiteSpace(line);
      			if(*line=='-')
			{
				/*if starts with negative sign we need to multiple by -1 the number so we differ the cases*/
       				line++;
        			number = -1*numFromLine(&line);
      			}
			else if(*line=='+')
			{
       				line++;
        			number = numFromLine(&line);
      			}
      			else
			{
        			number = numFromLine(&line);
      			}

			/*after we got the line we add it the list*/
      			instructionList=addWord3(instructionList,DC,number);
      			(*DC)++;

			/*and now we skip to the next number*/
			if(*line==',')
			{
				line++;
			}
			else
			{
				line = getCharAfterWhiteSpace(line);
				if(*line==',')
				{
					line++;
				}
			}
    		}
  	}
  	else if(strncmp(line,string,7)==0 && strlen(line) >= 7)
	{
    		line+=7;
    		line = getCharAfterWhiteSpace(line);
    		line++;
    		while(*line!=34)/*the max length of the string is 35*/
		{
			/*while we do not get to the "" we keep adding the chars*/
      			number= *line;
      			instructionList = addWord3(instructionList,DC,number);
      			(*DC)++;
      			line++;
    		}
    		instructionList = addWord3(instructionList,DC,0);
    		(*DC)++;
  	}
  	else if(strncmp(line,exter,7)==0)
	{
		/*if match to extern we just add the extern to the symbol list*/
		name = line;
		line = getNextWord(line);
		nameEntry = GetStringTilWhite(line);
		external = findEntry(symbolList,nameEntry);
		if(external==NULL)
		{
			addExtern(symbolList,name);
		}
		else
		{
			printf("ERROR IN LINE %d an entry was declared TWICE\n",lineNumber );
			*hasError=1;
		}
  	}
  	return instructionList;
}
/*--------------------------------------------------------------------------------
this function checks if the line is an instruction
	input:char* line- the line checked
	return: int. 1 if is data or string, 2 if entry or extern, 0 if not a
	instruction.
----------------------------------------------------------------------------------*/
int isInstruction(char* line)
{
	/*variable decelerations*/
  	char* data = ".data";
  	char* string  = ".string";
  	char* entry = ".entry";
  	char* exter = ".extern";
  	char* saveBeforeSymbol;
  	char* startLine= line;

	/* at the start we skip the label if exits*/
	line = getCharAfterWhiteSpace(line);
  	saveBeforeSymbol = moveToWhiteSpace(line);
  	if(saveBeforeSymbol-startLine>=1)
	{
    		saveBeforeSymbol--;/*LINE IS NOT EMPTY*/
  	}
  	if(*(saveBeforeSymbol)==':')
	{
    		saveBeforeSymbol++;
    		saveBeforeSymbol = getCharAfterWhiteSpace(saveBeforeSymbol);
    		line = saveBeforeSymbol;
  	}

	/* now we check if the first letters match one of the instruction command*/

	/*checks for instruction type ".data"*/
  	if(strncmp(line,data,5)==0)/*strcmp returns 0 if they are the same*/
	{
    		return 1;
  	}

	/*checks for instruction type ".entry"*/
 	if(strncmp(line,entry,6)==0)
	{
    		return 2;
  	}

	/*checks for instruction type ".string"*/
  	if(sizeOfLine(line)>=7&&strncmp(line,string,7)==0)
	{
    		return 1;
  	}

	/*checks for instruction type ".extern"*/
  	if(strncmp(line,exter,7)==0)
	{
    		return 2;
  	}

	/*if code reches this part line is not any of the above and this is not an instruction*/
  	return 0;
}
/*--------------------------------------------------------------------------------
this function gets a line. if the line is not an Instruction type of line, we
retrun -1. if it is, we check it for all of the possible errors, for example if
we have a line ".entry 234,535" we will report an error since the "entry" instruction
gets only one operand. if we found an error we return 1, if not we return 0.
	input:char* line- the line checked, int lineNum- the number of the line in
	the post macro file.
	return: int. -1 it is not an instruction line, 1 if we have an error 0 if no
----------------------------------------------------------------------------------*/
int checkForErrorInstruction(char* line, int lineNum)
{
	/*variable decelerations*/

	int i = 0;
	char* tempLine = line;


	/*we check if its a lable like LOOP:, and skip it*/
	if(*(moveToWhiteSpace(line) - 1) == ':')
	{
		if(!((*line >= 'A' && *line <= 'Z')||(*line >= 'a' && *line <= 'z')))/*needs to be alphanumeric*/
		{
			printf("ERROR! at line %d,a lable needs to start with a letter!\n",lineNum);
			return 1;
		}
		else
		{
			line = getNextWord(line);

		}
	}
	line = getCharAfterWhiteSpace(line);

	/*the "data instruction"*/
	if(checkIfData(line))
	{
		line = getCharAfterWhiteSpace(line);
		line += 5; /*to skip the .data and get to the operands*/
		line = getCharAfterWhiteSpace(line);



		while(*tempLine != '\n'  && *tempLine != '\0' && *tempLine != EOF)
		{


			if(!isNumUntilWhiteSpace(line+i))/*this checks every operand*/
			{
				/*the .data can recieve anything only numbers*/
				if(*tempLine != ' ' && *tempLine != '\n' && *tempLine != '\0' && *tempLine != EOF && *tempLine != '\t')
				{

					printf("ERROR! at line %d, the \".data\" Instruction gets whole numbers exclusivly.\n", lineNum);
				}
				return 1;
			}
			/*moving forward in the loop*/
			tempLine++;
			if(*tempLine == ',')
				tempLine++;

		}
		if(numOfNums(line) == 0)/*note that num of nums returns 0 if it sees a "none-number operand"*/
		{
			printf("ERROR! at line %d, the values given are not legal numbers.\n", lineNum);
			return 1;
		}
		if(numOfNums(line)-1 != getNumOfCommas(line))/*we need n-1 commas, when we have n operands.*/
		{
				if(numOfNums(line) != 0)
				{

					printf("ERROR! at line %d, every number needs to be seperated by a comma.\n", lineNum);
					return 1;
				}
		}
		if(has2CommasByEachOther(line))/*no 2 commas without operand in between*/
		{
			printf("ERROR! at line %d, we cant have 2 commas by each other.\n", lineNum);
			return 1;

		}
		if(endsInComma(line))/*cant end with a comma.*/
		{
			printf("ERROR! at line %d, cant end with comma.\n", lineNum);
			return 1;
		}
	}

	else if(checkIfString(line))
	{
		line = getCharAfterWhiteSpace(line);
		line += 7; /*to skip the .string and get to the numbers*/
		line = getCharAfterWhiteSpace(line);

		/*check if the stirng is not to big"*/
		if(strlen(line) >35)
		{
			printf("ERROR! at line %d, the string is to big!!.\n", lineNum);
			return 1;

		}
		/*check if the stirng starts and ends with " */
		if(*line != '"')
		{
			printf("ERROR! at line %d, the string recieved by instruction \"string\" needs to start with \".\n", lineNum);
			return 1;
		}
		if(*getLastChar(line) != '"')
		{
			printf("ERROR! at line %d, the string recieved by instruction \"string\" needs to end with \".\n", lineNum);
			return 1;


		}

		/*check if we have to many quotes, since we don't want to have more then 2*/
		if(numOfQuotes(line)> 2)
		{

			printf("ERROR! at line %d, the string recieved is invalid, since in has to many quotes.\n", lineNum);
			return 1;

		}
		/*if code got to this part we don't have any errors*/
		return 0;
	}
	else if(checkIfEntry(line))
	{
		line = getCharAfterWhiteSpace(line);
		line += 6; /*to skip the .entry and get to the operand*/
		line = getCharAfterWhiteSpace(line);

		/*check if the entry is an command name, like "mov"*/
		if(isCommand(line))
		{
		 	printf("ERROR! at line %d, .entry cant recieve a command.\n", lineNum);
			return 1;
		}
		if(strlen(line) >60)
		{
			printf("ERROR! at line %d, the operand is to big.\n", lineNum);
			return 1;

		}
		/*check if the operand is a register*/
		if(isRegister(line))
		{
			printf("ERROR! at line %d, .entry cant recieve a register.\n", lineNum);
			return 1;
		}
		/*we cant have any commas, since a comma seperates the operand into 2.*/
		if(getNumOfCommas(line) != 0)
		{
			printf("ERROR! at line %d, .entry recieves only a single operand.\n", lineNum);
			return 1;
		}
		/*we want to get only 1 operand for this instruction*/
		if(numOfWordsInLine(line) != 1)
		{
			printf("ERROR! at line %d, .entry recieves a single operand.\n", lineNum);
			return 1;
		}
		if(!isLegalOperand(line))/*alphanumeric*/
		{
			printf("ERROR! at line %d, .entry's operand needs to start with a letter (alphanumeric).\n", lineNum);
			return 1;
		}

		/*if code got to this part we don't have any errors*/
		return 0;

	}
	else if(checkIfExtern(line))
	{
		line = getCharAfterWhiteSpace(line);
		line += 7; /*to skip the .entry and get to the numbers*/
		line = getCharAfterWhiteSpace(line);

		/*note that we have the same errors as .entry*/

		/*check if the entry is an command name, like "mov"*/
		if(isCommand(line))
		{
		 	printf("ERROR! at line %d, .extern can't recieve a command.\n", lineNum);
			return 1;
		}
		if(strlen(line) >60)
		{
			printf("ERROR! at line %d, the operand is to big.\n", lineNum);
			return 1;

		}
		if(isRegister(line))
		{
			printf("ERROR! at line %d, .extern cant recieve a register.\n", lineNum);
			return 1;
		}
		if(getNumOfCommas(line) != 0)
		{
			printf("ERROR! at line %d, .extern recieves only a single operand.\n", lineNum);
			return 1;
		}

		if(numOfWordsInLine(line) != 1)
		{
			printf("ERROR! at line %d, .extern recieves a single operand.\n", lineNum);
			return 1;
		}
		if(!isLegalOperand(line))
		{
			printf("ERROR! at line %d, .extern's operand needs to start with a letter (alphanumeric).\n", lineNum);
			return 1;
		}
		return 0;

	}
	else
	{

		return -1;

	}
	/*if code got to this part we don't have any errors*/
	return 0;

}
/*--------------------------------------------------------------------------------
this function gets a line. if the line is not a command type of line, we
retrun -1. if it is, we check it for all of the possible errors, for example if
we have a line "move #234,#535" we will report an error since its not the right
adressing method. if we found an error we return 1, if not we return 0.
	input:char* line- the line checked, int lineNum- the number of the line in
	the post macro file.
	return: int. -1 it is not an command line, 1 if we have an error 0 if no
----------------------------------------------------------------------------------*/
int checkForErrorCommand(char* line, int lineNum)
{
	/*variable decelerations*/
	int i = 0;
	int j = 0;
	/*the "clr", "not", "inc", "prn", "red" command errors*/
	char* mov = "mov";
	char* cmp = "cmp";
	char* add = "add";
	char* sub = "sub";
	char* lea = "lea";

	char* clr = "clr";
	char* not = "not";
	char* inc = "inc";
	char* dec = "dec";
	char* jmp = "jmp";
	char* red = "red";
	char* prn = "prn";
	char* bne = "bne";
	char* jsr = "jsr";
	char* stop = "stop";
	char* rts = "rts";
	int isMov = 0;
	int isCmp = 0;
	int isAdd = 0;
	int isSub = 0;
	int isLea = 0;
	int isClr = 0;
	int isNot = 0;
	int isInc = 0;
	int isDec = 0;
	int isJmp = 0;
	int isRed = 0;
	int isPrn = 0;
	int isBne = 0;
	int isJsr = 0;
	int isStop = 0;
	int isRts = 0;

	while (*(line + j) != '\n' && *(line + j) != EOF && *(line + j) != '\0')
		j++;
	if (j >= 80)
	{
		printf("ERROR! at line %d, the line is too long.\n", lineNum);
		return 1;
	}

	/* The line is empty/comment it has no errors by definition*/
	line = getCharAfterWhiteSpace(line);
	if (line[i] != '\0' && line[i] != '\n' && line[i] != EOF && line[i] == ';')
		return 0;
	/*we check if its a lable like LOOP:*/
	if(*(moveToWhiteSpace(line) - 1) == ':')
	{
		if(!((*line >= 'A' && *line <= 'Z')||(*line >= 'a' && *line <= 'z')))/*needs to be alphanumeric*/
		{
			printf("ERROR at line %d,a lable needs to start with a letter!\n",lineNum);
			return 1;
		}

		line = getNextWord(line);
	}
	/*find the type of instruction*/
	for (i = 0; i < 3; i++)
		if (mov[i] == *(line + i)&& isspace(*(line+3)))
			isMov = 1;
		else
		{
			isMov = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (cmp[i] == *(line + i)&& isspace(*(line+3)))
			isCmp = 1;
		else
		{
			isCmp = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (add[i] == *(line + i)&& isspace(*(line+3)))
			isAdd = 1;
		else
		{
			isAdd = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (sub[i] == *(line + i)&& isspace(*(line+3)))
			isSub = 1;
		else
		{
			isSub = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (lea[i] == *(line + i)&& isspace(*(line+3)))
			isLea = 1;
		else
		{
			isLea = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (clr[i] == *(line + i)&& isspace(*(line+3)))
			isClr = 1;
		else
		{
			isClr = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (not[i] == *(line + i)&& isspace(*(line+3)))
			isNot = 1;
		else
		{
			isNot = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (inc[i] == *(line + i)&& isspace(*(line+3)))
			isInc = 1;
		else
		{
			isInc = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if ((dec[i] == *(line + i))&& isspace(*(line+3)))
			isDec = 1;
		else
		{
			isDec = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (jmp[i] == *(line + i)&& isspace(*(line+3)))
			isJmp = 1;
		else
		{
			isJmp = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (red[i] == *(line + i)&& isspace(*(line+3)))
			isRed = 1;
		else
		{
			isRed = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (prn[i] == *(line + i)&& isspace(*(line+3)))
			isPrn = 1;
		else
		{
			isPrn = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (bne[i] == *(line + i)&& isspace(*(line+3)))
			isBne = 1;
		else
		{
			isBne = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (jsr[i] == *(line + i)&& isspace(*(line+3)))
			isJsr = 1;
		else
		{
			isJsr = 0;
			break;
		}
	for (i = 0; i < 4; i++)
		if (stop[i] == *(line + i)&& isspace(*(line+4)))
			isStop = 1;

		else
		{
			isStop = 0;
			break;
		}
	for (i = 0; i < 3; i++)
		if (rts[i] == *(line + i)&& isspace(*(line+3)))
			isRts = 1;
		else
		{
			isRts = 0;
			break;
		}

	/*----------------------------------------------------------------
	error check for the "mov", "cmp", "add", "sub", "lea" instructions
	------------------------------------------------------------------*/
	if (isMov || isCmp || isAdd || isSub || isLea)
	{

		/*skip to the next grop of chars after the command name*/
		line = getNextWord(line);
		/*this is the line that recives TWO operand, so if we have more then one it's an error*/
		if (numOfWordsInLine(line) != 2 && !(numOfWordsInLine(line) == 1 && hasCommaInMid(line)) && !(numOfWordsInLine(line) == 3 && hasAloneComma(line)))
		{
			printf("ERROR! at line %d, this instruction must recieve exactely two operand.\n", lineNum);
			return 1;
		}

		/*we don't want a comma between the instruction and the operand*/
		if (*line == ',')
		{
			printf("ERROR! at line %d, the operands and instruction shoudn't be seperated by comma.\n", lineNum);
			return 1;

		}
		/*we dont want there to be 2 commas next to each other*/
		if (*(moveToComma(line) + 1) == ',')
		{
			printf("ERROR! at line %d, we dont want to have to commas by each other.\n", lineNum);
			return 1;


		}
		/*we have no comma seperating the 2 operands that this instruction recieves*/
		if (*moveToComma(line) != ',' || endsInComma(line))
		{
			printf("ERROR! at line %d, for this instruction we need a comma to seperate the 2 operands.\n", lineNum);
			return 1;
		}

		if (getNumOfCommas(line) > 1)
		{
			printf("ERROR! at line %d,to many commas.\n", lineNum);
			return 1;
		}
		if (endsInComma(line))
		{
			printf("ERROR! at line %d,we cant end with a comma.\n", lineNum);
			return 1;
		}

		if (isLea)
		{
			if (isAdressinMethod3(line))
			{
				printf("ERROR! at line %d, this instruction cant recieve the first operand with adressing method 3.\n", lineNum);
				return 1;
			}
			if (isAdressinMethod0(line))
			{
				printf("ERROR! at line %d, this instruction cant recieve the first operand with adressing method 0.\n", lineNum);
				return 1;
			}

			/*checking if the second operand is the right adressing method*/
			/************************************************************/
			line = moveToComma(line);
			if (*line != '\0'&& *line != '\n' && *line != EOF)
			{
				line++;
				if (isAdressinMethod0(line))
				{
					printf("ERROR! at line %d, this instruction cant recieve the second operand with adressing method 0.\n", lineNum);
					return 1;
				}
			}
		}
		if (isMov || isAdd || isSub)
		{
			/*checking if the second operand is the right adressing method*/
			/************************************************************/
			line = moveToComma(line);
			if (*line != '\0'&& *line != '\n' && *line != EOF)
			{
				line++;
				if (isAdressinMethod0(line))
				{
					printf("ERROR! at line %d, this instruction cant recieve the second operand with adressing method 0.\n", lineNum);
					return 1;
				}
			}
		}

	}

	/*------------------------------------------------------------------------------------
	error check for the "clr", "not", "inc", "jmp", "red","prn", "bne", "jsr" instructions
	-------------------------------------------------------------------------------------*/
	else if (isClr || isNot || isInc || isDec || isJmp || isRed || isPrn || isBne || isJsr)
	{

		/*skip to the next grop of chars after the command name*/
		line = getNextWord(line);


		/*this is the line that recives only ONE operand, so if we have more then one it's an error*/
		if (numOfWordsInLine(line) != 1 || (numOfWordsInLine(line) == 1 && hasCommaInMid(line)))
		{
			printf("ERROR! at line %d, this instruction must recieve exactely one operand.\n", lineNum);

			return 1;

		}
		if (getNumOfCommas(line) != 0)
		{
			printf("ERROR! at line %d, this instruction must recieve exactely one operand.\n", lineNum);

			return 1;

		}
		if (endsInComma(line))
		{
			printf("ERROR! at line %d, the operand can't end with a comma.\n", lineNum);
			return 1;
		}

		/*the operand is not alowed to start w/ a comma.*/
		if (*line == ',')
		{
			printf("ERROR! at line %d, the operand and instruction shoudn't be seperated by comma.\n", lineNum);
			return 1;
		}
		/*the operand that this instruction recieves can't be an instruction*/
		if (isCommand(line))
		{

			printf("ERROR! at line %d, we cant have an instruction be an operand.\n", lineNum);
			return 1;
		}

		/*we check if the addressing method is legal depending on the command*/
		if ((isClr || isNot || isInc || isDec || isJmp || isBne || isJsr || isRed) && isAdressinMethod0(line))
		/*0 cant be the adressing methods for these*/
		{

			printf("ERROR! at line %d, this instruction can't recieve an operand with adressing method 0.\n", lineNum);
			return 1;

		}

		if (isJmp || isBne || isJsr)/*3 cant be the adressing methods for these commands*/
		{
			if (isAdressinMethod3(line))
			{
				printf("ERROR! at line %d, this instruction can't recieve an operand with adressing method 3.\n", lineNum);
				return 1;

			}
		}

		line = getCharAfterWhiteSpace(line);
		if (*(line - 1) == ',')
		{
			printf("ERROR! at line %d, there cant be a comma at the end of the last operand.\n", lineNum);
			return 1;
		}

	}
	/*-------------------------------------------
	error check for the "rts", "stop" instructions.
	---------------------------------------------*/

	else if (isRts || isStop)
	{
		line = getNextWord(line);

		/*these commands recieve no operands, if they recieve any operands its an error.*/
		if (numOfWordsInLine(line) != 0)
		{
			printf("ERROR! at line %d, this method should recieve no operands.\n", lineNum);
			return 1;
		}
	}

	else
	{

		return -1;/*not a command*/

	}


	/*if we got to this part of the code then we have no errors*/
	return 0;
}


/*--------------------------------------------------------------------------------
this function checks if the next word in line is a command.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int isCommand(char* line)
{
	/*variable decelerations*/
	int i = 0;
	int j = 0;
	int inconsistant = 0;
	int isInstr = 0;
	char instructions[16][5] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "red", "prn", "bne", "jsr", "rts", "stop" };

	line = getCharAfterWhiteSpace(line);
	for (i = 0; i < 15; i++)
	{

		for (j = 0; j <= 3 && !inconsistant; j++)
		{
			if ((line[j] != instructions[i][j]))
				inconsistant = 1;
			if (j == 3)
			{
				if (line[j] == '\0' || line[j] == ' ' || line[j] == '\t' || line[j] == ',' || line[j] == '\n' || line[j] == EOF || line[j] == ',')
				/*we don't want somthing in the shape of movxx*/
				{
					isInstr = 1;

				}
			}

		}
		inconsistant = 0;


	}
	i++;
	/*check if it is "stop"*/
	for (j = 0; j <= 4 && !inconsistant; j++)
	{
		if ((line[j] != instructions[i][j]))
			inconsistant = 1;
		if (j == 4)
		{
			if (line[j] == '\0' || line[j] == ' ' || line[j] == '\t' || line[j] == ',' || line[j] == '\n' || line[j] == EOF || line[j] == ',')
				isInstr = 1;
		}

	}

	return isInstr;

}
/*--------------------------------------------------------------------------------
this function gets looks at the first operand of the line and checks if it is
adressing method 0.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int isAdressinMethod0(char* line)
{
	/*variable decelerations*/
	int is0 = 0;
	int i = 0;
	line = getCharAfterWhiteSpace(line);


	if (*line == '#')
	{
		line++;
		if (*(line) == '-'||*line=='+')/*we are alowed to get negative numbers in adressing method 0.*/
			line++;
		while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF && line[i] != '\0'&& line[i] != ',')
		{
			if (line[i] <= '9' && line[i] >= '0')
			{
				is0 = 1;
				i++;
			}
			else
			{
				is0 = 0;
				break;

			}

		}
	}

	return is0;
}
/*--------------------------------------------------------------------------------
this function gets looks at the first operand of the line and checks if it is
adressing method 0.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int isAdressinMethod3(char* line)
{

	if (isRegister(line))
	{
		return 1;
	}
	return 0;



}
/*--------------------------------------------------------------------------------
this function gets a line and checks if it is an instruction of type data.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int checkIfData(char *line)
{
	/*variable decelerations*/
	int isData = 1;
	int i = 0;
	char* da = ".data";
	line = getCharAfterWhiteSpace(line);

	if(strlen(line) < strlen(da))
		return 0;

	while(i < 5)
	{
		if(*(line+i) != *(da+i))
			isData = 0;

		i++;

	}
	if(*(line+i) != ' ' && *(line+i) != '\n' && *(line+i) != '\t' && *(line+i)!= '\0' && *(line+i) != EOF)
		isData = 0;

	return isData;
}

/*--------------------------------------------------------------------------------
this function gets a line and finds the number of diffrent numbers seperated by a
comma or white space.
	input:char* line- the line checked.
	return: int. the number of numbers. 0 if we found a none-legal number.
----------------------------------------------------------------------------------*/
int numOfNums(char* line)
{
	/*variable decelerations*/
	int numOf = 0;

	line = getCharAfterWhiteSpace(line);

	if (has2CommasByEachOther(line))
	{

		return 0;
	}

	if (endsInComma(line))
	{
		return 0;
	}

	while (*line != EOF && *line != '\0' && *line != '\n')/*while not at end of line*/
	{

		if (isNumUntilWhiteSpace(line))
		{

			numOf++;

		}
		else
		{

			return 0;
		}

		line = moveToComma(line);

		if( !(*line !=  '\n' && *line !=  EOF && *line !=  '\0'))/*at end of line*/
			break;
		if (*line == ',' && (*(line + 1) == ' ' || *(line + 1) == '\t'))
		{
			line++;
			line = getCharAfterWhiteSpace(line);
		}
		else
			line++;
		line = getCharAfterWhiteSpace(line);


	}

	return numOf;


}

/*--------------------------------------------------------------------------------
this function gets a line and checks if it is an instruction of type string.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int checkIfString(char *line)
{
	/*variable decelerations*/
	int isString = 1;
	int i = 0;
	char* str = ".string";

	line = getCharAfterWhiteSpace(line);

	if(strlen(line) < strlen(str))
		return 0;

	while(i < 7)
	{
		if(*(line+i) != *(str+i))
			isString = 0;

		i++;

	}
	if(*(line+i) != ' ' && *(line+i) != '\n' && *(line+i) != '\t' && *(line+i)!= '\0' && *(line+i) != EOF)
		isString = 0;

	return isString;
}

/*--------------------------------------------------------------------------------
this function gets a line and checks if it is an instruction of type entry.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int checkIfEntry(char* line)
{
	int isEntry = 1;
	int i = 0;
	char* entry = ".entry";
	line = getCharAfterWhiteSpace(line);

	if(strlen(line) < strlen(entry))
		return 0;

	while(i < 6)
	{
		if(*(line+i) != *(entry+i))
			isEntry = 0;

		i++;

	}
	if(*(line+i) != ' ' && *(line+i) != '\n' && *(line+i) != '\t' && *(line+i)!= '\0' && *(line+i) != EOF)
		isEntry = 0;

	return isEntry;
}
/*--------------------------------------------------------------------------------
this function gets a line and checks if it is an instruction of type extern.
	input:char* line- the line checked.
	return: int. 1 if yes, 0 if no
----------------------------------------------------------------------------------*/
int checkIfExtern(char* line)
{
	int isExtern = 1;
	int i = 0;
	char* exter = ".extern";
	line = getCharAfterWhiteSpace(line);

	if(strlen(line) < strlen(exter))
		return 0;

	while(i < 7)
	{
		if(*(line+i) != *(exter+i))
			isExtern = 0;

		i++;

	}
	if(*(line+i) != ' ' && *(line+i) != '\n' && *(line+i) != '\t' && *(line+i)!= '\0' && *(line+i) != EOF)
		isExtern = 0;

	return isExtern;
}
