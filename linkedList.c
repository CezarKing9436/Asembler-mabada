#include "Defenitions.h"



#define IS_BIT_SET(BF,N) (((BF)>>N)&0x1)/*checks the n-th bit in a number*/
/*--------------------------------------------------------------------------------
this function adds a node from type info to the list.
	input:char* line-the line we copy to the node, struct info **tail-the
	tail of the list we add to.
	return: void
----------------------------------------------------------------------------------*/
void addinfo(char* line, struct info **tail)
{
	/*variable decelerations*/
  	struct info *newNode = malloc(sizeof(struct info));
	checkIfAllocated(newNode);/*tested malloc*/

	/*update the tail*/
  	strcpy(newNode->info,line);
  	newNode->next = NULL;

  	if(*tail==NULL)/**/
	{
    		*tail = newNode;
    		return;
  	}
  	(*tail)->next=newNode;
}

/*--------------------------------------------------------------------------------
this function adds an node from type macro to a list.
	input:char* name- the name of the macro,struct info *infoHead- the info
	list that will be copied,struct macro** head- the list we add to.
	return: void
----------------------------------------------------------------------------------*/
void addNode(char* name,struct info *infoHead,struct macro** head)/* not a great name maybe : addMacro*/
{
	/*variable decelerations*/
	struct macro *newNode = malloc(sizeof(struct macro));
 	struct info *tailInfo =NULL;
  	checkIfAllocated(newNode);/*tested malloc*/

 	strcpy(newNode->name,name);

  	if(infoHead!=NULL)
	{
    		addinfo(infoHead->info,&(newNode->info));
    		infoHead = infoHead->next;
   		 tailInfo = newNode->info;
  	}
	/* copys the info list that was sent to the struct*/
  	while(infoHead!=NULL)
	{
    		addinfo(infoHead->info,&(tailInfo));
    		infoHead = infoHead->next;
    		tailInfo = tailInfo->next;

  	}

  	newNode->next = NULL;/*update the next node to NULL*/

  	if((*head==NULL))/*the linked list is empty*/
	{
    		*head=newNode;
    		return;
  	}
  	(*head)->next=newNode;
}
/*--------------------------------------------------------------------------------
this function adds an node from type wordExtra to a "word list"
	input:struct wordsList** wordList- th list we add to,int* counter- the counter of encoded line,int number-the number encoded
	return: struct wordsList**. the last node added
----------------------------------------------------------------------------------*/
struct wordsList** addWord3(struct wordsList** wordList,int* counter,int number)
{
	/*variable decelerations*/
  	int i;
  	wordsList *newNode = malloc(sizeof(wordsList));
  	checkIfAllocated(newNode);/*tested malloc*/

  	/*set up all the values*/
  	newNode->next = NULL;
  	newNode->wordExtra = malloc(sizeof(wordExtra));
	checkIfAllocated(newNode->wordExtra);
  	newNode->wordExtra->A=1;
  	newNode->wordExtra->R=0;
  	newNode->wordExtra->E=0;
  	newNode->wordCommand=NULL;
  	newNode->wordInformation=NULL;

  	/* create the 16 bits for the number*/
  	for (i = 0; i < 16; i++)
	{
    		if(IS_BIT_SET(number,i))
		{
      			newNode->wordExtra->address[i]=TR;
    		}
		else
		{
      			newNode->wordExtra->address[i]=FL;
    		}
  	}
  	newNode->counter=*counter;
  	if(*wordList==NULL)
	{
      		*wordList = newNode;
      		return wordList;
  	}

	/*if code reches this part then *wordList ain't NULL*/
  	(*wordList)->next=newNode;
  	wordList = &(*wordList)->next;

