#include "Defenitions.h"


char* CheckIfMacro(char*);
void OpenTempFILE(char * tempFileName);
struct info* GetMacroEnd(FILE*);
int PlaceIfMacro(char*, struct macro*, FILE*);
void clearInfo(struct info *inf);/*we never called the functions*/
void clearMacros(struct macro *mac);


/*--------------------------------------------------------------------------------
This function does the first iteration on the file and creates a file without macros.
	input: FILE *file- Pointer to file. char* fileName- A string of the file
	name.
	return: int (none relevant).
----------------------------------------------------------------------------------*/
int firstPass(FILE *file,char* fileName)
{

	/*variable decelerations*/
	FILE* tempFile;
	char* tempFileName = malloc(MAX_FILE_NAME);
	char* line;
	char* name;
	/*the linked lists*/
	struct macro *macroList = NULL;
	struct macro *macroTail = NULL;
	struct info *infoMacro;

	checkIfAllocated(tempFileName); /*tested malloc, notice that function terminates if unsuccessful.*/

	OpenTempFILE(fileName); /*gets the file temp name and opens it*/
	strcpy(tempFileName,fileName);/*getting the right name for the .am file*/
	strcat(tempFileName,".am");/*adding the ".am ending"*/

	/*prepare to write into tempFile*/
	tempFile = fopen(tempFileName,"w");

	while((line = getLine(file)))
	{
		name = CheckIfMacro(line);

    		/*if name isn't NULL it means that a macro has just been read*/
    		if(name != NULL)
		{
        		/*here we create the macros list*/
        		infoMacro = GetMacroEnd(file);
        		if(macroTail == NULL)
			{
        			addNode(name,infoMacro,&macroList);
        			macroTail=macroList;
        		}
        		else
			{
				/*add the macro and it's body to the macro list*/
          			addNode(name,infoMacro,&macroTail);
          			macroTail = macroTail->next;
        		}
        		free(name);/*free allocated memory*/
						clearInfo(infoMacro);
    		}

    		/*If name is null, there are 2 possibilities. The first is that the name is a macro, and second is
    		that it isn't a declaration of a macro nor is the name of an existing macro. If it is a macro name
   	 	then we copy to the file the macaro's info. For the second option we simply copy the line to the file*/
    		else if((macroList != NULL))
		{
			/*here we check if a given line is a known macro, if so we place the "macro info" (the body of
			the macro) into the file*/
        		if(PlaceIfMacro(line,macroList,tempFile) == 0)
			{	/*if PlaceIfMacro is 0 , it means it wasn't macro, so we just put to file*/
          			fputs(line,tempFile);
       			}
    		}
    		else
		{
      			fputs(line,tempFile);
    		}
    		free(line);/*free allocated memory*/
	}

		fclose(tempFile);
		clearMacros(macroList);
  	return 0;
}
/*-------------------------------------------------------------------------------------
This function gets a line checks if it's a macro. if it is we place in the file we
receive and return 1. otherwize, return 0.
	input: char* line- the line we check. struct macro* head- the struct macro.
	       FILE* file- and the file we write into
	return: int. 1 if it is macro, 0 otherwise.
---------------------------------------------------------------------------------------*/
int PlaceIfMacro(char* line,struct macro* head,FILE* file)
{

	/*variable decelerations*/
	char *macroName;
	struct macro *tmp = head;
	struct info *infoTmp;

	line = getCharAfterWhiteSpace(line);/*skips white spaces at start*/
	macroName = GetStringTilWhite(line);/*gets the first word from the line*/

 	while(tmp != NULL)/*while not at the end of the linked list*/
	{
 		if(strcmp(tmp->name,macroName) == 0)/*if we found a match*/
		{
   	   		infoTmp = tmp->info;
 	     		while(infoTmp->info != NULL)/*we are not at the end if it is not NULL*/
			{
        			fputs(infoTmp->info,file);/*place the macro "info" into the file*/
        			infoTmp = infoTmp->next;/*move forward in linked list*/
      			}
      			return 1;
    		}
    		tmp = tmp->next;/*move forward in linked list*/
  	 }
  	 return 0;
}
/*--------------------------------------------------------------------------------
This function gives us a possible "temp file" that we could copy to the file info
into.
	input: char * tempFileName- Name of the file we want to make useable.
	return: void.
----------------------------------------------------------------------------------*/
void OpenTempFILE(char * tempFileName)
{
	/*variable decelerations*/
	char* tempName = malloc(MAX_FILE_NAME);/*#1*/
	char* saveName = malloc(MAX_FILE_NAME);/*#2*/
	char* ending = ".am";
	char* repeat = tempFileName;
	FILE *file;
	checkIfAllocated(tempName);/*tested malloc #1*/
	checkIfAllocated(saveName);/*tested malloc #2*/

	strcpy(tempName,repeat);/*both lines save the first possible name*/
	strcpy(saveName,repeat);
	strcat(tempName,ending);/* add the .am*/
	tempName[MAX_FILE_NAME-1] = '\0';/*adding the null char to the end*/
	saveName[MAX_FILE_NAME-1] = '\0';/*adding the null char to the end*/

 	/*getting ready to read the file*/
	file = fopen(tempName, "r");

	/*If file can be opened it means it exist, therefore if that is the case
	then we put the name the repaet string and save the new string each time.
	we also put at the end the ".txt" so we know it is a txt file*/

	while(file){
		fclose(file);
		strcat(saveName,repeat);
		strcpy(tempName,saveName);
 		strcat(tempName,ending);
		if(strlen(tempName) > MAX_FILE_NAME -1)
		{
			printf("ERROR! file neme is to long, to many duplicates!!! please delete some of them!\n");
			exit(1);/*terminate*/

		}
		tempName[MAX_FILE_NAME-1] = '\0';/*adding the null char to the end*/
		saveName[MAX_FILE_NAME-1] = '\0';/*adding the null char to the end*/

		/*getting ready to read the file*/
		file = fopen(tempName, "r");
  	}
  	strcpy(tempFileName,saveName);

	/*free the memory alocated*/
	free(tempName);
	free(saveName);
}

