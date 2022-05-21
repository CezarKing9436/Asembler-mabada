#include "Defenitions.h"
#define IS_BIT_SET(BF,N) (((BF)>>N)&0x1)/* checks the n'th bit in a number*/
/*--------------------------------------------------------------------------------
This function turns the k-th bit into a number
	input:int* n-the number we change, int k-the bit we change
	return: void
----------------------------------------------------------------------------------*/
void turnK(int* n, int k)
{
     *n = ((1<<k)| *n);
}
/*--------------------------------------------------------------------------------
This function prints to file the wordCommand encoded. this is for the object file.
We are printing the word type of wordCommand, as explained in the readME file.
	input: wordCommand* word- the word we encode,FILE* file-the file we print
	to.
	return: void.
----------------------------------------------------------------------------------*/
void printWordCommand(wordCommand* word,FILE* file)
{
	/*variable decelerations*/
	int mask = 0;
	int i;
	int num = word->A;
  	char letter = 'E';
  	char saveLetter;
  	int found=0;

	/*look at A*/
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,2);
  	}

	/*look at R*/
  	num = word->R;
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,1);
  	}

	/*look at E*/
  	num = word->E;
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,0);
  	}

  	fprintf(file,"A%01x-",mask);/*printing to the .ob file*/

	/*look at opcode*/
  	mask=0;
  	num = word->opcode;
  	while(!found)
	{
    		if(num>3)
		{
      			letter--;
      			num-=4;
    		}
    		else
		{
      			found =1;/*stop any further loop*/
      			for (i = 0; i < 4; i++)
			{
        			if(i==num)
				{
          				turnK(&mask,i);
          				saveLetter=letter;
          				break;
        			}
      			}
    		}
  	}

  	letter= 'B';
  	while(letter<'F')
	{
    		if(letter==saveLetter)
		{
      			if(letter!='E')
			{
        			fprintf(file,"%c%01x-",letter,mask );/*printing to the .ob file*/
      			}
      			else
			{
        			fprintf(file,"%c%01x",letter,mask );/*printing to the .ob file*/
      			}
   		}
    		else
		{
      			if(letter!='E')
			{
        			fprintf(file,"%c0-",letter);/*printing to the .ob file*/
      			}
      			else
			{
        			fprintf(file,"%c0",letter);/*printing to the .ob file*/
      			}
   		}
    		letter++;
  	}
  	fprintf(file,"\n");/*go down a line*/
}
/*--------------------------------------------------------------------------------
this function prints to file the wordInformation encoded. We are printing the word
type of wordInformation, as explained in the readME file.
	input:wordInformation* word- the word we encode,FILE* file-the file we print to
	return: nothing
----------------------------------------------------------------------------------*/
void printWordInformation(wordInformation* word,FILE* file)
{
	/*variable decelerations*/
  	int mask = 0;
  	int i;
  	int num = word->A;

	/*look at A*/
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,2);
  	}

	/*look at R*/
  	num = word->R;
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,1);
  	}

	/*look at E*/
  	num = word->E;
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,0);
  	}

  	fprintf(file,"A%01x-",mask);/*printing to the .ob file*/

	/*looking at the funct of the word*/
  	mask =0;
  	num = word->funct;
  	for (i = 0; i < 4; i++)
	{
    		if(IS_BIT_SET(num,i))
		{
     			turnK(&mask,i);
    		}
  	}

  	fprintf(file,"B%01x-",mask );/*printing to the .ob file*/

	/*looking at the source register*/
  	mask=0;
  	num = word->source_register;
  	for (i = 0; i < 4; i++)
	{
    		if(IS_BIT_SET(num,i))
		{
      			turnK(&mask,i);
    		}
  	}
  	fprintf(file,"C%01x-",mask);/*printing to the .ob file*/

	/*looking at the source adressing method*/
	mask =0;
  	num = word->source_addressing;
  	for (i = 0; i < 2; i++)
	{
    		if(IS_BIT_SET(num,i))
		{
      			turnK(&mask,i+2);
    		}
  	}
	/*looking at the destenation register*/
	num = word->destanation_register;
  	for (i = 0; i < 2; i++)
	{
    		if(IS_BIT_SET(num,(i+2)))
		{
      			turnK(&mask,i);
    		}
  	}
  	fprintf(file,"D%01x-",mask);/*printing to the .ob file*/

  	mask =0;
  	for (i = 0; i < 2; i++)
	{
    		if(IS_BIT_SET(num,i))
		{
      			turnK(&mask,i+2);
    		}
  	}
	/*looking at the destanation adressing method*/
  	num = word->destanation_addressing;
  	for (i = 0; i < 2; i++)
	{
    		if(IS_BIT_SET(num,i))
		{
      			turnK(&mask,i);
    		}
  	}
	fprintf(file,"E%01x\n",mask);/*printing to the .ob file*/
}
/*--------------------------------------------------------------------------------
this function prints to file the wordExtra encoded. We are printing the word
type of wordExtra, as explained in the readME file.
	input:wordExtra* word- the word we encode,FILE* file-the file we print to
	return: void.
----------------------------------------------------------------------------------*/
void printWordExtra(wordExtra* word,FILE* file)
{
	/*variable decelerations*/
	int mask = 0,counter=0;
	int i;
	int num;
	char letter = 'B';

	/*looking at A*/
	num= word->A;
	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,2);
  	}

	/*looking at R*/
  	num = word->R;
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,1);
  	}

	/*looking at E*/
  	num = word->E;
  	if(IS_BIT_SET(num,0))
	{
    		turnK(&mask,0);
  	}
  	fprintf(file,"A%01x-",mask);/*printing to the .ob file*/


  	mask=0;
 	for (i = 15; i > -1; i--)
	{
    		if(word->address[i]==TR)
		{
      			turnK(&mask,i%4);
    		}

    		counter++;
    		if(counter==4)
		{
      			counter=0;
      			if(letter =='E')
			{
        			fprintf(file,"%c%01x\n",letter,mask);/*printing to the .ob file*/
      			}
     			else
			{
        			fprintf(file,"%c%01x-",letter,mask);/*printing to the .ob file*/
      			}
      			letter++;/*moving forward*/
      			mask=0;/*reset mask to 0*/
    		}
  	}
}
/*--------------------------------------------------------------------------------
this function analyzez the line, and adds the proper words depending on the context
this is if we are dealing with a 2 operand type of command.
note that this is for the second pass.
	input:struct wordsList** wordList-the encoded list,int* IC-the counter,int
	command - the command chosen, char* line - the line
	return: struct wordsList**. the last encoded line added
----------------------------------------------------------------------------------*/
struct wordsList** analyzeTwoOperand(struct wordsList** wordList,int* IC,int command,char* line)
{
	/*variable decelerations*/
	int source[2] ={0,0};
  	int destenation[2] ={0,0};
  	int immediateNumber;
	char* startFirstOperand;
	char* startSecondOperand;

