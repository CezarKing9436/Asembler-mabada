#include "Defenitions.h"
void createObEntFiles(struct wordsList** wordList,struct symbolNode** symbolList,char* name,int IC,int DC);
void printWords(struct wordsList** wordList,char* name,int IC,int DC);
int hasAloneComma(char* line);
void cleanLists(struct wordsList** wordList,struct symbolNode** symbolList);
void updateSymbolAdress(struct symbolNode** symbolList, int IC);
void updateInstructionAdress(struct wordsList** instructionList, int IC);

int main(int argc, char *argv[])
{
	/*variable decelerations*/
  	FILE *file;
	int i;
	char* fileName;
	char* fileNameAll;
	struct wordsList* wordList;
	struct wordsList* instructionList;
	struct symbolNode* symbolList;
	int IC;
	int DC;
  struct wordsList** wordListTail;
	for(i = 1;i<argc;i++)/*go through all of the files we get*/
	{
		/*variable decelerations*/
		fileName = malloc(MAX_FILE_NAME);
		fileNameAll = malloc(MAX_FILE_NAME);
		wordList=NULL;
		instructionList=NULL;
		symbolList = NULL;
		IC = IC_DEFAULT;
		DC = DC_DEFAULT;
		wordListTail=&wordList;

		checkIfAllocated(fileName);/*tested malloc*/
		checkIfAllocated(fileNameAll);/*tested malloc*/

		if(sizeof(argv[i])>MAX_FILE_NAME)/*if the file name is to big*/
		{
			printf("%s: file name over the limit\n",argv[i]);
		}

		/*getting the names of the files*/
		strcpy(fileName,argv[i]);
		strcpy(fileNameAll,argv[i]);
		strcat(fileName,".as");/*adding the ".as" to the end of the name*/

		/*opening the file*/
		file = fopen(fileName,"r");

		if(file!=NULL)/*if we have a .as file*/
		{
			/*FIRST PASS*/
			strcpy(fileName,argv[i]);
			firstPass(file,fileNameAll);/*dealing with macros here. the first pass over the file*/
			fclose(file);/*closing the original file*/

			/*SECOND PASS*/
			/*prepare the macro-free file*/
			strcpy(fileName,fileNameAll);
			strcat(fileName,".am");
			file = fopen(fileName,"r");

			if(secondPass(file,&wordList,&instructionList,&symbolList,&IC,&DC) == 1)/**/
			{
				/*freeing memory and closing files.*/
				fclose(file);
				free(fileName);
				free(fileNameAll);
        cleanLists(&wordList,&symbolList);
				continue;
			}
      /*now we connect the instruction list to the word list*/
			if(wordList==NULL){
				wordList = instructionList;
			}
			else{
				while((*wordListTail)->next!=NULL){
					wordListTail = &(*wordListTail)->next;
				}
				(*wordListTail)->next = instructionList;
			}

			updateInstructionAdress(&instructionList,IC);/*here we first update every instruction counter by adding it the value of IC*/
			updateSymbolAdress(&symbolList,IC);/*every symbol that is data we need to increase its counter and also update everyones base and offset*/
			fseek(file, 0, SEEK_SET);
			if(thirdPass(file,&wordList,&symbolList,fileNameAll)){
				fclose(file);
				free(fileName);
				free(fileNameAll);
        cleanLists(&wordList,&symbolList);
				continue;
			}
			/*closing files,freeing memory*/
			fclose(file);
			createObEntFiles(&wordList,&symbolList,fileNameAll,IC,DC);
			free(fileName);
			free(fileNameAll);
			cleanLists(&wordList,&symbolList);
		}
		else/*if we could'nt finf the file givin to us.*/
			printf("%s: file doesnt exist\n",fileName);
	}
	return 0;
}


/*--------------------------------------------------------------------------------
this function frees all of the memory associated with the wordList and symbolList.
	input: struct wordsList** wordList- the wordList we clean.struct
	symbolNode** symbolList- the symbolList we clean
	return: void.
----------------------------------------------------------------------------------*/
void cleanLists(struct wordsList** wordList,struct symbolNode** symbolList)
{
	/*variable decelerations*/
	wordsList* wordTemp;

	while((*wordList)!=NULL)/*while we did not reach the end of wordlist*/
	{
		if((*wordList)->wordCommand!=NULL)/*we are dealing with a command word*/
		{
			free((*wordList)->wordCommand);
		}
		else if((*wordList)->wordInformation!=NULL)/*we are dealing with a information word*/
		{
			free((*wordList)->wordInformation);
		}
		else
		{
			free((*wordList)->wordExtra);
		}

		wordTemp=*wordList;
		wordList = &(*wordList)->next;/*move forwad in the list*/
		free(wordTemp);
	}
}

