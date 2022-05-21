assembler:input.o main.o fileProces.o linkedList.o secondPass.o bitField.o thirdPass.o
	gcc -g -ansi -Wall -pedantic input.o fileProces.o linkedList.o main.o secondPass.o bitField.o thirdPass.o -o assembler
main.o:main.c fileProces.o secondPass.o thirdPass.o input.o Defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
thirdPass.o:thirdPass.c fileProces.o input.o Defenitions.h structs.h  linkedList.o
	gcc -c -ansi -Wall -pedantic thirdPass.c -o thirdPass.o
secondPass.o:secondPass.c fileProces.o input.o Defenitions.h structs.h  linkedList.o
	gcc -c -ansi -Wall -pedantic secondPass.c -o secondPass.o
fileProces.o:fileProces.c input.o Defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic fileProces.c -o fileProces.o
linkedList.o: linkedList.c Defenitions.h structs.h 
	gcc -c -ansi -Wall -pedantic linkedList.c -o linkedList.o
bitField.o: bitField.c Defenitions.h structs.h 
	gcc -c -ansi -Wall -pedantic bitField.c -o bitField.o
input.o:input.c Defenitions.h structs.h
	gcc -c -ansi -Wall -pedantic input.c -o input.o

