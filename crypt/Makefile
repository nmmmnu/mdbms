crypt.a : crypt.o crypt_util.o
	ar rc crypt.a crypt.o crypt_util.o
crypt.o	: crypt.c
	gcc -c crypt.c
crypt_util.o	: crypt_util.c
	gcc -c crypt_util.c
clean :
	rm -f *.[oa]