/*--------------------------------------------------------------------------------
  this function updates all the instructionList items counter
	input: struct wordsList** instructionList- the list we update,int IC- the number we add with
	return: void.
----------------------------------------------------------------------------------*/
void updateInstructionAdress(struct wordsList** instructionList,int IC){
  /*while there are items update it's counter by adding it IC*/
	while((*instructionList)!=NULL){
		(*instructionList)->counter = (*instructionList)->counter+IC;

		instructionList = &(*instructionList)->next;
	}
}
/*--------------------------------------------------------------------------------
  this function updates all of the counters,base and offset of the symbol list
	input: struct symbolNode** symbolList- the list we update,int IC-the numebr we add with
	return: void.
----------------------------------------------------------------------------------*/
void updateSymbolAdress(struct symbolNode** symbolList,int IC){
  /*while there are items, check if it is a data, if yes update counter.
  in any other way update the base and offset*/
	while((*symbolList)!=NULL){
		if((*symbolList)->attributes[DATA]==TR){
			(*symbolList)->base = (*symbolList)->base+IC;
		}
		(*symbolList)->offset = (*symbolList)->base%16;
		(*symbolList)->base = (*symbolList)->base-(*symbolList)->offset;
		symbolList = &(*symbolList)->next;
	}
}
/*--------------------------------------------------------------------------------
This function creates the .ob and the .ent files. it places the proper output in
the .ob file using the "printWords" function (one below), and itself places the .ent
output in the file. Notice that all of this is of course after the third pass.
	input: struct wordsList** wordList- the wordlist we got from the second and
	third pass. struct symbolNode** symbolList- the symble list we made after
	we passed over the file. char* name- the name of the original ".as" file
	return: void.
----------------------------------------------------------------------------------*/
void createObEntFiles(struct wordsList** wordList,struct symbolNode** symbolList,char* name,int IC,int DC)
{
	/*variable decelerations*/
	FILE* entryFile=NULL;
	char* entryName = malloc(MAX_FILE_NAME);
	checkIfAllocated(entryName);/*tested malloc*/

	printWords(wordList,name,IC,DC);/*creates the object file*/

	/*creating the .ent file*/

	strcpy(entryName,name);
	strcat(entryName,".ent");/*adding the ".ent" to the end of the file name.*/

	while((*symbolList)!=NULL)/*while we did not get to the end of symbolList*/
	{
		if((*symbolList)->attributes[ENTRY]==TR)/*if we find an entry*/
		{
			if(entryFile==NULL)/*not open*/
			{
				entryFile = fopen(entryName,"w");
			}
			/*puts the entry we found into the file*/
			fprintf(entryFile, "%s, %d, %d\n",(*symbolList)->name,(*symbolList)->base,(*symbolList)->offset);
		}
		symbolList = &(*symbolList)->next;/*move forward in the symbolList*/
	}
	if(entryFile!=NULL)/*file is open*/
	{
		fclose(entryFile);/*close the file*/
	}
	free(entryName);/*free alocated memory*/
}
/*--------------------------------------------------------------------------------
As we have written in the "readME" file, the words in this project come in 3 forms,
based on the adrressing method and operand. here we make the ".ob" file, and print
into it the words we have after the second and third pass. Printing each word
differs depending on it's type, and each word has a printing function of its own.
notice that all the "printword" functions are in bitfield.c.
	input: struct wordsList** wordList- all the words we will put into the ".ob"
	file. char* name- the file name. int IC- the IC. int DC- the DC.
	return: void.
----------------------------------------------------------------------------------*/
void printWords(struct wordsList** wordList,char* name,int IC,int DC)
{
	/*variable decelerations*/
	FILE* file;
	char* saveForFile = malloc(MAX_FILE_NAME);
	checkIfAllocated(saveForFile);/*tested malloc*/

	/*getting the name for the .ob file*/
	strcpy(saveForFile,name);
	strcat(saveForFile,".ob");

	file = fopen(saveForFile,"w");/*we prepare to write into the file*/


	/*putting all of the words into the object file*/
	fprintf(file,"\t%d %d\n",IC-100,DC );/*we print the title at the start of the .ob file*/

	while((*wordList)!=NULL)/*while we still have words to print*/
	{
		fprintf(file,"%04d ",(*wordList)->counter);/*printing the place of this word*/

		/*the printWordX functions are in "bitField.c"*/
		if((*wordList)->wordCommand!=NULL)
		{
			printWordCommand((*wordList)->wordCommand,file);
		}
		if((*wordList)->wordInformation!=NULL)
		{
			printWordInformation((*wordList)->wordInformation,file);
		}
		if((*wordList)->wordExtra!=NULL)
		{
			printWordExtra((*wordList)->wordExtra,file);
		}
		wordList = &(*wordList)->next;
	}
	/*freeing memory and closing files.*/
	free(saveForFile);
	fclose(file);
}
