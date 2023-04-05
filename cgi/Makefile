CFLAGS=-O2
OBJECTS=lex.yy.o y.tab.o main.o ../lib/libdbms.a ../crypt/crypt.a

all : mgen.cgi checktype

checktype : checktype.c
	gcc $(CFLAGS) checktype.c -o checktype
mgen.cgi : $(OBJECTS)
	gcc $(CFLAGS) $(OBJECTS) -o mgen.cgi -lm

main.o  : main.c
	gcc $(CFLAGS) -c main.c
lex.yy.o : lex.yy.c
	gcc $(CFLAGS) -c lex.yy.c
y.tab.o : y.tab.c
	gcc $(CFLAGS) -c y.tab.c
lex.yy.c : lex.l y.tab.c
	flex -f -i lex.l
y.tab.c : parser.y
	yacc -d -v parser.y

strip : all
	strip mgen.cgi
	strip checktype
clean :
	rm -f *.[ao] checktype mgen.cgi

