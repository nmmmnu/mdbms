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
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "global.hh"
#include "dbfile.hh"
#include "support.h"

#define BOUND 262144

int dberrno=ERR_OK;
char *dbcontext=NULL;
dbfile::dbfile(char *n, int psize, int *wm)
{
   struct stat statbuf;
   char fname[100];
   name=strdup(n);
   bufstackp=0;
   sprintf(fname,"%s.db",n);
   fd=open(fname,O_CREAT|O_RDWR,7*8*8+5*8+5);
   if (fd==-1)
   {
      seterror(ERR_OPEN,NULL);
      return;
   }
   seterror(ERR_OK,NULL);
   fstat(fd, &statbuf);
   pagesize=psize;
   npages=statbuf.st_size/pagesize;
   readmap(name);
   initfreestack();
   watermark=wm;
   cachehitvar=manager->getvarnum("cache.hits");
   cachefailvar=manager->getvarnum("cache.fails");
}
dbfile::~dbfile()
{
   sync();
   free(name);
   close(fd);
   close(mapfd);
}
void dbfile::readmap(char *name)
{
   char fname[100];
   mapbytes=1+(npages/(sizeof(char)*8));
   int first=npages==0;
   mapdirty=0;
   sprintf(fname,"%s.map",name);
   mapfd=open(fname,O_CREAT|O_RDWR,7*8*8+5*8+5);  
   if (mapfd==-1)
   {
      seterror(ERR_OPEN,NULL);
      return;
   }
   if (!npages) npages=1;
   pages=(page**)calloc(npages,sizeof(page*));
   freemap=(char*)calloc(mapbytes,sizeof(char));
   if (first)
   {
      char dummy=0;
      lseek(fd,pagesize-1,SEEK_SET);
      write(fd,&dummy,1);
      mapdirty=1;
   } else read(mapfd,freemap,mapbytes);
   seterror(ERR_OK,NULL);
}
void dbfile::initfreestack()
{
   stackp=0;
   for (int x=0;(x<npages)&&(stackp<STACKSIZE);x++) if (isfree(x)) 
      push(x);
}
int dbfile::readdata(char *buf, unsigned long amount, unsigned long offset)
{
   lseek(fd, offset, SEEK_SET);
   if (read(fd, buf, amount)!=(int)amount)
   {
      seterror(ERR_READ,NULL);
      return -ERR_READ;
   }
   seterror(ERR_OK,NULL);
   return 1;
}
int dbfile::writedata(char *buf, unsigned long amount, unsigned long offset)
{
   lseek(fd, offset, SEEK_SET);
   if (write(fd, buf, amount)!=(int)amount)
   {
      seterror(ERR_WRITE,NULL);
      return -ERR_WRITE;
   }
   seterror(ERR_OK, NULL);
   return 1;
}
int dbfile::writepage(page *p)
{
   unsigned long offset=pagesize*p->pagenum;
   int result=writedata(p->data,pagesize,offset);
   if (dberrno==ERR_OK) p->dirty=0;
   return result;
}
page *dbfile::addpage()
{
   char dummy=0;
   int newmapbytes=1+(++npages/(sizeof(char)*8));
   if (newmapbytes>mapbytes)
   {
      freemap=(char*)realloc(freemap,sizeof(char*)*newmapbytes);
      freemap[newmapbytes-1]='\0';
      mapbytes=newmapbytes;
   }
   pages=(page**)realloc(pages,sizeof(page*)*npages);
   pages[npages-1]=NULL;
   setbusy(npages-1);
   lseek(fd,npages*pagesize-1,SEEK_SET);
   write(fd,&dummy,1);
   return getpage(npages-1);
}
inline int dbfile::isfree(int page)
{
   return (((unsigned char)freemap[page/8])&(1<<(page%8)))?0:1;
}
void dbfile::setfree(int page)
{
   #ifdef DEBUG
      if (isfree(page)) printf("Freeing free page.\n");
   #endif
   freemap[page/8]&=~(1<<(page%8));
   mapdirty=1;
}
void dbfile::setbusy(int page)
{
   #ifdef DEBUG
      if (!isfree(page))
         printf("Blocking busy page.\n");
   #endif
   freemap[page/8]|=(1<<(page%8));
   mapdirty=1;
}
/**********************************************************************/
page *dbfile::newpage()
{
   int x=pop();
   if (x!=-1)
   {
      setbusy(x);
      return getpage(x);
   }
   return addpage();
}
void dbfile::sync()
{
   if (mapdirty) 
   {
      lseek(mapfd,0,SEEK_SET);
      write(mapfd,freemap,mapbytes);
      fsync(mapfd);
      mapdirty=0;
   }
   for (int x=0;x<npages;x++) if (pages[x]&&pages[x]->dirty)
      writepage(pages[x]);     
   fsync(fd);
   initfreestack();
}
int dbfile::getpagesize()
{
   return pagesize;
}
page *dbfile::getpage(int pagenum)
{
   unsigned long offset;
   if (pagenum<0||pagenum>npages||isfree(pagenum))
   {
      seterror(ERR_NOSUCHPAGE,NULL);
      return NULL;
   }
   page *returnpage=pages[pagenum];
   if (returnpage)
   {
      returnpage->accesses++;
      returnpage->setload();
      manager->incvar(cachehitvar);
      return returnpage;
   }
   manager->incvar(cachefailvar);
   returnpage=new page(this);
   offset=pagesize*pagenum;
   int result=readdata(returnpage->data,pagesize,offset);
   if (result<0)
   {
      perror("readdata");
      delete returnpage;
      return NULL;
   }
   (*watermark)-=pagesize;
   returnpage->pagenum=pagenum;
   returnpage->accesses=1;
   returnpage->dirty=0;
   returnpage->setload();
   seterror(ERR_OK,NULL);
   return pages[pagenum]=returnpage;
}
void dbfile::dropfromcache(int pagenum)
{
   delete(pages[pagenum]);
   pages[pagenum]=NULL;
   (*watermark)+=pagesize;
}
void dbfile::droppage(page *p)
{
   #ifdef DEBUG
      if (p->accesses==0) printf("Dropping non accessed page.\n");
   #endif
   p->accesses--;
   if (*watermark<=0&&p->accesses==0&&p->dirty==0)
      dropfromcache(p->pagenum);
}
void dbfile::freepage(int pagenum)
{
   setfree(pagenum);
   if (pages[pagenum])
   { 
      #ifdef DEBUG
         if (pages[pagenum]->accesses>0) printf("Freeing accessed page.\n");
      #endif
      dropfromcache(pagenum);
   }
   push(pagenum);
}
void dbfile::freepage(page *p)
{
   freepage(p->pagenum);
}
char *dbfile::getname()
{
   return name;
}
int dbfile::pop()
{
   if (!stackp) return -1;
   return freestack[--stackp];
}
void dbfile::push(int pagenum)
{
   if (stackp==STACKSIZE) return;
   freestack[stackp++]=pagenum;
}
void dbfile::checkcache()
{
   for (int x=0;x<npages;x++) if (pages[x])
   {
      pages[x]->load>>=1;
      if ((pages[x]->load>16&&*watermark>0)||pages[x]->dirty||
         pages[x]->accesses) continue;
      dropfromcache(x);
   }
}
void dbfile::pushbuffer(char *buf)
{
   if (bufstackp==STACKSIZE) free(buf); else
      bufstack[bufstackp++]=buf;
}
char *dbfile::getbuffer()
{
   if (!bufstackp) return (char*)malloc(pagesize);
   return bufstack[--bufstackp];
}
page::page(dbfile *p)
{
   parent=p, data=parent->getbuffer(), load=0;
}
page::~page()
{
   parent->pushbuffer(data);
}
inline void page::setload()
{
   load+=32;
   if (load>BOUND) load=BOUND;
}