  	return wordList;
}
/*--------------------------------------------------------------------------------
this function adds an node from type symbolNode to the symbol list
	input:struct symbolNode** symbolList- the symbol array we add to,char*
	name-the name of the symbol
	return: void
----------------------------------------------------------------------------------*/
void addExtern(struct symbolNode** symbolList,char* name)
{
	/*variable decelerations*/
  	int i;
  	int counter=0;
  	char* startName;
  	symbolNode *newNode = malloc(sizeof(symbolNode));
	checkIfAllocated(newNode);/*checked malloc*/

  	/*at the beggining set up the name*/
  	name = moveToWhiteSpace(name);
  	name = getCharAfterWhiteSpace(name);
  	startName = name;

  	while(*name!='\n')
	{
    		counter++;
    		name++;
  	}

  	strncpy(newNode->name,startName,counter);
  	newNode->name[counter]='\0';/*adding the NULL char to the end*/

  	/*set up the other values*/
  	newNode->base = 0;
 	newNode->offset = 0;
  	for (i = 0; i < 4; i++)
	{
    		if(i==EXTERNAL)
		{
      			newNode->attributes[EXTERNAL]=TR;
    		}
    		else
		{
      			newNode->attributes[i]=FL;
    		}
  	}
  	newNode->next =NULL;

  	/*connect the node*/
  	if(*symbolList==NULL)
	{
      		*symbolList = newNode;
      		return;
  	}

  	while((*symbolList)->next!=NULL)
	{
    		symbolList = &(*symbolList)->next;
  	}

  	(*symbolList)->next=newNode;
}
/*--------------------------------------------------------------------------------
	this function adds an node from type symbolNode to the symbol list
	input:struct symbolNode** symbolList-the list we add to,char* name-the name of the symbol, int* DC-it base(not final),int attributes-the attributes it has
	return: nothing
----------------------------------------------------------------------------------*/
void addSymbol(struct symbolNode** symbolList,char* name, int* DC,int attributes)
{
  	int i;
  	int counter=0;
  	char* startName = name;
  	symbolNode *newNode = malloc(sizeof(symbolNode));
  	checkIfAllocated(newNode);/*tested allocation*/

  	/*set up the valeus*/
  	while(*name!=':')/*not at the end of the lable*/
	{
    		counter++;
    		name++;
  	}
  	strncpy(newNode->name,startName,counter);
  	newNode->name[counter]='\0';/*adding the NULL char to the end*/
  	newNode->base = *DC;
  	newNode->offset = 0;

  	for (i = 0; i < 4; i++)
	{
    		if(attributes==i)
		{
      			newNode->attributes[attributes]=TR;
    		}
    		else
		{
      			newNode->attributes[i]=FL;
    		}
  	}

  	/*connect the node*/
  	newNode->next =NULL;
  	if(*symbolList==NULL)
	{
      		*symbolList = newNode;
      		return;
  	}
	/*if the code didn't return, then *symbolList is not NULL*/
  	while((*symbolList)->next!=NULL)
	{
    		symbolList = &(*symbolList)->next;
  	}
  	(*symbolList)->next=newNode;
}
/*--------------------------------------------------------------------------------
This function adds an node from type wordCommand to the wordList.
	input:struct wordsList** wordList-th list we add to,int* IC-the counter
	of encoded node,int command-the command we encode
	return: struct wordsList** the last node added
----------------------------------------------------------------------------------*/
struct wordsList** addZeroOperand(struct wordsList** wordList,int* IC,int command)
{
	/*variable decelerations*/
  	wordsList *newNode = malloc(sizeof(wordsList));

  	struct command commands[] = {{"mov",0,0},{"cmp",1,0}, {"add",2,10}, {"sub",2,11},{"lea",4,0},{"clr",5,10},	{"not",5,11},{"inc",5,12},{"dec",5,13},
  {"jmp",9,10},{"bne",9,11},{"jsr",9,12},{"red",12,0},{"prn",13,0},{"rts",14,0},{"stop",15,0}};
  checkIfAllocated(newNode);/*tested malloc*/


 	/* set up the values*/
  	newNode->next = NULL;
  	newNode->wordCommand = malloc(sizeof(wordCommand));
	checkIfAllocated(newNode->wordCommand);
  	newNode->wordCommand->A=1;
  	newNode->wordCommand->R=0;
  	newNode->wordCommand->E=0;
  	newNode->wordCommand->opcode = commands[command].code;
  	newNode->counter=*IC;
  	newNode->wordInformation=NULL;
  	newNode->wordExtra=NULL;

  	/*connect to the list*/
  	if(*wordList==NULL)
	{
    		*wordList = newNode;
    		return wordList;
  	}
	/*if the code didn't return, then *wordlist is not NULL*/

	/*place in the list properly*/
  	(*wordList)->next=newNode;
  	wordList = &(*wordList)->next;
  	return wordList;
}
/*--------------------------------------------------------------------------------
	this function adds an node from type wordInformation to the wordList
	input:struct wordsList** wordList-the list we add to,int* IC- the counter of encoded node,int command-the command indicates the func,
  int destanationRegister-the destanationRegister,int destanationAddressing- the destanationAddressing
	return: struct wordsList** the last node added
----------------------------------------------------------------------------------*/
struct wordsList** addOneOperand(struct wordsList** wordList,int* IC,int command,int destanationRegister,int destanationAddressing)
{
	/*variable decelerations*/
  	wordsList *newNode = malloc(sizeof(wordsList));

