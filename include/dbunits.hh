/*
 *  MDBMS source code
 *
 *  (c) 1997 HINT InterNetworking Technologies
 *
 * This is beta software. This version of mdbms can be used without
 * charge. Redistribution in source and binary forms, without any
 * modification, is permitted.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED 'AS IS'. NO RESPONSIBILITY FOR
 * ANY DAMAGE CAUSED BY THE USE OF THIS SOFTWARE, CAN BE ACCEPTED BY
 * THE AUTHOR AND/OR PUBLISHER.
 *
 * See the README.source file for information.
 *
 *  Please report bugs/fixes to marty@hinttech.com
 */

#ifndef dbunitshh
#define dbunitshh
#define TYPE_STREAM 0
#include "dbfile.hh"
struct streamentry
{
   short int filenum;
   int page;
   int size;
   unsigned char type;
};
class dbunit;
class stream
{
   dbfile *db;
   page *metapage;
   streamentry *metaentry;
   int streamnum, dead;
   int position, currentpage, currentoffset;
   int blocksize;
   int linkto(page *, page *);
   int linkedto(page *);
   dbfile *getdb();
   stream(dbfile *, int, page *, int);
   int reallocate(int, int);
   int getfirstpage();
   int freestream();
   public:

   ~stream();
   int toend();
   int seek(int);
   int trunc(int);
   int write(char *, int);
   int read(char *, int);
   int getstreamnum();
   int getsize();
   friend class dbunit;
};
class dbunit
{
   dbfile **files;
   int afiles, dirty;
   int watermark;
   dbfile *streamfile;
   void syncfile();
   void readfile();
   
   public:
   dbunit(int);
   ~dbunit();

   void sync();
   int createfile(char *, int);
   stream *createstream(int);
   stream *openstream(int);
   int destroystream(stream *s);
   void checkcache();
   friend class stream;
};
#endif
