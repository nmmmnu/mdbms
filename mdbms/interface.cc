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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include "global.hh"
#include "interface.hh"
#include "userext.hh"
#include "fastuserext.hh"
#include "dbms.hh"
#include "relation.hh"
#include "support.h"

tcpinterface::tcpinterface(int port, dbms *d, int t,
   int currentvar, int totalvar, int timeoutvar)
{
   int on=1;
   struct sockaddr_in sa;
   varcurrent=currentvar, vartotal=totalvar, vartimeout=timeoutvar;
   memset(&sa, 0, sizeof(sa));
   sa.sin_family=AF_INET;
   sa.sin_port=htons(port);
   sa.sin_addr.s_addr=htonl(INADDR_ANY);
   sock=socket(AF_INET,SOCK_STREAM,0);
   if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &on,
      sizeof(on))<0)
   {
      perror("setsockopt");
      exit(1);
   }
   if (bind(sock,(struct sockaddr *)&sa, sizeof(sa))<0)
   {
      perror("bind");
      exit(1);
   }
   listen(sock,5);
   FD_ZERO(&rmask);
   FD_ZERO(&wmask);
   FD_SET(sock,&rmask);
   rootuser=NULL, prompt=NULL;
   dbm=d->getdbman(), parent=d, type=t;
}
tcpinterface::~tcpinterface()
{
   if (prompt) free(prompt);
   close(sock);
}
void tcpinterface::resetmasks()
{
   user *temp;
   FD_ZERO(&rmask);
   FD_ZERO(&wmask);
   FD_SET(sock,&rmask);
   for (temp=rootuser;temp;temp=temp->next) temp->setmasks();
}
int tcpinterface::doselect(int quick)
{
   struct timeval timeout={0,10000};
   int amount;
   if (quick) timeout.tv_usec=0;
   rrmask=rmask, rwmask=wmask;  
   amount=select(256,&rrmask,&rwmask,NULL,&timeout);
   if (amount<0)
   {
      resetmasks();
      return 0;
   }
   return amount;
}
void tcpinterface::newuser()
{
   user *temp;
   int fd=accept(sock,NULL,0);
   if (fd<0) return;
   temp=new user(fd, parent, this);
   temp->next=rootuser;
   if (rootuser) rootuser->prev=temp;
   rootuser=temp;
   if (prompt)
   {
      temp->setprompt(prompt);
      temp->printprompt();
   }
   manager->incvar(vartotal);
   manager->incvar(varcurrent);
}
void tcpinterface::removeuser(user *u)
{
   if (u->next) u->next->prev=u->prev;
   if (u->prev) u->prev->next=u->next; else rootuser=u->next;
   FD_CLR(u->fd,&rmask);
   FD_CLR(u->fd,&wmask);
   manager->decvar(varcurrent);
   delete u;
}
void tcpinterface::run(int quick)
{
   user *temp;
   time_t now=time(NULL);
   doselect(quick);
   if (FD_ISSET(sock,&rrmask)) newuser();
   temp=rootuser;
   while (temp)
   {
      user *next=temp->next;
      if (temp->alive!=0)
      {
         if (temp->timeout)
         {
            temp->alive=0;
            manager->incvar(vartimeout);
         }
         if (now-temp->lastping>=USERPINGTIMEOUT)
         {
            temp->sendping();
            temp->lastping=now;
         } else if (now-temp->lastactive>=USERTIMEOUT)
         {
            temp->uprintf("# Timeout\n");
            temp->timeout=1;
         }
         if (FD_ISSET(temp->fd,&rrmask))
         {
            temp->input();
            temp->printprompt();
         }
         temp->run();
         if (FD_ISSET(temp->fd,&rwmask)) temp->output();
      } else removeuser(temp);
      temp=next;
   }
}
void tcpinterface::setprompt(char *s)
{
   user *temp;
   if (prompt) free(prompt);
   prompt=strdup(s);
   for (temp=rootuser;temp;temp=temp->next) temp->setprompt(s);
}
void tcpinterface::setbusy()
{
   parent->setbusy();
}
void tcpinterface::setfree()
{
   parent->setfree();
}
int tcpinterface::dbinuse(database *db, absuserext *who)
{
   user *temp; 
   for (temp=rootuser;temp;temp=temp->next)
      if (temp->ext!=who&&temp->ext->getcurrentdb()==db) return 1;
   return 0;
}
/*******************************************************************/
user::user(int d, dbms *dbm, tcpinterface *p)
{
   next=prev=NULL, alive=1, fd=d, insize=outsize=1024;
   inbuf=new char[insize], outbuf=new char[outsize];
   inbuf[0]=outbuf[0]='\0'; blocked=0, prompt=NULL;
   parent=p, timeout=0;
   setactive();
   setmasks();
   if (p->type==0) ext=new userext(this,dbm); else
      ext=new fastuserext(this,dbm);
}
user::~user()
{
   close(fd); 
   delete ext;
   delete inbuf;
   delete outbuf;
   if (prompt) free(prompt);
}
void user::setactive()
{
   lastactive=lastping=time(NULL);
}
void user::setmasks()
{
   FD_SET(fd,&parent->rmask);
   if (strlen(outbuf)) FD_SET(fd,&parent->wmask);
}
void user::input()
{
   char buf[1025];
   int bytes=read(fd,buf,1024);
   if (bytes<=0/*||!isprint(buf[0])*/)
   {
      alive=0;
      return;
   }
   buf[bytes]='\0';
   bytes+=strlen(inbuf)+1;
   if (bytes>insize) inbuf=(char*)realloc(inbuf,insize=bytes);
   strcat(inbuf,buf);
   run();
}
int user::nextline(char *source, char *dest, int limit)
{
   char *where=mstrchr(source,'\n'), *p;
   if (!where) return -1;
   if ((where-source)>limit)
      return -1;
   *(where++)='\0';
   p=mstrchr(source,'\r');
   if (p) *p='\0';
   strcpy(dest,source);
   memmove(source,where,strlen(where)+1);
   return where-source;
}
void user::output()
{
   if (write(fd,outbuf,strlen(outbuf))<0)
   {
      alive=0;
      return;
   }
   outbuf[0]='\0';
   fsync(fd);
   FD_CLR(fd,&parent->wmask);
}
void user::uprintf(char *s, ...)
{
   char b[10000];
   int len=strlen(outbuf), newlen;
   va_list ap;
   va_start(ap,s);
   vsnprintf(b,10000,s,ap);
   va_end(ap);
   newlen=strlen(b);
   while (newlen+len+10>=outsize) outbuf=(char*)realloc(outbuf,outsize+=1000);
   strcat(outbuf,b);
   FD_SET(fd,&parent->wmask);
}
void user::run()
{
   char buf[10000];
   ext->run();
   if (blocked) return;
   while (nextline(inbuf,buf,sizeof(buf))!=-1)
   {
      ext->parse(buf);
      if (blocked) break;
   }
}
void user::block()
{
   blocked=1;
}
void user::unblock()
{
   blocked=0;
   printprompt();
}
void user::setprompt(char *s)
{
   if (prompt) free(prompt);
   prompt=strdup(s);
}
void user::printprompt()
{
   if (!prompt||!alive||blocked) return;
   uprintf("%s",prompt);
}
void user::setload()
{
   parent->setbusy();
}
void user::unload()
{
   parent->setfree();
}
void user::sendping()
{
   ext->sendping();
}