	/*at the start we skip the first word, after that we get the information about the operand and we store it
	after that we skip the operand and analyze the second operand */
 	line = getNextWord(line);
	startFirstOperand = line;
  	findAdressingMethod(line,source);

	/*we want to skip the comma*/
  	while(*line!=',')
	{
    		line++;
  	}
  	line++;

	/*skip the "white space"*/
  	if(*line==' '||*line=='\t')
	{
    		line = getCharAfterWhiteSpace(line);
  	}

	/*now we got to the second operand*/
	startSecondOperand = line;
  	findAdressingMethod(line,destenation);

	/*at the start we add the 2 first words every command that has 2 operand needs*/
    	wordList = addZeroOperand(wordList,IC,command);
    	(*IC)++;
    	wordList = addTwoOperand(wordList,IC,command,source[0],source[1],destenation[0],destenation[1]);
    	(*IC)++;

	/*if a label is being used we add 2 words that will contain the address of the word
	 if we get a number starting with # we add the number to the wordList encoded*/
  	if(source[1]==RELATIVE_ADDR||source[1]==DIRECT_ADDR)
	{
		/*since we have to operands the IC goes up twice*/
    		wordList = addEmptyInstruction(wordList,IC);
    		(*IC)++;
    		wordList = addEmptyInstruction(wordList,IC);
    		(*IC)++;
  	}
  	else if (source[1] == IMMEDIATE_ADDR)
	{
		line = startFirstOperand;
    		line++;
		/* if the word beigns with '-' or '+' we need to split the cases*/
    		if(*line=='-')
		{
      			line++;
      			immediateNumber =- numFromLine(&line);
    		}
		else if(*line=='+')
		{
			line++;
			immediateNumber = numFromLine(&line);
		}
    		else
		{
      			immediateNumber = numFromLine(&line);
    		}
    		wordList = addWord3(wordList,IC,immediateNumber);
    		(*IC)++;
  	}
  	if(destenation[1]==RELATIVE_ADDR||destenation[1]==DIRECT_ADDR)
	{
   		wordList = addEmptyInstruction(wordList,IC);
    		(*IC)++;
    		wordList = addEmptyInstruction(wordList,IC);
    		(*IC)++;
  	}
  	else if (destenation[1] == IMMEDIATE_ADDR)
	{
		line = startSecondOperand;
    		line++;

		/* if the word beigns with '-' or '+' we need to split the cases*/
    		if(*line=='-')
		{
      			line++;
      			immediateNumber =- numFromLine(&line);
    		}
		else if(*line=='+')
		{
			line++;
      			immediateNumber = numFromLine(&line);
		}
    		else
		{
      			immediateNumber = numFromLine(&line);
    		}
   		wordList = addWord3(wordList,IC,immediateNumber);
    		(*IC)++;
  	}
  	return wordList;
}
/*--------------------------------------------------------------------------------
this function analyzez the line, and adds the proper words depending on the context.
this is if we are dealing with a 1 operand type of command.
	input:struct wordsList** wordList-the encoded list,int* IC-the counter,
	int command- the command chosen, char* line - the line.
	return: struct wordsList**. the last encoded line added
----------------------------------------------------------------------------------*/
struct wordsList** analyzeOneOperand(struct wordsList** wordList,int* IC,int command,char* line)
{
	/*variable decelerations*/
  	int addresing[2] ={0,0};
  	int immediateNumber;
  	line = getNextWord(line);
  	findAdressingMethod(line,addresing);/* this function gets the addresing array that holds the
						addresing register and method*/
  	/* at the start we add the word that contains the number of the command and then we add the second
	words that contains the funct value and more*/

