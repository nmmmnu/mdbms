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

#ifndef dbfilehh
#define dbfilehh

#define STACKSIZE 10000

class dbfile;
class page
{
   dbfile *parent;  
   public:
   page(dbfile *);
   ~page();
   char *data;
   int pagenum;
   int dirty;
   int accesses;
   unsigned load;
   inline void setload();
};
class dbfile
{
   int fd, mapfd, pagesize;
   int cachehitvar, cachefailvar;
   int *watermark;
   char *freemap;
   page **pages;
   int npages, mapdirty, mapbytes;
   char *bufstack[STACKSIZE];
   int freestack[STACKSIZE], bufstackp, stackp;
   int readdata(char *, unsigned long, unsigned long);
   int writedata(char *, unsigned long, unsigned long);
   void initfreestack();
   void readmap(char *);
   inline int isfree(int);
   void setfree(int);
   void setbusy(int);
   void dropfromcache(int);
   page *addpage();
   char *name;
   void push(int);
   int pop();

   public:
   dbfile(char *, int, int *);
   ~dbfile();
   int getpagesize();
   page *getpage(int);
   page *newpage();
   void droppage(page *);
   int writepage(page *);
   void freepage(page *);
   void freepage(int);
   void sync();
   void checkcache();
   char *getname();
   char *getbuffer();
   void pushbuffer(char *);
};

#endif
