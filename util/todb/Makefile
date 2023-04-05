all : todb
todb : todb.c
	gcc -O2 todb.c ../../lib/libdbms.a -o todb
strip : all
	strip todb
clean :
	rm -f todb
