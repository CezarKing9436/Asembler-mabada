Maman 14- Lavi Gimpel, Shai Vilkin.


The purpose of this project is to build an assembler for 16-instruction asssembly language.
We run on a 20 bit, 16 register CPU. We use a 2 Transitions type of assembller. ********

--------------------------------------------------------------------------------------------------
the ".c" files:

main.c---
main function definition, argv & argc processing, single file processing each time.

fileProces.c---
here we "preprocess" every .as file. we deal with the macros and preduce a .am file free of
macros. this is done through the "firstPass" function. In short, we use a linked list of all the
macros we find, in each "macro link" we have a second linked list that holds all of the lines in
the macro until the "endm".

secondPass.c---
In this function we go through the macro-free file, we start encoding the words, and save the
symbols we run into. Here we do the main part of the error detection. if we find a error in a
"command line" or "instruction line" we do not move to the third pass and we stop the encoding.
The encoding goes into a linked list to be put in the object file after we go through thirdPass
NOTE: We have relized the words we encode come in 3 shapes. the first (will be known as
"word1") has just the ARE and upcode, the second (will be known as "word2") has the funct and
all of the bits associated with the operands. the third (will be known as "word3") is built
based on the adressing method (optional of course).

thirdPass.c---
Here we assume that a line's syntax is legal. we go through the symbol chart, and encode the
file in accordance. if we dont find a laybol (=symbol), we declare an error.

input.c---
This file contains the functions to get a line from the file. additionly, we placed most of
the functions that analyze a single line, such as the number of commas in a givin line.

bitField.c---
Here is whre the encoding for the .obj is done. It contains the assisting functions for this
task as well.

linkedList.c---
This file is for all of the linked lists in this project. Also contains the assisting functions.

--------------------------------------------------------------------------------------------------
the ".h" files

Defenitions.h---
Here we declare all the functions that are used and declared in more then one single file.

structs.h---
Here we define and declare all of the structs used in the project.

--------------------------------------------------------------------------------------------------

GENERAL NOTES:
1)if we see that we already have a file with our name thats ends with .am , we try to create another file with
the same name reapeated. if the name becomes to long we print an error and exit the code.
2)if a malloc has failed we exit the whole code.
3)every error that is coonected to the symbol list is handeled at the thirdPass.c , this is becuase
during the first pass over the file we couldn't procude all of the symbol list.
4) we always keep the .am file becuase the errors that are shown are realted to the lines at that file
