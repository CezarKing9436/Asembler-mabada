#include "Defenitions.h"
#define IS_BIT_SET(BF,N) (((BF)>>N)&0x1)

struct wordsList** updateWord3InwordList(struct wordsList** wordList,char* line,struct symbolNode** symbolList,int lineNumber,FILE* file,int* hasError);
int updateEntryInSymbol(struct symbolNode** symbolList,char* line);
struct wordsList** changeWord3(struct wordsList** wordList,symbolNode* entry,FILE* );
int updateAllEntry(FILE* file , struct symbolNode** symbolList);
int thirdPass(FILE* file,struct wordsList** wordList,struct symbolNode** symbolList,char* name){
	char* line;
	char* nameFile = malloc(MAX_FILE_NAME);
	FILE* fileExt;
  int lineCounter=1;
  int hasError=0;
	int size;
	checkIfAllocated(nameFile);
	/*here we create the extrnal file wo we open it here*/
	strcpy(nameFile,name);
	strcat(nameFile,".ext");
	fileExt = fopen(nameFile,"w");
  hasError = updateAllEntry(file,symbolList);/* this function goes thorught the whole line and updates the entry in needed*/

  fseek(file, 0, SEEK_SET);/*we set the file to the start and start reading*/
	while((line = getLine(file))){
		/*if the line is empty or the line delcares an entry it means we dont need to proces them*/
    if(isCommentOrEmpty(line)){

    }
    else if(checkIfEntry(line)){

    }
    else{
      wordList = updateWord3InwordList(wordList,line,symbolList,lineCounter,fileExt,&hasError);
    }

    lineCounter++;
		free(line);
  }
	/*the next part checks if we need to remove the external file*/
	fseek (fileExt, 0, SEEK_END);
  size = ftell(fileExt);
  if (0 == size||hasError) {
		remove(nameFile);
  }
	free(nameFile);
  return hasError;
}
/*--------------------------------------------------------------------------------
	this function goes thrught whe whole file,each time she sees entry she tries to update it
	input: FILE* file-the file we go over , struct symbolNode** symbolList-the list we update
	return: int. 1 if it was successful 0,2,3 else
----------------------------------------------------------------------------------*/
int updateAllEntry(FILE* file , struct symbolNode** symbolList){
  char* line;
  int lineCounter=1;
  int hasError=0;
	int resultOfUpdating;/*we have 4 result of updating and we return them to know what to print*/
	char* saveBeforeSymbol;/*save pointer before symbol, we move with it on the string*/
  char* name;/*name save the start of the line*/
  fseek(file, 0, SEEK_SET);
  while((line = getLine(file))){
		name = line;
		line = getCharAfterWhiteSpace(line);
	  saveBeforeSymbol = moveToWhiteSpace(line);
	  saveBeforeSymbol--;/*LINE IS NOT EMPTY*/
	  if(*(saveBeforeSymbol)==':')
		{
			saveBeforeSymbol++;
			saveBeforeSymbol = getCharAfterWhiteSpace(saveBeforeSymbol);
			line = saveBeforeSymbol;
		}
    if(checkIfEntry(line)){
      if((resultOfUpdating=updateEntryInSymbol(symbolList,line))!=1){
				/*if result diffrent from 1 it means error and we print the correct one*/
        hasError=1;
				if(resultOfUpdating==0){
        	printf("ERROR in line %d an entry was used but never declared \n",lineCounter );/* not very good errror xd*/
				}
				else if (resultOfUpdating==3){
					printf("ERROR in line %d an entry was declared more than once \n",lineCounter );/* not very good errror xd*/
				}
				else{
					printf("ERROR in line %d, an external symbol cannot be a entry as well \n",lineCounter );
				}
      }
    }
    lineCounter++;
		free(name);
  }
  return hasError;
}
/*--------------------------------------------------------------------------------
	this function goes over the file and changes the proper encoded words if needed
	input: struct wordsList** wordList-the encoded list we change,char* line- the line we go over,struct symbolNode** symbolList- the symbol list,int lineNumber- line number,FILE* fileExt-the external file we edit
	return: struct wordsList** the start of the next encoded command
----------------------------------------------------------------------------------*/
struct wordsList** updateWord3InwordList(struct wordsList** wordList,char* line,struct symbolNode** symbolList,int lineNumber,FILE* fileExt,int* hasError){
  char* saveBeforeSymbol;
  char* name;
  char* name2;
  struct symbolNode* entry=NULL;
  struct symbolNode* entry2=NULL;
  int command;
	/* at the start we skip the entry if exits, it means we are going to the command*/
  line = getCharAfterWhiteSpace(line);
  saveBeforeSymbol=moveToWhiteSpace(line);
  if(saveBeforeSymbol-line>=1){
    saveBeforeSymbol--;/*may cuz problem*/
  }
  if(*(saveBeforeSymbol)==':'){
    saveBeforeSymbol++;
    saveBeforeSymbol = getCharAfterWhiteSpace(saveBeforeSymbol);
    line = saveBeforeSymbol;
  }
  command = findCommand(line);
	/*if the line isn't a command we dont proces it */
  if(command==-1){
    return wordList;
  }
  wordList = &(*wordList)->next;/*skips the first word we get from a command*/
  if(command == RTS || command==STOP){
		/*this 2 commands have only one word and because of that nothing is needed to be checked*/
  }
  line = getNextWord(line);
	/*now we are on the first opernad*/
  if(command == CLR || command == NOT || command == INC || command == DEC || command == JMP || command == BNE || command == JSR || command == RED || command == PRN ){
    while((*wordList)!=NULL&&(*wordList)->wordCommand==NULL){
			/*every encoded line start with wordCommand, so by skipping it at the start
			while we didn't get to that word agine we are in the same encoded line*/
      if((*wordList)->wordExtra!=NULL&&(*wordList)->wordExtra->A==0){
				/* if we got to a wordExtra that hasn't been configured yet, we will configure it now*/
				/*first we get the string of the operand, if the string can't be found in the symbolList it means error*/
        name = GetStringTilWhite(line);
        entry = findEntry(symbolList,name);
        if(entry==NULL){
          printf("IN LINE %d an attemt to use a entry thats not valid was attempted\n",lineNumber);
					wordList=&(*wordList)->next->next;
					*hasError=1;
        }
        else{
					/*here we configure the word*/
          wordList = changeWord3(wordList,entry,fileExt);
        }
      }
      else{
        wordList = &(*wordList)->next;
      }
    }
  }
  else{
    while((*wordList)!=NULL&&(*wordList)->wordCommand==NULL){
			/*every encoded line start with wordCommand, so by skipping it at the start
			while we didn't get to that word agine we are in the same encoded line*/
      if((*wordList)->wordExtra!=NULL&&(*wordList)->wordExtra->A==0){
				/* if we got to a wordExtra that hasn't been configured yet, we will configure it now*/
				/*first we get the string of the operand and second, if both strings can't be found in the symbolList it means error*/
        name = GetStringTilWhite(line);
        entry = findEntry(symbolList,name);
				/*skip to the nexst opernad*/
        while(*line!=','){
          line++;
        }
        line++;
        if(*line=='	'||*line==' '){/* one is tab the other is spacebar*/
          line = getCharAfterWhiteSpace(line);
        }
        name2 = GetStringTilWhite(line);
        entry2 = findEntry(symbolList,name2);
				/*if both equal to null it means error, else we start to configure*/
        if(entry==NULL&&entry2==NULL){
          printf("IN LINE %d an attemt to use a entry thats not valid was attempted\n",lineNumber);
					wordList=&(*wordList)->next->next;
					*hasError=1;
        }
        else if (entry!=NULL&&entry2==NULL){
          wordList = changeWord3(wordList,entry,fileExt);
        }
        else if (entry==NULL&&entry2!=NULL){
          wordList = changeWord3(wordList,entry2,fileExt);
        }
        else{
          wordList = changeWord3(wordList,entry,fileExt);
          wordList = changeWord3(wordList,entry2,fileExt);
        }
      }
      else{
        wordList = &(*wordList)->next;
      }
    }
  }
  return wordList;
}
/*--------------------------------------------------------------------------------
	this function updates the wordList depending on the symbol
	input: struct wordsList** wordList-the encoded list we change,symbolNode* entry- the entry values,FILE* fileExt-the external file we edit
	return: struct wordsList**. the encoded word after the pair we updated
----------------------------------------------------------------------------------*/
struct wordsList** changeWord3(struct wordsList** wordList,symbolNode* entry,FILE* fileExt){
  int num,i,j;
  if(entry->attributes[ENTRY]==TR||entry->attributes[DATA]==TR||entry->attributes[CODE]==TR){
		/*if the entry isn't external we loop twice, one for base the other for offset and we configure the encoded word*/
	  for(i=0;i<2;i++){
	    if(i==0){
	      num = entry->base;
	    }
	    else{
	      num = entry->offset;
	    }
	    for (j = 0; j < 16; j++){
	      if(IS_BIT_SET(num,j)){
	        (*wordList)->wordExtra->address[j]=TR;
	      }
	      else
	      {
	        (*wordList) ->wordExtra->address[j]=FL;
	      }
	    }
	    (*wordList)->wordExtra->R=1;
	    wordList = &(*wordList)->next;
	    }
  }
  else if(entry->attributes[EXTERNAL]==TR){
		/*if external we just raise E and base and offset is 0, we also print to extenral file*/
		fprintf(fileExt, "%s BASE %d\n",entry->name,(*wordList)->counter);
    (*wordList)->wordExtra->E=1;
    wordList = &(*wordList)->next;
		fprintf(fileExt, "%s OFFSET %d\n",entry->name,(*wordList)->counter);
    (*wordList)->wordExtra->E=1;
    wordList = &(*wordList)->next;
  }
  return wordList;
}
/*--------------------------------------------------------------------------------
	this function goes over the entry list and return the entry that matches the name if exits
	input:struct symbolNode** symbolList- the symbol list,char* name- the name we look for
	return: struct symbolNode*.an entry that matches the name. if none match returns NULL
----------------------------------------------------------------------------------*/
struct symbolNode* findEntry(struct symbolNode** symbolList,char* name){
  while((*symbolList)!=NULL){
		/*just loop thorught the whole symbolList to find the proper entry*/
    if(strcmp((*symbolList)->name,name)==0){
      return *symbolList;
    }
    symbolList = &(*symbolList)->next;
  }
  return NULL;
}
/*--------------------------------------------------------------------------------
	this function goes over the symbol list and updating the entry that matches the name from the line
	input:struct symbolNode** symbolList- the symbol list,char* line- the line we get the name from
	return: int. 1 if successful. 2 if entry and external were defined tegother. 0 if the entry was never delcared
----------------------------------------------------------------------------------*/
int updateEntryInSymbol(struct symbolNode** symbolList,char* line){
  char* name;
  line = getNextWord(line);
  name = GetStringTilWhite(line);
  while((*symbolList)!=NULL){
		/* after getting the name of the entry we loop thorught all the symbol list to check if
		there is an instance of it overthere*/
    if(strcmp(name,(*symbolList)->name)==0){
			if((*symbolList)->attributes[EXTERNAL]==TR){
				return 2;
			}
			if((*symbolList)->attributes[ENTRY]==TR){
				return 3;
			}
      (*symbolList)->attributes[ENTRY]=TR;
      return 1;
    }
    symbolList = &(*symbolList)->next;
  }
  return 0;
}