  	struct command commands[] = {{"clr",5,10},{"not",5,11},{"inc",5,12},{"dec",5,13},{"jmp",9,10},{"bne",9,11},{"jsr",9,12},{"red",12,0},{"prn",13,0}};
checkIfAllocated(newNode);/*tested malloc*/
  	/* set up the values*/
	newNode->next = NULL;
	newNode->wordInformation = malloc(sizeof(wordInformation));
	checkIfAllocated(newNode->wordInformation);
	newNode->wordInformation->A=1;
	newNode->wordInformation->R=0;
	newNode->wordInformation->E=0;
	command = command -5;/*a correction for the index must be done*/
	/*find the right funct*/
	newNode->wordInformation->funct = commands[command].funct;
	newNode->wordInformation->source_register=0;
	newNode->wordInformation->source_addressing=0;
	newNode->wordInformation->destanation_register = destanationRegister;
	newNode->wordInformation->destanation_addressing = destanationAddressing;
	newNode->wordCommand=NULL;
	newNode->wordExtra=NULL;
	newNode->counter=*IC;

  	/*connect to the list*/
  	if(*wordList==NULL)
	{
    		*wordList = newNode;
    		return wordList;
  	}
	/*if the code didn't return, then *wordList is not NULL*/
	 (*wordList)->next=newNode;
	 wordList = &(*wordList)->next;
	 return wordList;
}
/*--------------------------------------------------------------------------------
this function adds an node from type wordExtra to the wordList
	input:struct wordsList** wordList-the list we add to,int* IC- the counter
	of encoded node
	return: struct wordsList** the last node added
----------------------------------------------------------------------------------*/
struct wordsList** addEmptyInstruction(struct wordsList** wordList,int* IC)
{
	/*variable decelerations*/
  	wordsList *newNode = malloc(sizeof(wordsList));
  	int i;
  	checkIfAllocated(newNode);/*tested malloc*/
  	newNode->next = NULL;
  	newNode->wordExtra = malloc(sizeof(wordExtra));
  	checkIfAllocated(newNode);/*tested malloc*/

	/* set up the values*/
  	newNode->wordExtra->A=0;
  	newNode->wordExtra->R=0;
  	newNode->wordExtra->E=0;
  	newNode->wordCommand=NULL;
  	newNode->wordInformation=NULL;
  	for (i = 0; i < 16; i++)
	{
      		newNode->wordExtra->address[i]=FL;
  	}
  	/*connect to the lsit*/
  	newNode->counter=*IC;
  	if(*wordList==NULL)
	{
    		*wordList = newNode;
    		return wordList;
  	}
	/*if the code didn't return, then *wordList is not NULL*/
  	(*wordList)->next=newNode;
  	wordList = &(*wordList)->next;
  	return wordList;
}
/*--------------------------------------------------------------------------------
this function adds an node from type wordInformation to the wordList.
	input:struct wordsList** wordList-the list we add to,int* IC- the
	counter of encoded node,int command-the command indicates the func,
  	int sourceRegister-the sourceRegister,int sourceAddressing-the
	sourceAddressing, int destanationRegister-the destanationRegister,int
	destanationAddressing- the destanationAddressing
	return: struct wordsList** the last node added
----------------------------------------------------------------------------------*/
struct wordsList** addTwoOperand(struct wordsList** wordList,int* IC,int command,int sourceRegister,int sourceAddressing,int destanationRegister,int destanationAddressing)
{
	/*variable decelerations*/
	wordsList *newNode = malloc(sizeof(wordsList));
	struct command commands[] = {{"mov",0,0},{"cmp",1,0},{"add",2,10},{"sub",2,11},{"lea",4,0}};
	checkIfAllocated(newNode);/*tested malloc*/

	/*set up the values*/
	newNode->next = NULL;
	newNode->wordInformation = malloc(sizeof(wordInformation));
	checkIfAllocated(newNode->wordInformation);
	newNode->wordInformation->A=1;
	newNode->wordInformation->R=0;
	newNode->wordInformation->E=0;
	newNode->wordInformation->funct = commands[command].funct;
	newNode->wordInformation->source_register=sourceRegister;
	newNode->wordInformation->source_addressing=sourceAddressing;
	newNode->wordInformation->destanation_register = destanationRegister;
	newNode->wordInformation->destanation_addressing = destanationAddressing;
	newNode->wordCommand=NULL;
	newNode->wordExtra=NULL;
	newNode->counter=*IC;

  	/*connect to the list*/
  	if(*wordList==NULL)
	{
    		*wordList = newNode;
    		return wordList;
  	}

	/*if the code didn't return, then *wordList is not NULL*/
  	(*wordList)->next=newNode;
  	wordList = &(*wordList)->next;
  	return wordList;
}