/*--------------------------------------------------------------------------------
The fucntion saves in a list all of the lines in the macro.
	input: FILE* file-a FILE pointer
	return: struct info*- a linked list with all of the macro info (the body
	of the macro).
----------------------------------------------------------------------------------*/
struct info* GetMacroEnd(FILE* file)
{
	/*variable decelerations*/
	char* line;
	char* fullLine=malloc(MAX_LINE_LEN);
	int i,notEndm  = 0;
	char endm[] = "endm";
	struct info *info = NULL;
	struct info *infoTail = NULL;
	checkIfAllocated(fullLine);/*tested malloc*/

	while((line = getLine(file)))/*while the file hasn't ended*/
	{
		notEndm = 0;
 		strcpy(fullLine,line);
		line = getCharAfterWhiteSpace(line);
		if(sizeOfLine(line) >= 4)/*the length of "endm" is 4. */
		{
			for (i = 0; i < 4; i++)
			{
 				if(line[i] != endm[i])
				{
					notEndm= 1;
				}
      			}
    		}
		else
		{
      			notEndm = 1;
    		}

    		/*The notEndm flag means the we didn't get endm in the line
    		each time we iterate we save the line to a linked list. flag 1 means
    		length is less then endm or the word itself isn't endm*/

    		if(notEndm== 1)
		{
      			if((info == NULL))
			{
        			addinfo(fullLine,&info);
        			infoTail = info;
      			}
      			else
			{
        			addinfo(fullLine,&infoTail);
       		 		infoTail = infoTail->next;/*info tail always points to the last node*/
      			}
    		}
    		else
		{
      			break;
    		}
  	}
 	return info;
}


/*--------------------------------------------------------------------------------
the functions checks if the line includes a macro, if does it returns it's name.
per-instruction, we asume propriety in this part of the code.
	input: char* line- The string we are checking.
	return: string- the name of the macro,if it is a macro. If it's not
	a macro return NULL.
----------------------------------------------------------------------------------*/
char* CheckIfMacro(char* line)
{
	/*variable decelerations*/
 	char macro[] = "macro";
	int i;
  	char* macroName;
  	line = getCharAfterWhiteSpace(line);/*Point to the begining of the first letter.*/

 	/*Here we checked if the first word is macro, if so, we begin to copy the name*/

  	if(sizeOfLine(line)>=MIN_MACRO_SIZE)/*check if the line is at least the min size of a macro*/
	{
    		for (i = 0; i < MIN_MACRO_SIZE-2; i++)
		{
      			if(line[i]!=macro[i])
			{
       				 return NULL;
      			}
    		}
    		line += MIN_MACRO_SIZE-2;
    		line = getCharAfterWhiteSpace(line);/*point to the first letter of the name*/
   		macroName = GetStringTilWhite(line);/*gets the name*/
  	}
  	else
	{
    		return NULL;/* if the length is smaller than the min it can't have a macro*/
 	}
  	return macroName;
}
/*--------------------------------------------------------------------------------
This function gets a pointer to a macro and clears all of the memory associated
with it.
	input: struct macro *mac, the macro we clear.
	return: void
----------------------------------------------------------------------------------*/
void clearMacros(struct macro *mac)
{
	/*variable decelerations*/
	struct macro *tempMac;
	while(mac != NULL)
	{
		/*clear the info of the mac. */
		clearInfo(mac->info);
		tempMac = (*mac).next;
		free(mac);/*free the memory allocated*/
		mac = tempMac;

	}

}
/*--------------------------------------------------------------------------------
This function gets a pointer to a info and clears all of the memory associated
with it.
	input: struct macro *mac, the macro we clear.
	return: void
----------------------------------------------------------------------------------*/
void clearInfo(struct info *inf)
{
	/*variable decelerations*/
	struct info *tempInfo;

		/*clear the info of a mac. */
		while(inf != NULL)
		{
			tempInfo = (*inf).next;/* saving the next "info" struct */
			free(inf); /*mac.info has been initialized by malloc earlier so we free it*/
			inf = tempInfo;
		}
}

/*--------------------------------------------------------------------------------
This function checks if an alocation of memory was succesful. If not it terminates
the program.
	input: void *ptr- the pointer we allocated memory for.
	return: void
----------------------------------------------------------------------------------*/
void checkIfAllocated(void *ptr)
{
    if (!ptr)/*if NULL then ptr = 0*/
    {
        printf("was NOT able to allocate memory!!!\n");
        exit(1);/*terminate*/
    }
}
