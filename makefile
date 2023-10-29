default: hashtable.o utilities.o list.o main.c
	gcc main.c hashtable.o utilities.o list.o -o mvote -lm

list.o:list.h list.c
	gcc -c list.c 

utilities.o:utilities.h utilities.c
	gcc -c utilities.c -lm

hashtable.o:hashtable.h hashtable.c list.h utilities.h 
	gcc -c hashtable.c

