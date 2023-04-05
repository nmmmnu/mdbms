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

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "global.hh"
#include "dbunits.hh"
#include "support.h"

dbunit::dbunit(int cachemem)
{
   dirty=0, watermark=cachemem*1024;
   streamfile=new dbfile("units",sizeof(streamentry),&watermark);
   readfile();
}
dbunit::~dbunit()
{
   int x;
   syncfile();
   for (x=0;x<afiles;x++) delete files[x];
   delete streamfile;
}

void dbunit::syncfile()
{
   char version[50]=PHYSVERSION;
   if (!dirty) return;
   int fd=open(METAFILE,O_WRONLY|O_CREAT|O_TRUNC,7*8*8+5*8+8);
   if (fd==-1) return;
   write(fd,version,50);
   for (int x=0;x<afiles;x++)
   {
      char name[50];
      int pagesize=files[x]->getpagesize();
      strcpy(name,files[x]->getname());
      write(fd,name,50);
      write(fd,&pagesize,sizeof(int));
   }
   close(fd);
   dirty=0;
}
void dbunit::sync()
{
   for (int x=0;x<afiles;x++) files[x]->sync();
   streamfile->sync();
   syncfile();
}
void dbunit::readfile()
{
   int fd=open(METAFILE,O_RDONLY), pagesize;
   char name[50];
   afiles=0;
   files=NULL;
   if (fd==-1) return;
   while (read(fd,name,50)==50)
   {
      if (name[0]=='V') continue;
      read(fd,&pagesize,sizeof(int));
      createfile(name,pagesize);
   }
}
stream *dbunit::openstream(int streamnum)
{
   stream *returnstream;
   page *meta=streamfile->getpage(streamnum);
   if (!meta) return NULL;
   streamentry *sentry=(streamentry*)meta->data;
   returnstream=new stream(files[sentry->filenum],sentry->filenum,meta,0);
   return returnstream;
}
/* Size: Indicated size, stream is not sized */
stream *dbunit::createstream(int size)
{
   stream *newstream;
   page *metapage;
   int bestdif=-1, x, dbnum;
   dbfile *db=NULL;
   for (x=0;x<afiles;x++)
   {
      int dif=abs(files[x]->getpagesize()-size);
      if (!db||dif<bestdif) db=files[x], bestdif=dif, dbnum=x;
   }
   if (!db)
   {
      seterror(ERR_NODBFILES,NULL);
      return NULL;
   }
   metapage=streamfile->newpage();
   newstream=new stream(db,dbnum,metapage,1);
   return newstream;
}
int dbunit::destroystream(stream *s)
{
   s->dead=1;
   s->freestream();
   streamfile->droppage(s->metapage);
   streamfile->freepage(s->metapage);
   return 1;
}
int dbunit::createfile(char *filename, int pagesize)
{
   dbfile *newfile=new dbfile(filename,pagesize,&watermark);
   if (dberrno!=ERR_OK) return -1;
   dirty=1;
   afiles++;
   files=(dbfile**)realloc(files,sizeof(dbfile*)*afiles);
   files[afiles-1]=newfile;
   return 1;
}
void dbunit::checkcache()
{
   int x;
   for (x=0;x<afiles;x++) files[x]->checkcache();
   streamfile->checkcache();
}
int stream::linkto(page *source, page *dest)
{
   int num=dest?dest->pagenum:-1;
   *(int*)source->data=num;
   source->dirty=1;
   return 1;
}
int stream::linkedto(page *source)
{
   return *(int*)source->data;
}
stream::stream(dbfile *dbp, int filenum, page *mp, int init)
{
   metapage=mp, dead=0;
   metaentry=(streamentry*)mp->data;
   if (init)
   {
      page *firstpage;
      firstpage=dbp->newpage();
      linkto(firstpage,NULL);
      metaentry->size=0, metaentry->page=firstpage->pagenum;
      metaentry->filenum=filenum;
      metaentry->type=TYPE_STREAM;
      metapage->dirty=firstpage->dirty=1;
      dbp->droppage(firstpage);
   }
   streamnum=mp->pagenum;
   db=dbp;
   currentpage=metaentry->page,position=currentoffset=0;
   blocksize=db->getpagesize()-sizeof(int);
}
stream::~stream()
{
   if (!dead) db->droppage(metapage);
}
int stream::getfirstpage()
{
   return metaentry->page;
}
dbfile *stream::getdb()
{
   return db;
}
int stream::reallocate(int size, int seekonly)
{
   int nextpage, togo=size, lastpagenum;
   page *first=db->getpage(metaentry->page), *previous=first;
   if (!first) return -1;
   togo-=blocksize;
   while (togo>0)
   {
      nextpage=linkedto(previous);
      page *next;
      if (nextpage!=-1) next=db->getpage(nextpage); else
      {
         next=db->newpage();
         linkto(previous,next);
         linkto(next,NULL);
         metapage->dirty=1;
      }
      db->droppage(previous);
      previous=next;
      togo-=blocksize;
   }
   nextpage=linkedto(previous);
   lastpagenum=previous->pagenum;
   db->droppage(previous);
   if (!seekonly)
   {
      linkto(previous,NULL);
      while (nextpage!=-1)
      { 
         page *next=db->getpage(nextpage);
         nextpage=linkedto(next);
         db->droppage(next);
         db->freepage(next);
      }
      metapage->dirty=1;
      metaentry->size=size;
   }
   if (seekonly||position>size)
   { 
      position=size;
      currentpage=lastpagenum; 
      currentoffset=togo+blocksize;
   }
   return size;
}
int stream::seek(int pos)
{
   return reallocate(pos, 1);
}
int stream::trunc(int pos)
{
   if (pos>metaentry->size)
   {
      seterror(ERR_BADVALUE,NULL);
      return -ERR_BADVALUE;
   }
   reallocate(pos, 0);
   return pos;
}
int stream::freestream()
{
   int next;
   page *p=db->getpage(getfirstpage());
   next=*(int*)p->data;
   db->droppage(p);
   db->freepage(p);
   while(next!=-1)
   {
      p=db->getpage(next);
      if (!p) break;
      next=*(int*)p->data;
      db->droppage(p);
      db->freepage(p);
   }
   return 1;
}
int stream::write(char *buf, int bytes)
{
   int bufp=0, togo=bytes;
   page *p;
   if (position+bytes>metaentry->size) reallocate(position+bytes, 0);
   while (togo)
   {
      int next, writeable=blocksize-currentoffset;
      if (writeable>togo) writeable=togo;
      p=db->getpage(currentpage);
      if (!p) break;
      next=*(int*)p->data;
      memcpy(p->data+currentoffset+sizeof(int),buf+bufp,writeable);
      togo-=writeable, bufp+=writeable, position+=writeable;
      p->dirty=1;
      db->droppage(p);
      currentoffset+=writeable;
      if (currentoffset==blocksize&&next!=-1)
         currentpage=next, currentoffset=0;
   } 
   return bytes;
}
int stream::read(char *buf, int bytes)
{
   int toread, bufp=0, bytesread=0, next, end=0; 
   do
   {
      page *thispage;
      toread=blocksize-currentoffset;
      if (toread>bytes) toread=bytes;
      if ((position+toread)>=metaentry->size)
         toread=metaentry->size-position, end=1;
      thispage=db->getpage(currentpage);
      if (!thispage) break;
      next=*(int*)thispage->data;
      if (buf) memcpy(buf+bufp,thispage->data+sizeof(int)+currentoffset,toread);
      bytesread+=toread, currentoffset+=toread, bytes-=toread, position+=toread;
      bufp+=toread;
      db->droppage(thispage);
      if (currentoffset==blocksize&&next!=-1)
         currentpage=next, currentoffset=0;
   } while (bytes&&currentpage!=-1&&!end);
   return bytesread;
}
int stream::getstreamnum()
{
   return streamnum;
}
int stream::getsize()
{
   return metaentry->size;
}
int stream::toend()
{
   return reallocate(metaentry->size,1);
}
