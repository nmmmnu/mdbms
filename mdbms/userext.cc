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
#include <ctype.h>
#include "global.hh"
#include "userext.hh"
#include "support.h"
userext::userext(user *p, dbms *d):absuserext(p)
{
   bufsize=1024, trans=NULL;
   buf=(char*)malloc(bufsize);
   tree=NULL, username=NULL, dbm=d, buf[0]='\0';  
   p->uprintf("# "PRODUCT" ready.\n");
   FILE *issue=fopen(BASE "/issue.txt","r");
   if (!issue) return;
   char line[1024];
   while (fgets(line,1024,issue))
      p->uprintf("# %s",line);
   fclose(issue);
}
userext::~userext()
{
   free(buf);
   if (username) free(username);
   if (trans)
   {
      delete trans;
      parent->unload();
   }
}
void userext::parse(char *s)
{
   parent->setactive();
   if (s[0]!='\\') addline(s); else switch(s[1])
   {
      case 'q': parent->alive=0; break;
      case 'e': buf[0]='\0'; break;
      case 'g': doparse(); break;
      case 's': parent->uprintf("%s",buf); break;
      case 'h': help(s); break;
      case 't': status(); break;
      case 'l': list(s); break;
      default : parent->uprintf("Syntax error, type \\h for help\n");
   }
}
void userext::help(char *s)
{
   int copymode=0, topicmode=0;
   char topic[100],line[100];
   s+=2; while (isspace(*s)) s++;
   if (!*s) strcpy(topic,"global"); else strcpy(topic,s);
   FILE *io=fopen(BASE "/help.txt","r"); 
   if (!io)
   {
      parent->uprintf("Could not open help file 'help.txt'\n");
      return;
   }
   if (!strcmp(s,"topics")) topicmode=1;
   while (fgets(line,100,io))
   {
      if (strncmp(line,"%topic",6)==0)
      {
         char top[100], *p;
         if (copymode) break;
         if (sscanf(line,"%%topic %s:%s",top)!=1) continue;
         p=strchr(top,':');
         if (p) *(p++)='\0';
         p=strchr(line,':');
         if (p) *(p++)='\0';
         if (topicmode)
         {
            parent->uprintf("%s: %s",top,p);
            continue;
         }
         if (strcasecmp(top,topic)==0)
         {
            int x;
            char line[100];
            char *header="MDBMS ONLINE DOCUMENTATION";
            if (p==NULL) p="";
            parent->uprintf("%s %53s",header,p);
            for (x=0;x<79;x++) line[x]='=';
            line[79]='\0';
            parent->uprintf("%s\n",line);
            copymode=1;
         }
      } else if (copymode) parent->uprintf("%s",line);
   }
   fclose(io);
}
void userext::status()
{
   if (username) parent->uprintf("You are authorized as \"%s\".\n",username); else
      parent->uprintf("You are not authorized.\n");
   if (currentdb) parent->uprintf("Current database is \"%s\".\n",currentdb->getname());
      else parent->uprintf("There's no current database.\n");
}
void userext::list(char *s)
{
   char buf[100];
   s+=2; while (isspace(*s)) s++;
   if (!*s) s=NULL;
   int nvars=manager->getnvars();
   for (int x=0;x<nvars;x++) if (manager->sprintvalue(x,buf))
   {
      char *varname=manager->getvar(x)->name;
      if (!s||strstr(varname,s)) parent->uprintf("%s=%s\n",varname, buf);
   }
}
void userext::addline(char *s)
{
   char *p;
   int size=strlen(buf)+strlen(s)+2;
   if (size>=bufsize) buf=(char*)realloc(buf,bufsize=size);
   strcat(buf,s);
   strcat(buf,"\n");
   p=buf+strlen(buf)-1;
   while (isspace(*p)||*p=='\n') p--;
   if (*p==';') doparse();
}
void userext::doparse()
{
   if (!buf[0])
   {
      parent->uprintf("Buffer empty.\n");
      return;
   }
   tree=new sqltree(buf);
   buf[0]='\0';
   parent->setload();
   if (tree->getnerrors()==0)
   {
      trans=new transaction(dbm, &currentdb, &username, this);
      trans->execute(tree);
   } 
   int nerrors=tree->getnerrors();
   if (nerrors==0)
   {
      char *m=tree->getmessages();
      if (m) parent->uprintf("%s\n",m);
      parent->uprintf("Successful.\n");
   } else
      for (int x=0;x<nerrors;x++) parent->uprintf("%s",tree->getserror(x)); 
   if (nerrors||trans->busy==0)
   {
      delete trans;
      trans=NULL;
      parent->unload();
      return;
   } 
   parent->block();
   if (trans->type==NODE_SELECT)
   {
      char buf[10000];
      sprintheader(buf,trans->currentsearch);
      parent->uprintf("\n%s\n",buf);
   }
}
void userext::run()
{
   if (!trans) return;
   char buf[1000]="";
   int count=0, res;
   do
   {
      res=trans->run();
      if (!res) break;
      if (trans->recordready)
      {
         sprintrecord(buf,trans->currentsearch);
         parent->uprintf("%s\n",buf);
      }
   } while (count++<10);
   if (!res)
   {
      buf[0]='\0';
      if (trans->type==NODE_SELECT)
      {
         closeheader(buf,trans->currentsearch);
         parent->uprintf("%s\n",buf);
      }
      parent->uprintf("\n%d row(s).\n",trans->amount);
      delete trans;
      trans=NULL;
      parent->unblock();
      parent->unload();
   }
}
char *userext::sprintheader(char *s, sqlsearch *search)
{
   char buf[10000], format[10];
   int x, size;
   s[0]='\0';
   for (x=0;x<search->nattributes;x++)
   {
      attentry *at=&search->attributes[x];
      char *name=at->name;
      int len=strlen(name), type=at->tree?at->tree->result:at->at->type;
      if (type==ATT_VARCHAR) size=at->at?at->at->size:30; else
      if (type==ATT_DATE) size=21; else size=10;
      if (len>size) size=len+2;
      at->asciilen=size;
   }
   closeheader(s, search);
   strcat(s,"\n");
   for (x=0;x<search->nattributes;x++)
   {
      attentry *at=&search->attributes[x];
      char *name=at->name;
      sprintf(format,"|%%-%ds", at->asciilen);
      sprintf(buf, format, name);
      strcat(s, buf);
   }
   strcat(s,"|");
   strcat(s,"\n");
   closeheader(s, search);
   return s;
}
void userext::sprintfdate(char *buf, time_t date)
{
   char b[100];
   struct tm *timep=localtime(&date);
   buf[0]='\0';
   if (timep->tm_year!=2||timep->tm_mday!=1||timep->tm_mon!=0)
      sprintf(buf,"%d %s %d",timep->tm_mday, months[timep->tm_mon],
      timep->tm_year+1900);
   if (timep->tm_hour||timep->tm_sec||timep->tm_min)
   {
      sprintf(b,"%s%02d:%02d:%02d", buf[0]?" ":"", timep->tm_hour, timep->tm_min,
         timep->tm_sec);
      strcat(buf,b);
   }
}
void userext::sprintmoney(char *buf, double val, int len)
{
   char format[10], value[100];
   sprintf(value,"%.2f",val);
   sprintf(format,"$%%%ds",--len);
   sprintf(buf,format,value);
}
char *userext::sprintrecord(char *s, sqlsearch *search)
{
   char format[10], storage[1000], tb[100];
   int x;
   strcpy(s,"|");
   for (x=0;x<search->nattributes;x++)
   {
      char *buf;
      value *cv=&search->currentrecord[x];
      int len=search->attributes[x].asciilen;
      if (cv->size==0) sprintf(buf=tb,"NULL"); else switch (cv->type)
      {
         case ATT_CHAR    :
         case ATT_VARCHAR : buf=cv->data.string;
                            if (strlen(buf)>len) buf[len]='\0'; break;
         case ATT_INT     : sprintf(buf=tb,"%d",*cv->data.integer); break;
         case ATT_FLOAT   : sprintf(buf=tb,"%.2f",*cv->data.floating); break;
         case ATT_DATE    : sprintfdate(buf=tb,*cv->data.date); break;
         case ATT_MONEY   : sprintmoney(buf=tb,*cv->data.floating,len); break;
         case ATT_NULL    : sprintf(buf=tb,"NULL"); break;
      }
      sprintf(format,"%%-%ds|", len);
      sprintf(storage,format,buf);
      strcat(s,storage);
   }
   return s;
}
char *userext::closeheader(char *s, sqlsearch *search)
{
   int c=0;
   char buf[10000];
   for (int x=0;x<search->nattributes;x++)
   {
      buf[c++]='+';
      attentry *at=&search->attributes[x];
      for (int y=0;y<at->asciilen;y++) buf[c++]='-';
   }
   buf[c++]='+';
   buf[c]='\0';
   strcat(s,buf);
   return s;
}
void userext::sendping()
{
/*   parent->uprintf("\n# Are you there?\n");
   parent->printprompt();*/
}
