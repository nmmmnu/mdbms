CC=gcc
FLAGS=-O2

libdbms.a : errors.o libdbms.o
	ar rcs libdbms.a errors.o libdbms.o
errors.o  : errors.c errors.h
	$(CC) -c $(FLAGS) errors.c
libdbms.o : libdbms.c libdbms.h 
	$(CC) -c $(FLAGS) libdbms.c
clean :
	rm -f *.[oa]
