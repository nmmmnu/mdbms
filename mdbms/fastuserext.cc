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
#include "fastuserext.hh"
#include "protocol.h"
#include "support.h"
#include "sql.hh"
fastuserext::fastuserext(user *p, dbms *d):absuserext(p)
{
   trans=NULL, transnum=-1;
   tree=NULL, username=NULL, dbm=d;
   p->uprintf("# "PRODUCT" FOP interface port.\n");
}
fastuserext::~fastuserext()
{
   if (username) free(username);
   if (trans)
   {
      delete trans;
      parent->unload();
   }
}
void fastuserext::parse(char *s)
{
   parent->setactive();
   if (s[0]!='\\') doparse(s); else switch(s[1])
   {
      case 'q': parent->alive=0; break;
      case 'h': dohelp(s+2); break;
      case 'p': break;
      default : sscanf(s+1,"%d",&transnum); break;
   }
}
void fastuserext::showcode(int code)
{
   parent->uprintf("!%d\n",code);
}
void fastuserext::doparse(char *buf)
{
   if (!buf[0]) return;
   tree=new sqltree(buf);
   showcode(MDP_QUERYRESPONSE);
   parent->uprintf("%d\n",transnum);
   transnum=-1;
   if (tree->getnerrors()==0)
   {
      trans=new transaction(dbm, &currentdb, &username, this);
      trans->execute(tree);
   } 
   parent->setload();
   int nerrors=tree->getnerrors();
   if (nerrors==0) showcode(MDP_OK); else
   {
      showcode(MDP_ERROR);
      parent->uprintf("%d\n",nerrors);
      for (int x=0;x<nerrors;x++)
      {
         error *err=tree->geterror(x);
         parent->uprintf("%d %s\n",err->num, err->string?err->string:""); 
      }
   }
   if (nerrors||trans->busy==0)
   {
      showcode(MDP_READY);
      delete trans;
      trans=NULL;
      parent->unload();
      return;
   }
   parent->block();
   switch(trans->type)
   {
      case NODE_SELECT :
            showcode(MDP_STARTDATA);
            parent->uprintf("%d\n",trans->currentsearch->nattributes);
            for (int x=0;x<trans->currentsearch->nattributes;x++)
            {
               attentry *at=trans->currentsearch->attributes+x;
               parent->uprintf("%d %d %s\n",at->type,at->asciilen,at->name);
            }
            break;
   }
}
void fastuserext::run()
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
         sprintrecord(buf, trans->currentsearch);
         parent->uprintf("%s", buf);
      }
   } while (count++<20);
   if (!res)
   {
      showcode(MDP_READY);
      delete trans;
      trans=NULL;
      parent->unblock();
      parent->unload();
   }
}
void fastuserext::sprintrecord(char *s, sqlsearch *search)
{
   char tb[100];
   s[0]='\0';
   for (int x=0;x<search->nattributes;x++)
   {
      char *buf;
      value *cv=&search->currentrecord[x];
      if (cv->size==0) sprintf(buf=tb,"\\NULL"); else switch (cv->type)
      {
         case ATT_CHAR    :
         case ATT_VARCHAR : buf=cv->data.string; break;
         case ATT_INT     : sprintf(buf=tb,"%d",*cv->data.integer); break;
         case ATT_FLOAT   : sprintf(buf=tb,"%f",*cv->data.floating); break;
         case ATT_DATE    : sprintf(buf=tb,"%lu",*cv->data.date); break;
         case ATT_MONEY   : sprintf(buf=tb,"%.2f",*cv->data.floating); break;
         case ATT_NULL    : sprintf(buf=tb,"\\NULL"); break;
      }
      if (buf[0]=='!') strcat(s,"\\");
      strcat(s,buf); strcat(s,"\n");
   }
}
void fastuserext::dohelp(char *s)
{
   showcode(MDP_QUERYRESPONSE);
   parent->uprintf("%d\n",transnum);
   transnum=-1;
   while (isspace(*s)) s++;
   table *t=currentdb?currentdb->gettable(s):(table*)NULL;
   if (!t)
   { 
      showcode(MDP_ERROR);
      return;
   }
   showcode(MDP_STARTHELP);
   int max=t->getnattributes(),x;
   parent->uprintf("%d\n",max);
   for (x=0;x<max;x++)
   {
      attribute *at=t->getattribute(x);
      parent->uprintf("%d %d %d %d %s\n",at->type,at->size,at->nulls?1:0,
         at->primary?1:0,at->name);
   }
   for (x=0;t->getfkey(x);x++)
   {
      parent->uprintf("%s %d",t->getfkey(x),t->getaction(x));
      for (int y=0;y<max;y++)
      {
         attribute *at=t->getattribute(y);
         if (at->foreignkey==x) parent->uprintf(" %s",at->name);
      }
      parent->uprintf("\n");
   }
   showcode(MDP_READY);
}
void fastuserext::sendping()
{
   showcode(MDP_PING);
}
