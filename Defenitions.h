#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

#define MAX_STRUCT_NAME 75
#define MAX_LINE_LEN 80
#define MAX_FILE_NAME 50
#define MIN_MACRO_SIZE 7
#define MAX_FILE_NAME 50
#define IC_DEFAULT 100
#define DC_DEFAULT 0



char * getLine(FILE*);
int firstPass(FILE *,char*);
char* getCharAfterWhiteSpace(char*);
char* GetStringTilWhite(char *);
int sizeOfLine(char*);
void addNode(char*,struct info*,struct macro**);
void addinfo(char*, struct info**);
int isNumUntilWhiteSpace(char* line);
void checkIfAllocated(void *ptr);
int secondPass(FILE* file,struct wordsList** wordList,struct wordsList** instructionList,struct symbolNode** symbolList,int* IC,int* DC);
int numOfWordsInLine(char *line);
char* getNextWord(char *line);
char* moveToComma(char* line);
void checkIfAllocated(void *ptr);
char* moveToWhiteSpace(char* line);
int endsInComma(char* line);
int has2CommasByEachOther(char* line);
int checkIfEntry(char* line);
struct wordsList** addWord3(struct wordsList** wordList,int* DC,int number);
int numFromLine(char** line);
void printWordExtra(wordExtra* word,FILE* file);
void printWordInformation(wordInformation* word,FILE* file);
void printWordCommand(wordCommand* word,FILE* file);
void addSymbol(struct symbolNode** symbolList,char* name, int* DC,int attributes);
int findCommand(char* line);
struct wordsList** addZeroOperand(struct wordsList** wordList,int* IC,int command);
struct wordsList** addOneOperand(struct wordsList** wordList,int* IC,int command,int destanationRegister,int destanationAddressing);
struct wordsList** addEmptyInstruction (struct wordsList** wordList,int* IC);
struct wordsList** addTwoOperand(struct wordsList** wordList,int* IC,int command,int sourceRegister,int sourceAddressing,int destanationRegister,int destanationAddressing);
int isCommentOrEmpty(char* line);
void addExtern(struct symbolNode** symbolList,char* name);
int thirdPass(FILE* file,struct wordsList** wordList,struct symbolNode** symbolList,char*);
int isLegalOperand(char* line);
struct symbolNode* findEntry(struct symbolNode** symbolList,char* name);
int numOfQuotes(char* line);
char* getLastChar(char* line);
int getNumOfCommas(char* line);
int isRegister(char* line);
int hasCommaInMid(char* line);
int hasAloneComma(char* line);
struct wordsList** analyzeTwoOperand(struct wordsList** wordList,int* IC,int command,char* line);
struct wordsList** analyzeOneOperand(struct wordsList** wordList,int* IC,int command,char* line);
void findAdressingMethod(char* line, int* address);
int openSquareBrac(char* line);
int closeSquareBrac(char* line);
