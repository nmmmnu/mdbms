all : db utils libs mgen
db : crypt/crypt.a
	cd mdbms ; make
utils : libs
	cd util; make
crypt/crypt.a:
	cd crypt ; make
libs :
	cd lib ; make
mgen : crypt/crypt.a libs
	cd cgi ; make

backup : clean
	tar cvfz dbms.tgz *

install : all
	install -s -m 755 cgi/mgen.cgi cgi/checktype mdbms/db util/todb/todb util/getdb/getdb util/dumpdb/dumpdb /usr/local/bin
	install -m 755 man/db.8 man/dumpdb.8 man/getdb.8 man/todb.8 /usr/man/man8
	install -m 755 man/fop.5 /usr/man/man5
	install -m 755 -d /usr/local/lib/mdbms
	install -m 755 -d /usr/local/include/mdbms
	install -m 755 lib/errors.o lib/libdbms.o /usr/local/lib/mdbms
	install -m 755 lib/errors.h lib/libdbms.h lib/protocol.h /usr/local/include/mdbms
	install -d -m 755 /usr/local/etc/mdbms
	install -m 755 help.txt issue.txt /usr/local/etc/mdbms


clean :
	rm -f db dbms.tgz
	cd mdbms ; make clean
	cd util ; make clean
	cd cgi ; make clean
	cd lib ; make clean
	cd crypt ; make clean

strip : all
	cd mdbms ; make strip
	cd util ; make strip
	cd cgi ; make strip
