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
#include <time.h>
#include <unistd.h>
#include "global.hh"
#include "support.h"
#include "sqlsearch.hh"
#include "nodes.h"

#define COND_CHECK 1
#define COND_MET   2

sqlsearch::sqlsearch(dbmanager *d, database *db, node *t, sqlsearch *p,
   int perm, char *who)
{
   dbase=db, dbm=d, ntables=0, tables=NULL, forwardsearch=0, parent=p;
   attributes=NULL, nattributes=0, tree=t, currentrecord=NULL, amount=0;
   globalvalue=NULL, forward=0, delayed=0, firstentry=1, nrecords=0;
   select=tree->n1, from=tree->n2->n1, where=tree->n2->n2->n1, nsubs=0;
   subselect=NULL, localtables=0, groupsready=0, orderready=0, orderby=NULL;
   groupby=NULL, groupbys=NULL, orderbys=NULL, permissions=perm, permwho=who;
   if (tree->n2->n2->n2->type==NODE_GROUP)
      groupby=tree->n2->n2->n2->n1; else
      orderby=tree->n2->n2->n2->n1;
   seterror(ERR_OK, NULL);

   success=filltables(); if (!success) return;
   if (groupby) success=checkgroupby(); if (!success) return;
   if (orderby) success=checkorderby(); if (!success) return;
   success=fillattr(); if (!success) return;
   if (nattributes) currentrecord=(value*)malloc(sizeof(value)*nattributes);
   fillts();
   if (where) success=checkwhere(where,1); if (!success) return;
   if (tree->type==NODE_UPDATE) success=checkupdate();
}
int sqlsearch::checkupdate()
{
   int localtype=attributes[0].type, desired=attributes[1].type;
   if (localtype==desired||localtype==ATT_NULL) return 1;
   if (desired==ATT_CHAR||desired==ATT_VARCHAR)
      if (localtype==ATT_CHAR||localtype==ATT_VARCHAR) return 1;
   if (desired==ATT_MONEY)
      if (localtype==ATT_INT||localtype==ATT_FLOAT) return 1;
   if (desired==ATT_FLOAT||desired==ATT_INT)
      if (localtype==ATT_INT||localtype==ATT_FLOAT) return 1;
   char buffer[100];
   sprintf(buffer,"Can't convert %s to %s", attnames[localtype],
      attnames[desired]);
   seterror(ERR_CONVERT, buffer);
   return 0;
}
void sqlsearch::clearstorage()
{
   for (int x=0;x<nattributes;x++)
   {
      globalv *val=&attributes[x].storage;
      if (attributes[x].dupped&&(attributes[x].type==ATT_CHAR||
         attributes[x].type==ATT_VARCHAR))
         if (val->string) free(val->string);
      memset(val, 0, sizeof(globalv));
      attributes[x].dupped=0;
   }
}
void sqlsearch::reset()
{
   int x;
   clearstorage();
   for (x=0;x<nsubs;x++) subselect[x].search->reset();
   for (x=0;x<ntables;x++) if (tables[x].parent==0)
   {
      if (tables[x].currentr) delete tables[x].currentr;
      tables[x].currentr=NULL;
      tables[x].ts->rewind();
   }
   amount=nrecords=delayed=0;
   firstentry=1, groupsready=0, orderready=0;
   if (groupbys) groupbys->reset();
   if (orderbys) orderbys->reset();
}
sqlsearch::~sqlsearch()
{
   int x;
   for (x=0;x<nsubs;x++) delete subselect[x].search;
   if (subselect) free(subselect);
   for (x=0;x<ntables;x++)
   {
      if (tables[x].parent) continue;
      if (tables[x].ts) delete tables[x].ts;
      if (tables[x].currentr) delete tables[x].currentr;
   }
   clearstorage();
   if (currentrecord) free(currentrecord);
   if (tables) free(tables);
   if (attributes) free(attributes);
   if (groupbys) delete groupbys;
   if (orderbys) delete orderbys;
}
int sqlsearch::fillall()
{
   int c=0, x, y;
   for (x=0;x<localtables;x++) nattributes+=tables[x].tab->getnattributes();
   attributes=(attentry*)calloc(nattributes,sizeof(attentry));
   for (x=0;x<localtables;x++)
   {
      table *temp=tables[x].tab;
      for (y=0;y<temp->getnattributes();y++)
      {
         attribute *ta=temp->getattribute(y);
         attributes[c].at=ta, attributes[c].tabnum=x;
         attributes[c].tabentry=y, attributes[c].tree=NULL;
         attributes[c].type=ta->type, attributes[c].asciilen=ta->size;
         strcpy(attributes[c++].name,ta->name);
      }
   }
   return 1;
}
int sqlsearch::parseattribute(node *n)
{     
   if (n->s2)
   {
      int ok=0, y, tabnum;
      table *tab;
      for (int x=0;x<ntables&&!ok;x++)
         if (!strcasecmp(tables[x].name,n->s2))
            ok=1, tabnum=x;
      if (!ok)
      {
         seterror(ERR_NOSUCHTBL,n->s2);
         return 0;
      }
      ok=0, tab=tables[tabnum].tab;
      for (y=0;y<tab->getnattributes();y++)
      {
         attribute *ta=tab->getattribute(y);
         if (!strcasecmp(ta->name,n->s1))
         {
            ok=1, n->num1=tabnum, n->num2=y, n->result=ta->type;
            globaltype=ta->type;
            break;
         }
      }
      if (!ok)
      {
         seterror(ERR_NOSUCHATTR,n->s1);
         return 0;
      }
   } else
   {
      int ok=0, x, y;
      for (x=0;x<ntables&&!ok;x++)
      {
         table *temp=tables[x].tab;
         for (y=0;y<temp->getnattributes();y++)
         {
            attribute *ta=temp->getattribute(y);
            if (!strcasecmp(ta->name,n->s1))
            {
               ok=1, n->num1=x, n->num2=y, n->result=ta->type;
               globaltype=ta->type;
               break;
            }
         }
      }
      if (!ok)
      {
         seterror(ERR_NOSUCHATTR,n->s1);
         return 0;
      }
   }
   return 1;
}
int sqlsearch::checkaddsub(node *n)
{
   int type1, type2, ok=0;
   if (!checktree(n->n1)) return 0; type1=globaltype;
   if (!checktree(n->n2)) return 0; type2=globaltype;
   if (!(n->type==NODE_SUBSTRACT&&(type1!=ATT_DATE&&type2==ATT_DATE)))
   switch (type1)
   {
      case ATT_INT   : ok=1, globaltype=type2; break;
      case ATT_FLOAT : if (type2==ATT_INT)
                          { ok=1, globaltype=ATT_FLOAT; break; }
                       ok=1, globaltype=type2; break;
      case ATT_MONEY : if (type2!=ATT_DATE)
                          { ok=1, globaltype=ATT_MONEY; }
                       break;
      case ATT_DATE  : if (type2==ATT_INT||type2==ATT_FLOAT)
                          { ok=1, globaltype=ATT_DATE; }
                       if (type2==ATT_DATE&&n->type==NODE_SUBSTRACT)
                          { ok=1, globaltype=ATT_INT; }
                       break;
   }
   if (!ok)
   { 
      char buf[100], *types, *fromto;
      if (n->type==NODE_ADD) types="add", fromto="to"; else
         types="substract", fromto="from";
      sprintf(buf,"Can't %s %s %s %s",types,attnames[type1],fromto,
         attnames[type2]);
      seterror(ERR_MATHERROR,buf);
      return 0;
   }
   n->result=globaltype;
   return 1;
}
int sqlsearch::checkmuldiv(node *n)
{
   int type1, type2, ok=0;
   if (!checktree(n->n1)) return 0; type1=globaltype;
   if (!checktree(n->n2)) return 0; type2=globaltype;
   switch (type1)
   {
      case ATT_INT   : if (n->type==NODE_DIVIDE)
                       {
                          if (type2==ATT_DATE||type2==ATT_MONEY) break;
                          ok=1, globaltype=ATT_FLOAT; break;
                       }
                       if (type2!=ATT_DATE)
                          { ok=1, globaltype=type2; }
                       break;
      case ATT_FLOAT : if (type2==ATT_INT||type2==ATT_FLOAT)
                          { ok=1, globaltype=ATT_FLOAT; }
                       if (type2==ATT_MONEY)
                          { ok=1, globaltype=ATT_MONEY; }
                       break;
      case ATT_MONEY : if (type2==ATT_INT||type2==ATT_FLOAT)
                          { ok=1, globaltype=ATT_MONEY; }
                       break;
      case ATT_DATE  : break;
   }
   if (!ok)
   { 
      char buf[100], *types, *with;
      if (n->type==NODE_MULTIPLY) types="multiply", with="with"; else
         types="divide", with="by";
      sprintf(buf,"Can't %s %s %s %s",types,attnames[type1],with,
         attnames[type2]);
      seterror(ERR_MATHERROR,buf);
      return 0;
   }
   n->result=globaltype;
   return 1;
}
int sqlsearch::checkunary(node *n)
{
   int type1;
   if (!checktree(n->n1)) return 0;
   n->result=type1=globaltype;
   if (type1!=ATT_INT&&type1!=ATT_MONEY&&type1!=ATT_FLOAT)
   {
      char buf[100];
      const char *types=n->type==NODE_UMINUS?"minus":"plus";
      sprintf(buf,"Can't apply unary %s to %s",types,attnames[type1]);
      seterror(ERR_MATHERROR,buf);
      return 0;
   }
   return 1;
}
int sqlsearch::checkaggregate(node *n)
{
   if (n->n1->type==NODE_STAR)
   {
      n->result=ATT_INT;
      if (n->num1==FUNC_COUNT) return 1;
      seterror(ERR_STAR, NULL);
      return 0;
   }
   if (!checktree(n->n1)) return 0;
   int res=globaltype=n->result=n->n1->result;
   if (n->num1==FUNC_COUNT) res=n->result=ATT_INT;
   if (n->num1==FUNC_AVG&&res==ATT_INT) n->result=ATT_FLOAT;
   if (res==ATT_INT||res==ATT_FLOAT||res==ATT_MONEY) return 1;
   if (n->num1==FUNC_MIN||n->num1==FUNC_MAX) return 1;
   char buf[100];
   sprintf(buf,"Can't use %s in aggregate function",attnames[res]);
   seterror(ERR_PARAMETER, buf);
   return 0;
}
int sqlsearch::checkfunction(node *n)
{
   if (!checktree(n->n1)) return 0;
   if (n->n1->result!=ATT_DATE)
   {
      char buf[100];
      sprintf(buf,"%s(%s)",funcnames[n->num1],attnames[n->n1->result]);
      seterror(ERR_PARAMETER, buf);
      return 0;
   }
   switch (n->num1)
   {
      case FUNC_YEAR: case FUNC_MONTH: case FUNC_DAY: case FUNC_HOUR:
      case FUNC_MINUTE: case FUNC_SECOND: case FUNC_DAYOFWEEK: case FUNC_WEEKNUM:
            globaltype=n->result=ATT_INT; break;
      case FUNC_STRIPTIME: case FUNC_STRIPDATE:
            globaltype=n->result=ATT_DATE; break;
   }
   return 1;
}
int sqlsearch::checkprocedure(node *n)
{
   switch (n->num1)
   {
      case FUNC_DATE: globaltype=n->result=ATT_DATE; break;
      case FUNC_ROWNUM: globaltype=n->result=ATT_INT; break;
   }
   return 1;
}
int sqlsearch::checktree(node *n)
{
   switch (n->type)
   {
      case NODE_IDENT    : return parseattribute(n);
      case NODE_MULTIPLY :
      case NODE_DIVIDE   : return checkmuldiv(n);
      case NODE_SUBSTRACT:
      case NODE_ADD      : return checkaddsub(n);
      case NODE_UPLUS    :
      case NODE_UMINUS   : return checkunary(n);
      case NODE_ATTRIBUTE: globaltype=n->result=n->num1; return 1;
      case NODE_COLFUNC  : return checkaggregate(n);
      case NODE_FUNCTION : return checkfunction(n);
      case NODE_PROCEDURE: return checkprocedure(n);

      default : seterror(ERR_NOTSUPPORTED,NULL);
   }
   return 0;
}
int sqlsearch::fillattr()
{
   int c=0, type=tree->type;
   if (!select) return 1;
   node *tree=select;
   if (tree->type==NODE_STAR) return fillall();
   node *temp=tree;
   for (nattributes=0;temp;temp=temp->n2) nattributes++;
   attributes=(attentry*)calloc(nattributes,sizeof(attentry));
   do
   {
      attentry *thisatt=attributes+c;
      node *data=tree->n1;
      if (data->s1) strncpy(thisatt->name,data->s1,39);
         else thisatt->name[0]='\0';
      thisatt->tree=data=data->n1, thisatt->at=NULL;
      thisatt->asciilen=-1;
      if (thisatt->name[0]=='\0')
      {
         if (data->type==NODE_IDENT) strncpy(thisatt->name,data->s1,39); else
            sprintf(thisatt->name,"col %d",c);
      }
      if (!checktree(data)) return 0;
      thisatt->type=data->result;
      if (!groupby)
      {
         if (data->type==NODE_COLFUNC)
         {
            if (!forward&&c)
            {
               seterror(ERR_AGGREGATEMIX,NULL);
               return 0;
            }
            forward=1;
         } else if (forward)
         {
            seterror(ERR_AGGREGATEMIX,NULL);
            return 0;
         }
      }
      if (data->type==NODE_IDENT)
      {
         int tabnum=data->num1, entnum=data->num2;
         attribute *ta=tables[tabnum].tab->getattribute(entnum);
         thisatt->at=ta, thisatt->tabnum=tabnum;
         thisatt->tabentry=entnum, thisatt->tree=NULL;
         thisatt->asciilen=ta->size;
         if (type==NODE_UPDATE&&ta->primary==1)
         {
            seterror(ERR_MODIFYPRIMARY, ta->name); 
            return 0;
         }
      }
      tree=tree->n2, c++;
   } while (tree);
   return 1;
}
int sqlsearch::checkgroupby()
{
   int c, amount=0;
   node *temp;
   for (temp=groupby;temp;temp=temp->n2)
   {
      if (!checktree(temp->n1)) return 0;
      amount++;
   }
   groupbys=new sorter(amount, ntables, this);
   for (c=0,temp=groupby;temp;temp=temp->n2,c++)
      groupbys->set(c,temp->n1->result,temp->n1,1);
   return 1;
}
int sqlsearch::checkorderby()
{
   int c, amount=0;
   node *temp;
   for (temp=orderby;temp;temp=temp->n2)
   {
      if (!checktree(temp->n1->n1)) return 0;
      amount++;
   }
   orderbys=new sorter(amount, ntables, this);
   for (c=0,temp=orderby;temp;temp=temp->n2,c++)
      orderbys->set(c,temp->n1->n1->result,temp->n1->n1,temp->n1->num1);
   return 1;
}
int sqlsearch::checkand(node *n, int andonly)
{
   if (!checkwhere(n->n1,andonly)) return 0;
   if (!checkwhere(n->n2,andonly)) return 0;
   n->result=COND_CHECK;
   if (!andonly) return 1;
   if (n->n1->result==COND_MET&&n->n2->result==COND_MET) n->result=COND_MET;
   return 1;
}
int sqlsearch::checkor(node *n)
{
   if (!checkwhere(n->n1,0)) return 0;
   if (!checkwhere(n->n2,0)) return 0;
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checkequals(node *n, int andonly)
{
   if (!checktree(n->n1)) return 0;
   if (!checktree(n->n2)) return 0;
   if ((n->n1->result!=ATT_CHAR||n->n2->result!=ATT_VARCHAR)&&
       (n->n1->result!=ATT_VARCHAR||n->n2->result!=ATT_CHAR)) 
   if (n->n1->result!=n->n2->result)
   {
      char buffer[100];
      sprintf(buffer,"Can't compare %s with %s",attnames[n->n1->result],
         attnames[n->n2->result]);
      seterror(ERR_COMPARE, buffer);
      return 0;
   }
   n->result=COND_CHECK;
   if (!andonly||n->n1->type!=NODE_IDENT||n->n2->type!=NODE_ATTRIBUTE)
      return 1;
   if (n->n1->num1>=localtables) return 1;
   value v;
   tablesearch *ts=tables[n->n1->num1].ts;
   v.type=n->n2->result;
   v.id=n->n1->num2;
   switch(v.type)
   {
      case ATT_MONEY:
      case ATT_FLOAT: v.data.floating=&n->n2->fval; v.size=sizeof(double); break;
      case ATT_INT: v.data.integer=&n->n2->num2; v.size=sizeof(int); break;
      case ATT_CHAR:
      case ATT_VARCHAR: v.data.string=n->n2->s1; v.size=1+strlen(v.data.string);
                        break;
      case ATT_DATE: v.data.date=&n->n2->dval; v.size=sizeof(time_t); break;
   }
   ts->addvalue(&v);
   n->result=COND_MET;
   return 1;
}
int sqlsearch::checkother(node *n)
{
   if (!checktree(n->n1)) return 0;
   if (!checktree(n->n2)) return 0;
   if (n->n1->result!=n->n2->result)
   {
      char buffer[100];
      sprintf(buffer,"Can't compare %s with %s",attnames[n->n1->result],
         attnames[n->n2->result]);
      seterror(ERR_COMPARE, buffer);
      return 0;
   }
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checkinlist(node *n)
{
   node *temp;
   if (!checktree(n->n1)) return 0;
   for (temp=n->n2;temp;temp=temp->n2)
      if ((temp->n1->result=temp->n1->num1)!=n->n1->result)
   {
      char buffer[100];
      sprintf(buffer,"Can't compare %s with %s",attnames[n->n1->result],
         attnames[temp->n1->result]);
      seterror(ERR_COMPARE, buffer);
      return 0;
   }
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checkin(node *n)
{
   node *sub=n->n2->n1;
   if (!checktree(n->n1)) return 0;
   if (sub->type==NODE_STAR||sub->n2)
   {
      if (n->n1->type!=NODE_IDENT)
      {
         seterror(ERR_SUBMORECOLS,NULL);
         return 0;
      }
      delete sub;
      node *ident=new node(NODE_IDENT, strdup(n->n1->s1));
      node *exp=new node(NODE_COLEXP, ident, NULL);
      n->n2->n1=new node(NODE_MORE, exp, NULL);
   }
   sqlsearch *newsearch=new sqlsearch(dbm, dbase, n->n2, this, GRANTSELECT, permwho);
   if (newsearch->success==0)
   {
      delete newsearch;
      return 0;
   }
   int t1=n->n2->n1->n1->n1->result;
   int t2=n->n1->result;
   if (t1!=t2)
   {
      char buffer[100];
      sprintf(buffer,"Can't compare %s with %s",attnames[t1],
         attnames[t2]);
      seterror(ERR_COMPARE, buffer); 
      delete newsearch;
      return 0;
   }
   n->num1=nsubs++;
   if (subselect) subselect=(subs*)realloc(subselect,sizeof(subs)*nsubs); else
      subselect=(subs*)malloc(sizeof(subs));
   subselect[nsubs-1].search=newsearch;
   subselect[nsubs-1].needmore=0;
   subselect[nsubs-1].valueok=0;
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checkbetween(node *n)
{
   int t1,t2,t3;
   if (!checktree(n->n1)) return 0;
   if (!checktree(n->n2->n1)) return 0;
   if (!checktree(n->n2->n2)) return 0;
   t1=n->n1->result, t2=n->n2->n1->result, t3=n->n2->n2->result;
   if (t1!=t2||t1!=t3)
   {
      char buffer[100];
      sprintf(buffer,"Can't compare %s with %s",attnames[t1],
         attnames[t2]);
      seterror(ERR_COMPARE, buffer);
      return 0;
   }
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checkexists(node *n)
{
   sqlsearch *newsearch=new sqlsearch(dbm, dbase, n->n1, this, GRANTSELECT, permwho);
   if (newsearch->success==0)
   {
      delete newsearch;
      return 0;
   }
   n->num1=nsubs++;
   if (subselect) subselect=(subs*)realloc(subselect,sizeof(subs)*nsubs); else
      subselect=(subs*)malloc(sizeof(subs));
   subselect[nsubs-1].search=newsearch;
   subselect[nsubs-1].needmore=0;
   subselect[nsubs-1].valueok=0;
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checklike(node *n)
{
   if (!checktree(n->n1)) return 0;
   if (n->n1->result!=ATT_VARCHAR)
   {
      char buffer[100];
      sprintf(buffer,"Can't compare %s with expression",attnames[n->n1->result]);
      seterror(ERR_COMPARE, buffer);
      return 0;
   }
   n->result=COND_CHECK;
   return 1;
}
int sqlsearch::checkwhere(node *n, int andonly)
{
   switch (n->type)
   {
      case NODE_EQUALS: return checkequals(n, andonly);
      case NODE_LESS: case NODE_MORE: case NODE_NEQ: case NODE_LEQ:
      case NODE_MOQ: return checkother(n);
      case NODE_AND : return checkand(n,andonly);
      case NODE_OR  : return checkor(n);
      case NODE_NOT  : return checkwhere(n->n1,0);
      case NODE_NOT_NULL:
      case NODE_NULL: return checktree(n->n1);
      case NODE_INLIST: return checkinlist(n);
      case NODE_IN: return checkin(n);
      case NODE_BETWEEN: return checkbetween(n);
      case NODE_EXISTS: return checkexists(n);
      case NODE_LIKE: return checklike(n);
      default : break;
   }
   return 0;
}
int sqlsearch::filltables()
{
   int c;
   node *temp;
   localtables=0;
   for (temp=from;temp;temp=temp->n2) localtables++;
   ntables=localtables+(parent?parent->ntables:0);
   tables=(tabentry*)calloc(ntables,sizeof(tabentry));
   for (c=0, temp=from;temp;temp=temp->n2, c++)
   {
      node *data=temp->n1;
      tables[c].tab=dbase->gettable(data->s1);
      tables[c].ts=NULL;
      tables[c].currentr=NULL;
      if (!tables[c].tab)
      {
         seterror(ERR_NOSUCHTBL, data->s1);
         return 0;
      }
      if (!tables[c].tab->isgranted(permwho,permissions))
      {
         seterror(ERR_PERMISSION,NULL);
         return 0;
      }
      strncpy(tables[c].name, data->s2?data->s2:data->s1, 39);
   }
   for (c=localtables;c<ntables;c++)
   {
      tables[c]=parent->tables[c-localtables];
      tables[c].parent++;
   }
   return 1;
}
int sqlsearch::execfunction(node *n)
{
   if (!exectree(n->n1)) return 0;
   struct tm *tmp=localtime(&globalvalue->date);
   switch (n->num1)
   {
      case FUNC_YEAR:  globalvalue->integer=tmp->tm_year+1900; break;
      case FUNC_MONTH: globalvalue->integer=tmp->tm_mon+1; break;
      case FUNC_DAY:   globalvalue->integer=tmp->tm_mday; break;
      case FUNC_HOUR:  globalvalue->integer=tmp->tm_hour; break;
      case FUNC_MINUTE:globalvalue->integer=tmp->tm_min; break;
      case FUNC_SECOND:globalvalue->integer=tmp->tm_sec; break;
      case FUNC_DAYOFWEEK:globalvalue->integer=tmp->tm_wday; break;
      case FUNC_WEEKNUM:globalvalue->integer=getweekno(tmp); break;
      case FUNC_STRIPTIME: tmp->tm_isdst=-1, tmp->tm_year=2;
                           tmp->tm_mday=1, tmp->tm_mon=0;
                           globalvalue->date=mktime(tmp); break;
      case FUNC_STRIPDATE: tmp->tm_isdst=-1, tmp->tm_hour=tmp->tm_min=0,
                           tmp->tm_sec=0; globalvalue->date=mktime(tmp);
                           break;
   }
   return 1;
}
int sqlsearch::execprocedure(node *n)
{
   switch (n->num1)
   {
      case FUNC_DATE:  globalvalue->date=time(NULL); break;
      case FUNC_ROWNUM:  globalvalue->integer=nrecords+1; break;
   }
   return 1;
}
int sqlsearch::execident(node *n)
{
   value *v=tables[n->num1].currentr->getvalues()+n->num2;
   if (v->size==0)
   {
      globalvalue->string=NULL;
      return 0;
   }
   switch (v->type)
   {
      case ATT_VARCHAR:
      case ATT_CHAR  : globalvalue->string=v->data.string; break;
      case ATT_INT   : globalvalue->integer=*v->data.integer; break;
      case ATT_MONEY :
      case ATT_FLOAT : globalvalue->floating=*v->data.floating; break;
      case ATT_DATE  : globalvalue->date=*v->data.date; break;
      case ATT_NULL  : return 0;
   }
   return 1;
}
int sqlsearch::execaddsub(node *n)
{
   int fact=n->type==NODE_ADD?1:-1;
   globalv val1, val2;
   if (!exectree(n->n1)) return 0; val1=*globalvalue;
   if (!exectree(n->n2)) return 0; val2=*globalvalue;
   if (n->result==ATT_DATE) 
   {
      switch (n->n1->result)
      {
         case ATT_INT   : globalvalue->date=val1.integer*86400+
                             fact*val2.date; break;
         case ATT_FLOAT : globalvalue->date=(time_t)(val1.floating*86400+
                             fact*val2.date); break;
         case ATT_DATE  : globalvalue->date=(time_t)(val1.date+fact*86400*
                             ((n->n2->result==ATT_INT)?val2.integer:val2.floating));
                             break;
      }
   } else if (n->result==ATT_INT)
   {
      if (n->n1->result==ATT_DATE) globalvalue->integer=(int)
         (val1.date-val2.date)/86400; else
         globalvalue->integer=val1.integer+fact*val2.integer;
   } else if (n->result==ATT_MONEY||n->result==ATT_FLOAT) 
   {
      switch (n->n1->result)
      {
         case ATT_INT  : globalvalue->floating=val1.integer+fact*val2.floating;
                         break;
         case ATT_MONEY:
         case ATT_FLOAT: globalvalue->floating=val1.floating+fact*
                         ((n->n2->result==ATT_INT)?val2.integer:val2.floating);
      }
   }
   return 1;
}
int sqlsearch::execmuldiv(node *n)
{
   int mul=n->type==NODE_MULTIPLY;
   globalv val1, val2;
   if (!exectree(n->n1)) return 0; val1=*globalvalue;
   if (!exectree(n->n2)) return 0; val2=*globalvalue;
   if (n->result==ATT_INT)
   {
      globalvalue->integer=val1.integer*val2.integer;
   } else if (n->result==ATT_MONEY||n->result==ATT_FLOAT) 
   {
      switch (n->n1->result)
      {
         case ATT_INT:   if (mul) globalvalue->floating=val1.integer*
                            ((n->n2->result==ATT_INT)?val2.integer:val2.floating);
                         else globalvalue->floating=val1.integer/(double)
                            (((n->n2->result==ATT_INT)?val2.integer:val2.floating));
                         break;
         case ATT_MONEY:
         case ATT_FLOAT: if (mul) globalvalue->floating=val1.floating*
                            ((n->n2->result==ATT_INT)?val2.integer:val2.floating);
                         else globalvalue->floating=val1.floating/(double)
                            (((n->n2->result==ATT_INT)?val2.integer:val2.floating));
                         break;
      }
   }
   return 1;
}
int sqlsearch::execattrib(node *n)
{
   switch (n->result)
   {
      case ATT_VARCHAR:
      case ATT_CHAR  : globalvalue->string=n->s1; break;
      case ATT_INT   : globalvalue->integer=n->num2; break;
      case ATT_MONEY :
      case ATT_FLOAT : globalvalue->floating=n->fval; break;
      case ATT_DATE  : globalvalue->date=n->dval; break;
      case ATT_NULL  : globalvalue->string=NULL; return 0;
   }
   return 1;
}
int sqlsearch::execuminus(node *n)
{
   if (!exectree(n->n1)) return 0;
   if (n->result==ATT_MONEY||n->result==ATT_FLOAT)
      globalvalue->floating*=-1;
   else globalvalue->integer*=-1;
   return 1;
}
int sqlsearch::execcolfunc(node *n)
{
   int resok;
   globalv *oldv, value;
   oldv=globalvalue;
   globalvalue=&value;
   resok=exectree(n->n1);
   globalvalue=oldv;
   if (n->num1==FUNC_MIN||n->num1==FUNC_MAX)
   {
      int fact=n->num1==FUNC_MIN?1:-1;
      if (resok) switch(n->n1->result)
      {
         case ATT_CHAR:
         case ATT_VARCHAR:
            if (firstentry||((fact*strcmp(globalvalue->string,value.string))>0))
            {
               if (globalvalue->string) free(globalvalue->string);
               globalvalue->string=strdup(value.string);
            }
            break;
         case ATT_INT:
            if (firstentry||fact*globalvalue->integer>fact*value.integer)
               globalvalue->integer=value.integer; 
            break;
         case ATT_MONEY:
         case ATT_FLOAT:
            if (firstentry||fact*globalvalue->floating>fact*value.floating)
               globalvalue->floating=value.floating; 
            break;
         case ATT_DATE:
            if (firstentry||fact*globalvalue->date>fact*value.date)
               globalvalue->date=value.date; 
            break;
      }
   } else if (n->num1==FUNC_SUM)
   {
      if (resok)
      {
         if (n->result==ATT_INT) globalvalue->integer+=value.integer; else
            globalvalue->floating+=value.floating;
      }
   } else if (n->num1==FUNC_AVG)
   {
      if (resok)
      {
         if (n->n1->result==ATT_INT) globalvalue->floating+=value.integer; else
            globalvalue->floating+=value.floating;
      } else currentok=0;
   } else if (n->num1==FUNC_COUNT)
   {
      if (resok) globalvalue->integer++;
   }
   return 1;
}
int sqlsearch::exectree(node *n)
{
   switch(n->type)
   {
      case NODE_IDENT : return execident(n);
      case NODE_ATTRIBUTE: return execattrib(n);
      case NODE_SUBSTRACT :
      case NODE_ADD   : return execaddsub(n);
      case NODE_MULTIPLY:
      case NODE_DIVIDE : return execmuldiv(n);
      case NODE_UMINUS: return execuminus(n);
      case NODE_UPLUS: return exectree(n->n1);
      case NODE_COLFUNC: return execcolfunc(n);
      case NODE_FUNCTION: return execfunction(n);
      case NODE_PROCEDURE: return execprocedure(n);
      case NODE_STAR: return 1; /* for count(*) */
      default: break;
   }
   return 1;
}
int sqlsearch::lowcompare(int type, globalv *v1, globalv *v2)
{  
   int res;
   #define NUMCMP(X,Y) (X==Y?0:X<Y?-1:1)
   switch (type)
   {
      case ATT_CHAR:
      case ATT_VARCHAR: if (!v1->string) res=-1; else
                        if (!v2->string) res=1; else
                           res=strcmp(v1->string,v2->string);
                        return res<0?-1:res>0?1:0;
      case ATT_INT : return NUMCMP(v1->integer,v2->integer);
      case ATT_MONEY:
      case ATT_FLOAT: return NUMCMP(v1->floating,v2->floating);
      case ATT_DATE: return NUMCMP(v1->date,v2->date);
   }
   return 0;
}
int sqlsearch::parseequals(node *n)
{
   globalv value1, value2;
   globalvalue=&value1;
   if (!exectree(n->n1))
   {
      globalvalue=NULL;
      return -2;
   }
   globalvalue=&value2;
   if (!exectree(n->n2)) 
   {
      globalvalue=NULL;
      return -2;
   }
   globalvalue=NULL;
   int res=lowcompare(n->n1->result,&value1,&value2);
   return res<0?-1:res>0?1:0;
}
int sqlsearch::parsenull(node *n)
{
   globalv dum;
   globalvalue=&dum;
   int resok=exectree(n->n1);
   globalvalue=NULL;
   return resok;
}
int sqlsearch::parseinlist(node *n)
{
   node *temp;
   globalv value1;
   globalvalue=&value1;
   if (!exectree(n->n1))
   {
      globalvalue=NULL;
      return 0;
   }
   for (temp=n->n2;temp;temp=temp->n2)
   {
      globalv value2;
      globalvalue=&value2;
      if (!exectree(temp->n1)) continue;
      if (lowcompare(n->n1->result,&value1,&value2)==0)
      {
         globalvalue=NULL;
         return 1;
      }
   }
   globalvalue=NULL;
   return 0;
}
int sqlsearch::parsebetween(node *n)
{
   globalv v1, v2, v3;
   globalvalue=&v1;
   if (!exectree(n->n1))
   {
      globalvalue=NULL;
      return 0;
   }
   globalvalue=&v2;
   if (!exectree(n->n2->n1))
   {
      globalvalue=NULL;
      return 1;
   }
   globalvalue=&v3;
   if (!exectree(n->n2->n2))
   {
      globalvalue=NULL;
      return 0;
   }
   globalvalue=NULL;
   return (lowcompare(n->n1->result, &v1, &v2)==1)&&
     (lowcompare(n->n1->result, &v1, &v3)==-1);
}
extern "C"
{
int wildmat(char *, char *);
}
int sqlsearch::parselike(node *n)
{
   int result;
   globalv v1;
   globalvalue=&v1;
   if (!exectree(n->n1)) result=0; else
      result=wildmat(v1.string, n->s1);
   globalvalue=NULL;
   return result;
}
int sqlsearch::parseexists(node *n)
{
   subs *search=subselect+n->num1;
   sqlsearch *subsearch=search->search;
   if (search->resultready) return search->result;
   search->needmore=0;
   while (subsearch->run())
   {
      if (subsearch->recordready==1)
      {
         search->resultready=search->result=1;
         return 1;
      }
   }
   search->resultready=1;
   search->result=0;
   return 0;
   return 0;
}
int sqlsearch::parsein(node *n)
{
   subs *search=subselect+n->num1;
   if (search->resultready) return search->result;
   if (!search->valueok)
   {
      globalv *oldv=globalvalue;
      globalvalue=&search->value;
      if (!exectree(n->n1))
      {
         search->valueok=1;
         globalvalue=oldv;
         search->resultready=1;
         search->result=0;
         return 0;
      }
      search->valueok=1;
      globalvalue=oldv;
   }
   while (search->search->run())
   {
      if (search->search->recordready==1)
      {
         globalv val;
         value *v=search->search->currentrecord;
         switch (n->n1->result)
         {
            case ATT_INT : val.integer=*v->data.integer; break;
            case ATT_DATE: val.date=*v->data.date; break;
            case ATT_FLOAT:
            case ATT_MONEY: val.floating=*v->data.floating; break;
            case ATT_VARCHAR:
            case ATT_CHAR : val.string=v->data.string; break;
         }
         if (!lowcompare(n->n1->result,&search->value,&val))
         {
            search->resultready=1;
            search->result=1;
            return 1;
         }
      }
   }
   search->resultready=1;
   search->result=0;
   search->needmore=0;
   return 0;
}
int sqlsearch::parsewhere(node *n)
{
   int temp;
   if (n->result==COND_MET) return 1;
   switch (n->type)
   {
      case NODE_AND: return parsewhere(n->n1)&&parsewhere(n->n2);
      case NODE_OR : return parsewhere(n->n1)||parsewhere(n->n2);
      case NODE_NOT : return !parsewhere(n->n1);
      case NODE_EQUALS : return parseequals(n)==0;
      case NODE_NEQ : return parseequals(n)!=0;
      case NODE_LESS : return parseequals(n)==-1;
      case NODE_MORE : return parseequals(n)==1;
      case NODE_LEQ : temp=parseequals(n); return temp==-1||temp==0;
      case NODE_MOQ : temp=parseequals(n); return temp==1||temp==0;
      case NODE_NOT_NULL: return parsenull(n);
      case NODE_NULL : return !parsenull(n);
      case NODE_INLIST: return parseinlist(n);
      case NODE_IN    : return parsein(n);
      case NODE_BETWEEN: return parsebetween(n);
      case NODE_EXISTS: return parseexists(n);
      case NODE_LIKE: return parselike(n);
      default : break;
   }
   return 0;
}
int sqlsearch::getsize(value *v)
{
   switch (v->type)
   {
      case ATT_VARCHAR:
      case ATT_CHAR : return 1+strlen(v->data.string);
      case ATT_MONEY:
      case ATT_FLOAT: return sizeof(double);
      case ATT_INT: return sizeof(int);
      case ATT_DATE: return sizeof(time_t);
   }
   return 0;
}
int sqlsearch::checkok(node *n)
{
   int x;
   for (x=localtables;x<ntables;x++)
      tables[x].currentr=parent->tables[x-localtables].currentr;
   int ok=parsewhere(n);
   if (!ok) return 0;
   for (x=0;x<nsubs;x++) if (subselect[x].needmore) return 0;
   return 1;
}
void sqlsearch::activateincurrent()
{
   attentry *atp=attributes;
   for (int x=0;x<nattributes;x++, atp++)
   {
      if (atp->at) currentrecord[x]=tables[atp->tabnum].
         currentr->getvalues()[atp->tabentry]; else
      {
         globalvalue=&atp->storage;
         int resok=exectree(atp->tree);        
         if (atp->tree->type==NODE_COLFUNC) atp->dupped=1;
         if (forward)
         {
            globalvalue=NULL;
            continue;
         }
         if (atp->tree->result==ATT_CHAR||atp->tree->result==ATT_VARCHAR)
            currentrecord[x].data.string=globalvalue->string; else
            currentrecord[x].data.integer=&globalvalue->integer;
         currentrecord[x].type=atp->tree->result;
         currentrecord[x].size=resok?getsize(currentrecord+x):0;
         globalvalue=NULL;
      }
      currentrecord[x].id=x;
   }
}
void sqlsearch::activateingroup(sorter *what)
{
   int x;
   for (x=0;x<ntables;x++) what->addhandle(tables[x].ts->gethandle());
   for (x=0;x<what->getamount();x++)
   {
      globalvalue=what->addvalue(x);
      memset(globalvalue, 0, sizeof(globalv));
      exectree(what->gettree(x));        
      what->checkvalue(x);
      globalvalue=NULL;
   }
}
void sqlsearch::activate()
{
   currentok=where?checkok(where):1;
   if (!currentok||!select) return;
   if (groupby)
   {
      activateingroup(groupbys);
      currentok=0;
   } else if (orderby)
   {
      activateingroup(orderbys);
      currentok=0;
   } else activateincurrent();
}
void sqlsearch::delayedactivate()
{
   attentry *atp=attributes;
   for (int x=0;x<nattributes;x++, atp++)
   {
      globalv *v=&atp->storage;
      if (atp->tree&&atp->tree->type==NODE_COLFUNC)
      {
         if (atp->tree->num1==FUNC_AVG)
            v->floating/=(double)(nrecords?nrecords:1);
      } else
      {
         if (atp->at)
         {
            currentrecord[x]=tables[atp->tabnum].
               currentr->getvalues()[atp->tabentry];
            continue;
         }
         globalvalue=v;
         exectree(atp->tree);
         globalvalue=NULL;
      }
      currentrecord[x].type=atp->tree->result;
      currentrecord[x].id=x;
      if (nrecords==0&&(!atp->tree||!atp->tree->type==NODE_COLFUNC||
         atp->tree->num1!=FUNC_COUNT))
      {
         currentrecord[x].size=0, currentrecord[x].data.string=NULL;
         currentrecord[x].type=atp->tree->result;
      } else 
      {
         if (atp->tree->result==ATT_CHAR||atp->tree->result==ATT_VARCHAR)
            currentrecord[x].data.string=v->string; else
            currentrecord[x].data.integer=&v->integer;
         currentrecord[x].type=atp->tree->result;
         currentrecord[x].size=getsize(currentrecord+x);
      }
   }
}
int sqlsearch::nextentry()
{
   int tabnum=localtables-1, more, x, ready=0;
   for (x=0;x<nsubs;x++)
   {
      subselect[x].needmore=0, subselect[x].valueok=0;
      subselect[x].resultready=0;
      subselect[x].search->reset();
   }
   for (x=0;x<localtables;x++) if (tables[x].currentr==NULL)
   {
      tables[x].currentr=tables[x].ts->getnext();
      if (tables[x].currentr==NULL) return 0;
      ready=1;
   }
   if (ready) return 1;
   do
   {
      more=0;
      tabentry *temp=&tables[tabnum];
      if (temp->currentr) delete temp->currentr; 
      temp->currentr=temp->ts->getnext();
      if (!temp->currentr&&tabnum==0) return 0;
      if (!temp->currentr)
      {
         more=1, tabnum--;
         temp->ts->rewind();
         temp->currentr=temp->ts->getnext();
      }
   } while (more);
   return 1;
}
void sqlsearch::dropprevious()
{
   tables[0].currentr->closestream();
   tables[0].ts->destroyit();
}
void sqlsearch::doupdate()
{
   value *v=tables[0].currentr->getvalues()+attributes[1].tabentry;
   if (attributes[1].at->nulls==0&&!currentrecord[0].size) return;
   if (v->data.string) free(v->data.string);
   if (currentrecord[0].size)
   {
      int localtype=currentrecord[0].type, desired=v->type;
      if (desired==localtype||desired==ATT_VARCHAR)
      {
         v->data.string=(char*)malloc(currentrecord[0].size);
         memcpy(v->data.string,currentrecord[0].data.string,currentrecord[0].size);
         v->size=currentrecord[0].size;
      } else switch (desired)
      {
         case ATT_CHAR: v->data.string=(char*)malloc(2);
                        v->data.string[0]=currentrecord[0].data.string[0];
                        v->data.string[1]='\0';
                        v->size=2;
                        break;
         case ATT_INT:  v->data.string=(char*)malloc(sizeof(int));
                        v->size=sizeof(int);
                        if (localtype==ATT_MONEY||localtype==ATT_FLOAT)
                           *(int*)v->data.string=(int)*currentrecord[0].data.floating;
                           else *(int*)v->data.string=*currentrecord[0].data.integer;
                        break;
         case ATT_MONEY:
         case ATT_FLOAT:v->data.string=(char*)malloc(sizeof(double));
                        v->size=sizeof(double);
                        if (localtype==ATT_MONEY||localtype==ATT_FLOAT)
                           *(double*)v->data.string=(double)*currentrecord[0].data.floating;
                           else *(double*)v->data.string=(double)*currentrecord[0].data.integer;
                        break;
      }
   } else v->data.string=NULL, v->size=currentrecord[0].size;
   tables[0].tab->store(tables[0].currentr);
}
int sqlsearch::primaryrun()
{
   int needmore=0;
   if (nsubs) for (int x=0;x<nsubs;x++) if (subselect[x].needmore)
   {
      needmore=1;
      subselect[x].needmore=0;
   }
   if (needmore) return 1; else return nextentry(); 
}
int sqlsearch::orderbyrun()
{
   if (currentindex>=orderbys->nelements)
   {  
      if (!forward) return 0;
      delayed=1, amount++, recordready=1;
      currentok=1;
      delayedactivate();
      return 1;
   }
   int x, *handles=orderbys->list[currentindex++].handle;
   for (x=0;x<ntables;x++)
   {
      tables[x].ts->sethandle(handles[x]);
      if (tables[x].currentr) delete tables[x].currentr; 
      tables[x].currentr=tables[x].ts->getnext();
   }
   activateincurrent();
   if (!forward) recordready=1;
   firstentry=0;
   nrecords++;
   return 1;
}
int sqlsearch::groupbyrun()
{
   if (currentgroup>=groupbys->ngroups) return 0;
   if (groupbys->groups[currentgroup]==currentindex)
   {
      delayedactivate();
      firstentry=1; recordready=1; nrecords=0;
      amount++; currentgroup++;
      return 1;
   }
   int *handles=groupbys->list[currentindex++].handle, x;
   for (x=0;x<ntables;x++)
   {
      tables[x].ts->sethandle(handles[x]);
      if (tables[x].currentr) delete tables[x].currentr; 
      tables[x].currentr=tables[x].ts->getnext();
   }
   activateincurrent();
   firstentry=0;
   nrecords++;
   return 1;
}
int sqlsearch::run()
{
   if (delayed) return 0;
   recordready=0;
   if (firstentry) clearstorage();
   if (groupsready) return groupbyrun();
   if (orderready) return orderbyrun();
   int notready=primaryrun();
   if (notready)
   {
      activate();
      firstentry=0;
      if (!forward&&currentok) amount++, recordready=1;
      if (currentok) nrecords++;
      return 1;
   }
   if (groupby&&!groupsready)
   {
      groupbys->sort();
      if (groupbys->nelements==0) return 0;
      groupbys->setgroups();
      groupsready=1, firstentry=1, forward=1, nrecords=0;
      currentindex=0, currentgroup=0;
      return 1;
   }
   if (orderby&&!orderready)
   {
      orderbys->sort();
      if (orderbys->nelements==0) return 0;
      orderready=1, firstentry=1, nrecords=0, currentindex=0;
      return 1;
   }
   if (forward)
   {
      delayed=1, amount++, recordready=1;
      currentok=1;
      delayedactivate();
      return 1;
   }
   return 0;
}
void sqlsearch::fillts()
{
   for (int x=0;x<localtables;x++) tables[x].ts=
     new tablesearch(tables[x].tab, 0, NULL);
}
int sqlsearch::getamount()
{
   return amount;
}
record *sqlsearch::getcurrent()
{
   return tables[0].currentr;
}
/***************************************************************/
sorter::sorter(int a, int n, sqlsearch *p)
{
   groupbys=(forwardentry *)calloc(ngroupbys=a,sizeof(forwardentry));
   nhandles=maxhandles=0, handles=NULL, tables=n, parent=p, list=NULL;
   groups=NULL, ngroups=0;
}
sorter::~sorter()
{
   reset();
   free(groupbys);
}
void sorter::set(int x, int t, node *n, int sort)
{
   groupbys[x].type=t, groupbys[x].tree=n, groupbys[x].sorttype=sort;
}
void sorter::reset()
{  
   if (handles) free(handles); 
   handles=NULL;
   nhandles=maxhandles=0;
   for (int x=0;x<ngroupbys;x++)
   {
      forwardentry *thise=groupbys+x;
      if (thise->type==ATT_CHAR||thise->type==ATT_VARCHAR)
      {
         for (int y=0;y<thise->nvalues;y++) 
            if (thise->values[y].string) free(thise->values[y].string);
      }
      free(thise->values);
      thise->values=NULL;
      thise->maxvalues=thise->nvalues=0;
   }
   if (list) delete list; list=NULL;
   if (groups) delete groups; groups=NULL; ngroups=0;
}
globalv *sorter::addvalue(int x)
{
   forwardentry *thise=groupbys+x;
   if (thise->nvalues==thise->maxvalues)
   {
      if (thise->values) thise->values=(globalv*)realloc(thise->values,
         sizeof(globalv)*(thise->maxvalues+=1000)); else
      thise->values=(globalv*)malloc(sizeof(globalv)*(thise->maxvalues=1000));
   } 
   return thise->values+thise->nvalues++;
}
node *sorter::gettree(int x)
{
   return groupbys[x].tree;
}
void sorter::checkvalue(int x)
{
   forwardentry *thise=groupbys+x;
   if (thise->type!=ATT_CHAR&&thise->type!=ATT_VARCHAR) return;
   globalv *val=thise->values+thise->nvalues-1;
   if (val->string) val->string=strdup(val->string);
}
int sorter::getamount()
{
   return ngroupbys;
}
void sorter::addhandle(int x)
{
   if (nhandles==maxhandles)
   { 
      if (handles) handles=(int*)realloc(handles,sizeof(int)*(maxhandles+=1000));
         else handles=(int*)malloc(sizeof(int)*(maxhandles=1000));
   }
   handles[nhandles++]=x;
}
sqlsearch *sqlsp;
sorter *sortp;
int compare(const void *p1, const void *p2)
{
   int n1=((sortstruct *)p1)->index;
   int n2=((sortstruct *)p2)->index;
   int num=0, res;
   do
   {
      int type=sortp->groupbys[num].type;
      globalv *v1=sortp->groupbys[num].values+n1;
      globalv *v2=sortp->groupbys[num].values+n2;
      res=sqlsp->lowcompare(type, v1, v2);
      if (res==0) if (num==sortp->ngroupbys-1) return 0; else num++;
   } while (res==0);
   return res*sortp->groupbys[num].sorttype;
}
void sorter::setgroups()
{
   if (!list) return;
   int prev=list[0].index;
   groups=(int*)malloc(sizeof(int)*nelements);
   for (int x=0;x<nelements;x++)
   {
      int num=0, res;
      int index=list[x].index;
      if (x) do
      {
         int type=groupbys[num].type;
         globalv *v1=groupbys[num].values+index;
         globalv *v2=groupbys[num].values+prev;
         res=parent->lowcompare(type, v1, v2);
         if (res==0) if (num==ngroupbys-1) break; else num++;
      } while (res==0);
      if (x&&res!=0) groups[ngroups++]=x, prev=index;
   }
   groups[ngroups++]=nelements;
   groups=(int*)realloc(groups,sizeof(int)*ngroups);
}
void sorter::sort()
{
   int *intp=handles;
   nelements=groupbys[0].nvalues;
   if (!nelements) return;
   list=new sortstruct[nelements];
   for (int x=0;x<nelements;x++, intp+=tables)
      list[x].index=x, list[x].handle=intp;  
   sqlsp=parent, sortp=this;  
   qsort(list, nelements, sizeof(sortstruct), compare);
}
