# Base dir for installation, needed to find the help files
BASE=/usr/local/etc/mdbms

#CFLAGS=-I../include -I../lib -g -D BASE=\"$(BASE)\"
CFLAGS=-I../include -I../lib -O2 -D BASE=\"$(BASE)\"
CC=gcc
CPP=g++
LEX=flex -i -f
YACC=bison -y
#LIBS=-lsocket -lcrypt

OBJECTS=dbfile.o dbunits.o relation.o main.o support.o interface.o dbms.o \
        lex.yy.o y.tab.o userext.o sql.o sqlsearch.o absuserext.o\
        fastuserext.o errors.o manage.o wildmat.o ../crypt/crypt.a

db : $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) $(LIBS) -o db

wildmat.o : wildmat.c
	$(CC) $(CFLAGS) -c wildmat.c
.cc.o :
	$(CPP) $(CFLAGS) -c $<
.c.o :
	$(CPP) $(CFLAGS) -c $<

lex.yy.c : lex.l y.tab.c
	$(LEX) lex.l

y.tab.c : parser.y
	$(YACC) -d -v parser.y

backup : clean
	tar cvfz dbms.tgz *
clean :
	rm -f db core *.o
strip : db
	strip db
