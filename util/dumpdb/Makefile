all : dumpdb
dumpdb : dumpdb.c
	gcc -O2 dumpdb.c ../../lib/libdbms.a -o dumpdb
strip : all
	strip dumpdb
clean :
	rm -f dumpdb