	/*at the start we add the 2 first words every command that has 1 operand needs*/
  	wordList = addZeroOperand(wordList,IC,command);
  	(*IC)++;
  	wordList = addOneOperand(wordList,IC,command,addresing[0],addresing[1]);
  	(*IC)++;
  	/*if a label is being used we add 2 words that will contain the address of the word
    	if we get a number starting with # we add the number to the wordList encoded*/
  	if(addresing[1]==RELATIVE_ADDR||addresing[1]==DIRECT_ADDR)
	{
    		wordList = addEmptyInstruction(wordList,IC);
    		(*IC)++;
    		wordList = addEmptyInstruction(wordList,IC);
    		(*IC)++;
  	}
  	else if (addresing[1] == IMMEDIATE_ADDR)
	{
    		line++;
		/* if the word beigns with '-' or '+' we need to split the cases*/
    		if(*line=='-')
		{

      			line++;
      			immediateNumber =- numFromLine(&line);
    		}
		else if(*line=='+')
		{
			line++;
			immediateNumber = numFromLine(&line);
		}

    		else
		{
     			 immediateNumber = numFromLine(&line);
    		}
    		wordList = addWord3(wordList,IC,immediateNumber);
    		(*IC)++;/*move the IC count up.*/
  	}
 	return wordList;
}
/*--------------------------------------------------------------------------------
this function analyzez the line, and returns the addresing method and register.
	input:char* line - the line,int* address- the array that holds the values
	return: nothing(but changes the array given)
----------------------------------------------------------------------------------*/
void findAdressingMethod(char* line, int* address)
{
	/*variable decelerations*/
  	int addresing[2] = {0,0};/* addresing[0] = Register , addresing[1]= addresing*/
  	int hasBrackets=0;

	/*here we check the register and addresing method that is being used*/

	/* we check each option one by one*/
  	if(isRegister(line))
	{
    		line=line+1;
    		addresing[0]=numFromLine(&line);
    		addresing[1] = 3;
  	}
  	else if(*line=='#')
	{
    		line=line+1;
    		addresing[0]=0;
    		addresing[1] = 0;
  	}
  	else
	{
    		while(*line!='\0'&&*line!='\n')/*not at the end of the line*/
		{
      			if(*line=='[')
			{
        			addresing[0] = 10+*(line+3)-'0';
       				addresing[1] = 2;
        			hasBrackets=1;
        			break;
      			}
      		line++;
    		}
    		if(!hasBrackets)
		{
      			addresing[0] = 0;
      			addresing[1] = 1;
   		}
  	}
  	address[0]=addresing[0];
  	address[1]=addresing[1];
}
