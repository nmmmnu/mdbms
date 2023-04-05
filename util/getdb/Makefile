all : getdb
getdb : getdb.c
	gcc -O2 getdb.c ../../lib/libdbms.a -o getdb
strip : all
	strip getdb
clean :
	rm -f getdb
