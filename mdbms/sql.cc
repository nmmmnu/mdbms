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
#include "attributes.h"
#include "global.hh"
#include "lexyacc.h"
#include "sql.hh"
#include "support.h"
#include "sqlsearch.hh"
#include "relation.hh"
#include "dbms.hh"
sqltree::sqltree(char *code)
{
   ::parse(code);
   tree=rootnode;
   rootnode=NULL;
   nerrors=::nerrors;
   messages=NULL;
   if (nerrors)
   {
      err=(error*)malloc(sizeof(error)*nerrors);
      for (int x=0;x<nerrors;x++) err[x].string=errorstrings[x],
         err[x].num=ERR_PARSEERROR;
   } else err=NULL;
}
sqltree::~sqltree()
{
   if (tree) delete tree;
   for (int x=0;x<nerrors;x++) if (err[x].string) free(err[x].string);
   if (err) free(err);
   if (messages) free(messages);
}
int sqltree::getnerrors()
{
   return nerrors;
}
char *sqltree::getserror(int x)
{
   static char buf[100];
   if (x>=nerrors) return NULL;
   seterror(err[x].num, err[x].string);
   return serror(buf);
}
error *sqltree::geterror(int x)
{
   if (x>=nerrors) return NULL;
   return err+x;
}
char *sqltree::getmessages()
{
   return messages;
}
void sqltree::adderror()
{
   nerrors++;
   if (err) err=(error*)realloc(err,sizeof(error)*nerrors); else
      err=(error*)malloc(sizeof(error));
   err[nerrors-1].num=dberrno;
   err[nerrors-1].string=dbcontext?strdup(dbcontext):(char*)NULL;
}
void sqltree::addmessage(char *s)
{
   if (!messages)
   {
      messages=(char*)malloc(10000);
      messages[0]='\0';
   }
   strcat(messages,s);
}
/********************************************************************/
transaction::transaction(dbms *d, database **db, char **u, absuserext *p)
{
   dbm=d->getdbman(), currentdb=db, user=u, thistree=NULL, currentsearch=NULL;
   busy=0, dbmsp=d, currentref=NULL, amount=0, parent=p;
   setopstat();
}
transaction::~transaction()
{
   if (thistree) delete thistree;
   if (currentsearch) delete currentsearch;
   if (currentref) delete currentref;
}
void transaction::setopstat()
{
   opstat=*user&&!strcmp(*user,"system");
}
int transaction::nocurrentdb()
{
   if (*currentdb) return 0;
   seterror(ERR_NODB, NULL);
   thistree->adderror();
   return 1;
}
void transaction::execute(sqltree *tree)
{
   thistree=tree;
   start(tree->tree);
}
void transaction::start(node *n)
{
   if (n->n2) start(n->n2);
   rootnode(n->n1);
}
void transaction::rootnode(node *n)
{
   switch (n->type)
   {
      case NODE_CREATE : nodecreate(n->n1); break;
      case OPEN_USER   : nodeuser(n->n1); break;
      case NODE_DROP   : nodedrop(n->n1); break;
      case NODE_UPDATE : startsearch(n, GRANTUPDATE); break;
      case NODE_SELECT : startsearch(n, GRANTSELECT); break;
      case NODE_DELETE : nodedelete(n); break;
      case NODE_OPEN   : nodeopen(n->n1); break;
      case NODE_SYNC   : dbmsp->sync(); break;
      case NODE_INSERT : nodeinsert(n); break;
      case NODE_HELP   : nodehelp(n->s1); break;
      case ROOT_SET    : nodeset(n->n1->s1,n->n1->s2); break;
      case NODE_GRANT  : nodegr(n->n1,1); break;
      case NODE_REVOKE : nodegr(n->n1,2); break;
      default : break;
   }
}
void transaction::nodegr(node *n, int type)
{
   if (nocurrentdb()) return;
   node *temp;
   table *t;
   for (temp=n->n1;temp;temp=temp->n2)
   {
      t=(*currentdb)->gettable(temp->s1);
      if (!t)
      {
         seterror(ERR_NOSUCHTBL, temp->s1);
         thistree->adderror();
         return;
      }
      if (!t->isgranted(*user,n->num1<<5))
      {
         seterror(ERR_PERMISSION, temp->s1);
         thistree->adderror();
         return;
      }
   }
   if (n->num2) n->num1|=(n->num1<<5);
   for (temp=n->n1;temp;temp=temp->n2)
   {
      t=(*currentdb)->gettable(temp->s1);
      node *users;
      for (users=n->n2;users;users=users->n2)
         dbm->grantrevoke(*currentdb,t,users->s1,n->num1,1,type);
   }
}
void transaction::nodeset(char *name, char *data)
{
   if (!strcmp(name,"pwd"))
   {
      if (*user) dbm->setuserpassword(*user,data); else
      {
         seterror(ERR_NOUSER,"set password");
         thistree->adderror();
      }
   }
}
void transaction::nodedelete(node *n)
{
   if (!startsearch(n,GRANTDELETE)) return;
   currentref=new refdelete(*currentdb,n->n2->n1->n1->s1,currentsearch,dbmsp,
      user, parent);
   if (currentref->getnentrys()==0)
   {
      delete currentref;
      currentref=NULL;
   }
}
void transaction::nodedrop(node *n)
{
   switch (n->type)
   {
      case NODE_IDENT : dodroptable(n); break;
      case NODE_DROPDB: dodropdb(n); break;
   }
}
void transaction::dodropdb(node *n)
{
   database *db=dbm->opendatabase(n->s1);
   if (!db)
   {
      seterror(ERR_NOSUCHDB, n->s1);
      thistree->adderror();
      return;
   }
   if (!opstat)
   {
      seterror(ERR_PERMISSION, "destroydb");
      thistree->adderror();
      return;
   }
   if (dbmsp->dbinuse(db,parent))
   {
      seterror(ERR_DBINUSE, "destroydb");
      thistree->adderror();
      return;
   }
   if (dbm->destroydatabase(db, n->num2, *user)==0)
   {
      thistree->adderror();
      return;
   }
   if (*currentdb==db) *currentdb=NULL;
}
void transaction::dodroptable(node *n)
{
   if (nocurrentdb()) return;
   table *t=(*currentdb)->gettable(n->s1);
   if (!t)
   {
      seterror(ERR_NOSUCHTBL, n->s1);
      thistree->adderror();
   } else
   {
      if (!t->isgranted(*user,GRANTDROP))
      {
         seterror(ERR_PERMISSION,"drop");
         thistree->adderror();
      } else if (!dbm->destroytable(*currentdb,t,n->num2))
         thistree->adderror();
   }
}
void transaction::nodecreate(node *n)
{
   switch (n->type)
   {
      case NODE_CREATEDB  : docreatedb(n->n1); break;
      case NODE_CREATETBL : docreatetbl(n->n1); break;
      case NODE_CREATEVIEW: docreateview(n); break;
      case NODE_CREATEUSER: docreateuser(n); break;
      default : break;
   }
}
void transaction::docreateuser(node *n)
{
   if (!opstat)
   {
      seterror(ERR_PERMISSION,"adduser");
      thistree->adderror();
      return;
   }
   if (!dbm->adduser(n->s1,n->s2))
      thistree->adderror();
}
void transaction::docreatedb(node *n)
{
   char name[]="system";
   if (!opstat)
   {
      seterror(ERR_PERMISSION,"createdb");
      thistree->adderror();
      return;
   }
   if (!dbm->createdatabase(n->s1, n->s2?n->s2:name))
      thistree->adderror();
}
int transaction::checkprimary(node *n)
{
   node *keys=n->n2;
   if (!keys->n1) return 1;
   for (node *temp=keys->n1;temp;temp=temp->n2)
   {
      int ok=0;
      for (node *t2=n->n1;t2&&!ok;t2=t2->n2) if (!strcmp(temp->s1,t2->n1->s1))
         ok=1, t2->num1=1;
      if (!ok)
      {
         seterror(ERR_NOSUCHATTR, temp->s1);
         thistree->adderror();
         return 0;
      }
   }
   return 1;
}
int transaction::checkforeignkey(table *t, node *fkeys, node *root)
{
   int hasprimary=0;
   if (t)
   {
      /* Referenced table is different table */
      attribute *at;
      int x=0;
      node *thisat=fkeys->n1->n1;
      while ((at=t->getattribute(x++))!=NULL)
      {
         if (!thisat)
         {
            seterror(ERR_MOREATTNEEDED,NULL);
            thistree->adderror();
            return 0;
         }
         if (!at->primary) continue; /* not primary key->continue */
         hasprimary=1;
         if (thisat->num1!=at->type)
         {
            seterror(ERR_TYPES,thisat->s1);
            thistree->adderror();
            return 0;
         }
         thisat=thisat->n2;
         if (!thisat) break;
      }
      if (thisat)
      {
         if (hasprimary) seterror(ERR_TOOMANYATT,NULL); else
            seterror(ERR_CANTREFERE,t->getname());
         thistree->adderror();
         return 0;
      }
   } else
   {
      /* Referenced table is our own table */
      node *at=root->n1;
      node *thisat=fkeys;
      while (at)
      {
         if (!thisat)
         {
            seterror(ERR_MOREATTNEEDED,NULL);
            thistree->adderror();
            return 0;
         }
         if (!at->num1) continue; /* not primary key->continue */
         hasprimary=1;
         if (thisat->n1->n1->num1!=at->n1->n1->num1)
         {
            seterror(ERR_TYPES,fkeys->s1);
            thistree->adderror();
            return 0;
         }
         thisat=thisat->n2;
         at=at->n2;
         if (!thisat) break;
      }
      if (thisat)
      {
         if (hasprimary) seterror(ERR_TOOMANYATT,NULL); else
            seterror(ERR_CANTREFERE,t->getname());
         thistree->adderror();
         return 0;
      }
   }
   return 1;
}
int transaction::checkforeigns(node *n)
{
   int fknum=0;
   node *keys=n->n2;
   if (!keys->n2) return 1;
   for (node *temp=keys->n2;temp;temp=temp->n2, fknum++)
   {
      node *fk=temp->n1;
      table *t=(*currentdb)->gettable(fk->s1);
      if (!t&&strcmp(fk->s1,n->s1)) /* t==NULL -> table is our own table */
      {
         seterror(ERR_NOSUCHTBL, fk->s1);
         thistree->adderror();
         return 0;
      }
      for (node *t1=temp->n1->n1;t1;t1=t1->n2)
      {
         int ok=0;
         for (node *t2=n->n1;t2&&!ok;t2=t2->n2) if (!strcmp(t1->s1,t2->n1->s1))
         {
            if (t2->num2!=-1)
            {
               seterror(ERR_ISFOREIGN, t1->s1);
               thistree->adderror();
               return 0;
            }
            if (fk->n2&&fk->n2->num1==DELRULE_SETNULL&&((t2->num1==1)||
               t2->n1->num1==0))
            {
               seterror(ERR_CANTSETNULL, t1->s1);
               thistree->adderror();
               return 0;
            }
            ok=1, t2->num2=fknum, t1->num1=t2->n1->n1->num1;
         }
         if (!ok)
         {
            seterror(ERR_NOSUCHATTR, t1->s1);
            thistree->adderror();
            return 0;
         }
      }
      if (!checkforeignkey(t,temp,n)) return 0;
   }
   return 1;
}
void transaction::docreatetbl(node *n)
{
   if (nocurrentdb()) return;
   if (!opstat) if (!*user||strcmp(*user,(*currentdb)->getauth()))
   {
      seterror(ERR_PERMISSION,"createtable");
      thistree->adderror();
      return;
   }
   if (!checkprimary(n)) return;
   if (!checkforeigns(n)) return;
   attribute at;
   node *temp;
   if (strlen(n->s1)>=39) n->s1[39]='\0';
   table *newtable=dbm->createtable(*currentdb, TBL_NORMAL, n->s1);
   if (!newtable)
   {
      thistree->adderror();
      return;
   }
   for (temp=n->n1;temp;temp=temp->n2)
   {
      node *atnode=temp->n1;
      strncpy(at.name,atnode->s1,39);
      at.primary=temp->num1;
      at.nulls=at.primary?0:atnode->num1;
      at.type=atnode->n1->num1;
      at.size=atnode->n1->num2;
      at.foreignkey=temp->num2;
      newtable->addattribute(&at);
   }
   for (temp=n->n2->n2;temp;temp=temp->n2)
   {
      node *fk=temp->n1;
      newtable->addfkey(fk->s1, fk->n2?fk->n2->num1:DELRULE_RESTRICT);
   }
}
void transaction::nodeopen(node *n)
{
   database *db=dbm->opendatabase(n->s1);
   if (db) *currentdb=db; else
   {
      seterror(ERR_NOSUCHDB, n->s1);
      thistree->adderror();
   }
}
void transaction::nodeuser(node *n)
{
   int stat=dbm->verifyuser(n->s1,n->s2);
   if (stat==-1||(!opstat&&stat==1))
   {
      seterror(ERR_INVALIDPWD, "setuser");
      thistree->adderror();
      return;
   }
   if (*user) free(*user);
   *user=strdup(n->s1);
   setopstat();
}
void transaction::nodeinsert(node *n)
{
   int amount=0, hasprimary=0;
   tablesearch *search=NULL;
   if (nocurrentdb()) return;
   table *t=(*currentdb)->gettable(n->s1);
   if (!t)
   {
      seterror(ERR_NOSUCHTBL, n->s1);
      thistree->adderror();
      return;
   }
   if (!t->isgranted(*user,GRANTINSERT))
   {
      seterror(ERR_PERMISSION, "insert");
      thistree->adderror();
      return;
   }
   record *newrecord=new record(NULL, dbm->getdbunit());
   node *temp=n->n1;
   while (temp)
   {
      int ok=checkandadd(temp->n1, newrecord, t);
      if (!ok)
      {
         thistree->adderror();
         delete newrecord;
         return;
      }
      if (ok==2)
      {
         if (!search) search=new tablesearch(t, 0, NULL);
         search->addvalue(newrecord->getvalues()+newrecord->getnvalues()-1);
         hasprimary=1;
      }
      temp=temp->n2;
      amount++;
   }
   if (search)
   {
      record *temprec;
      if ((temprec=search->getnext())!=NULL)
      {
         delete temprec;
         seterror(ERR_DUPPRIMARY,NULL);
         thistree->adderror();
         delete search;
         return;
      }
   }
   if (search) delete search;
   if (amount<t->getnattributes())
   {
      seterror(ERR_MOREVALUESNEEDED, NULL);
      thistree->adderror();
      return;
   } 
   thistree->addmessage("1 row(s) inserted.\n");
   t->store(newrecord);
   delete(newrecord);
}
int transaction::checkandadd(node *att, record *newr, table *t)
{
   int next=newr->getnvalues();
   attribute *a=t->getattribute(next);
   if (!a)
   {
      seterror(ERR_TOOMANYVAL, NULL);
      return 0;
   }
   if (att->num1==ATT_CHAR&&a->type==ATT_VARCHAR) att->num1=ATT_VARCHAR;
   if (att->num1==ATT_INT&&a->type==ATT_FLOAT)
   {
      att->num1=ATT_FLOAT;
      att->fval=(double)att->num2;
   }
   if (a->type!=att->num1&&att->num1!=ATT_NULL)
   {
      char s[100];
      sprintf(s,"Attribute %s: expected:%s, got:%s", a->name, attnames[a->type],
         attnames[att->num1]);
      seterror(ERR_COMPATIBLEINSERT,s);
      return 0;
   }
   if (att->num1==ATT_NULL&&(a->nulls==0||a->primary==1))
   {
      seterror(ERR_NULLNOTALLOWED, a->name);
      return 0;
   }
   value v;
   v.id=next, v.type=a->type;
   if (att->num1==ATT_NULL)
      v.size=0, v.data.string=NULL; else switch (a->type)
   {
      case ATT_INT    : v.data.integer=&att->num2;
                        v.size=sizeof(int); 
                        break;
      case ATT_MONEY  :
      case ATT_FLOAT  : v.data.floating=&att->fval;
                        v.size=sizeof(double); 
                        break;
      case ATT_VARCHAR: v.data.string=att->s1;
                        v.size=strlen(att->s1)+1;
                        break;
      case ATT_CHAR:    v.data.string=att->s1;
                        v.size=strlen(att->s1)+1;
                        break;
      case ATT_DATE:    v.data.date=&att->dval;
                        v.size=sizeof(time_t);
                        break;
      default:		seterror(ERR_NOTSUPPORTED, NULL); 
                        return 0;
   }
   newr->addvalue(&v,1);
   return a->primary?2:1;
}
void transaction::nodehelp(char *s)
{
   table *t=NULL;
   char key[1000]="";
   if (*currentdb) t=(*currentdb)->gettable(s);
   if (!t)
   {
      seterror(ERR_NOSUCHTBL,s);
      thistree->adderror();
      return;
   }
   int x=0;
   attribute *at;
   char buf[100];
   sprintf(buf,"Attributes of table \"%s\":\n", s);
   thistree->addmessage(buf);
   while ((at=t->getattribute(x))!=NULL)
   {
      char add[100]="";
      if (at->type==ATT_VARCHAR) sprintf(add,"(%d)",at->size);
      sprintf(buf, "   %02d: Name: \"%-10s\" Type: %s%s %s\n",x,at->name,
         attnames[at->type], add, at->nulls?"NULL":"NOT NULL");
      thistree->addmessage(buf);
      if (at->primary) 
      {
         if (key[0]!='\0') strcat(key,", ");
         strcat(key,at->name);
      }
      x++;
   }
   sprintf(buf,"Primary key: %s\n",key[0]?key:"none");
   thistree->addmessage(buf);
   for (x=0;x<t->getnkeys();x++)
   {
      int y=0;
      key[0]='\0';
      while ((at=t->getattribute(y++))!=NULL) if (at->foreignkey==x)
      {
         if (key[0]!='\0') strcat(key,", ");
         strcat(key, at->name);
      }
      sprintf(buf,"Foreign key %s references %s\n", key, t->getfkey(x));
      thistree->addmessage(buf);
      sprintf(buf,"   on delete %s\n",t->getaction(x)==DELRULE_RESTRICT?
         "restrict":t->getaction(x)==DELRULE_CASCADE?"cascade":"set null");
      thistree->addmessage(buf);
   }
   thistree->addmessage("Security:\n");
   for (x=0;x<10;x++)
   {
      grantstruct *temp;
      key[0]='\0';
      for (temp=t->getrootgrant();temp;temp=temp->next) if (temp->type&(1<<x))  
      {
         if (key[0]) strcat(key,", ");
         strcat(key,temp->name);
      }
      if (!key[0]) continue;
      sprintf(buf,"   %s granted to: %s \n",grantnames[x],key);
      thistree->addmessage(buf);
   }
}
int transaction::startsearch(node *n, int permissions)
{
   if (currentsearch)
   {
      seterror(ERR_SEARCHPENDING,NULL);
      thistree->adderror();
      return 0;
   }
   if (nocurrentdb()) return 0;
   currentsearch=new sqlsearch(dbm, *currentdb, n, NULL, permissions, *user);
   if (!currentsearch->success)
   {
      thistree->adderror();
      delete currentsearch;
      currentsearch=NULL;
      return 0;
   }
   busy=1;
   type=n->type;
   return 1;
}
sqlsearch *transaction::getsearch()
{
   return currentsearch;
}
void transaction::docreateview(node *n)
{
   if (!startsearch(n->n1,GRANTSELECT)) return;
   if (!opstat) if (!*user||strcmp(*user,(*currentdb)->getauth()))
   {
      seterror(ERR_PERMISSION,"createtable");
      thistree->adderror();
      return;
   }
   table *newtable=dbm->createtable(*currentdb, TBL_VIEW, n->s1);
   if (!newtable)
   {
      delete currentsearch;
      currentsearch=NULL;
      busy=0;
      thistree->adderror();
      return;
   }
   for (int x=0;x<currentsearch->nattributes;x++)
   {
      attribute at;
      attentry *ae=currentsearch->attributes+x;
      strcpy(at.name,ae->name);
      at.primary=0; at.nulls=0;
      at.type=ae->type;
      at.size=0;
      newtable->addattribute(&at);
   }
   currenttable=newtable;
   type=NODE_CREATEVIEW;
}
int transaction::run()
{
   if (!currentsearch) return 0;
   if (!busy&&currentsearch)
   {
      delete currentsearch;
      return 0;
   }
   if (!currentsearch->run()) return busy=0;
   switch (type)
   {
      case NODE_DELETE:
            if (currentsearch->recordready)
            {
               int ok=currentref?currentref->checkrestrict():1;
               if (ok)
               {
                  if (currentref) currentref->run();
                  currentsearch->dropprevious();
                  amount++;
               }
            }
            recordready=0;
            return 1;
      case NODE_CREATEVIEW:
            if (currentsearch->recordready)
            {
               record *newrecord=new record(NULL, dbm->getdbunit());
               for (int x=0;x<currentsearch->nattributes;x++)
                  newrecord->addvalue(currentsearch->currentrecord+x,1);
               currenttable->store(newrecord);
               delete newrecord;
               amount++;
            }
            recordready=0;
            return 1;
      case NODE_SELECT:
            recordready=currentsearch->recordready;
            if (recordready) amount++;
            return 1;
      case NODE_UPDATE:
            if (currentsearch->recordready)
            {
               currentsearch->doupdate();
               amount++;
            }
            recordready=0;
            return 1;
   }
   recordready=currentsearch->recordready;
   return 1;
}
/*********************************************************************/
refdelete::refdelete(database *_cdb, char *name, sqlsearch *cs, dbms *_dbms,
   char **_currentuser, absuserext *p)
{
   nentrys=0, entrys=NULL, currenttrans=NULL, currentsearch=cs, cdb=_cdb;
   cdbms=_dbms, currentuser=_currentuser, nkeys=0, indexes=NULL, parent=p;
   int max=cdb->getntables();
   for (int x=0;x<max;x++)
   {
      table *t=cdb->gettable(x);
      if (!t) continue;
      if (!strcmp(t->getname(),name))
      {
         attribute *at;
         int max=t->getnattributes();
         for (int c=0;c<max;c++)
         {
            at=t->getattribute(c); 
            if (!at) continue;
            if (at->primary)
            {
               nkeys++;
               if (indexes) indexes=(int*)realloc(indexes,sizeof(int)*nkeys);
                  else indexes=(int*)malloc(sizeof(int));
               indexes[nkeys-1]=c;
            }
         }
      }
      for (int y=0;y<t->getnkeys();y++) if (!strcmp(name, t->getfkey(y)))
      {
         int temp=0;
         attribute *at;
         nentrys++;
         if (entrys) entrys=(refentry*)realloc(entrys,sizeof(refentry)*nentrys);
            else entrys=(refentry*)malloc(sizeof(refentry));
         refentry *thise=entrys+nentrys-1;
         thise->tabname=t->getname(), thise->key=y, thise->nkeys=0;
         thise->action=t->getaction(y), thise->names=NULL;
         while ((at=t->getattribute(temp++))!=NULL) if (at->foreignkey==y)
         {
            thise->nkeys++;
            if (thise->names)
               thise->names=(char**)realloc(thise->names,sizeof(char*)*
               thise->nkeys);
            else thise->names=(char**)malloc(sizeof(char*));
            thise->names[thise->nkeys-1]=at->name;
         }
      }
   }
}
refdelete::~refdelete()
{
   for (int x=0;x<nentrys;x++)
      if (entrys[x].names) free(entrys[x].names);
   if (entrys) free(entrys);
   if (indexes) free(indexes);
}
int refdelete::getnentrys()
{
   return nentrys;
}
int refdelete::checkrestrict()
{
   sqltree *currenttree;
   transaction *currenttrans;
   record *r=currentsearch->getcurrent();
   value *v=r->getvalues();
   for (int x=0;x<nentrys;x++) if (entrys[x].action==DELRULE_RESTRICT)
   {
      int ok=1;
      refentry *thise=entrys+x;
      char buf[1000], where[1000]="";
      for (int y=0;y<thise->nkeys;y++)
      {
         char tempbuf[100];
         char *svalue=r->sprintvalue(indexes[y]);
         if (where[0]) strcat(where," and ");
         sprintf(tempbuf,"%s=%s",thise->names[y],svalue);
         free(svalue);
         strcat(where,tempbuf);
      }
      sprintf(buf,"select * from %s where %s;",thise->tabname,where);
      currenttree=new sqltree(buf);
      currenttrans=new transaction(cdbms,&cdb,currentuser,parent);
      currenttrans->execute(currenttree);
      while (currenttrans->busy&&ok)
      {
         currenttrans->run();
         if (currenttrans->recordready) ok=0;
      }
      delete currenttrans;
      if (!ok) return 0;
   }
   return 1;
}
void refdelete::run()
{
   sqltree *currenttree;
   transaction *currenttrans;
   record *r=currentsearch->getcurrent();
   value *v=r->getvalues();
   for (int x=0;x<nentrys;x++)
   {
      refentry *thise=entrys+x;
      if (thise->action==DELRULE_CASCADE)
      {
         char buf[1000], where[1000]="";
         for (int y=0;y<thise->nkeys;y++)
         {
            char tempbuf[100];
            char *svalue=r->sprintvalue(indexes[y]);
            if (where[0]) strcat(where," and ");
            sprintf(tempbuf,"%s=%s",thise->names[y],svalue);
            free(svalue);
            strcat(where,tempbuf);
         }
         sprintf(buf,"delete from %s where %s;",thise->tabname,where);
         currenttree=new sqltree(buf);
         currenttrans=new transaction(cdbms,&cdb,currentuser,parent);
         currenttrans->execute(currenttree);
         while (currenttrans->busy) currenttrans->run();
         delete currenttrans;
      } else if (thise->action==DELRULE_SETNULL)
      {
         int y;
         char buf[1000], where[1000]="";
         for (y=0;y<thise->nkeys;y++)
         {
            char tempbuf[100];
            char *svalue=r->sprintvalue(indexes[y]);
            if (where[0]) strcat(where," and ");
            sprintf(tempbuf,"%s=%s",thise->names[y],svalue);
            free(svalue);
            strcat(where,tempbuf);
         }
         for (y=0;y<thise->nkeys;y++)
         {
            sprintf(buf,"update %s set %s=NULL where %s;",
               thise->tabname,thise->names[y],where);
            currenttree=new sqltree(buf);
            currenttrans=new transaction(cdbms,&cdb,currentuser,parent);
            currenttrans->execute(currenttree);
            while (currenttrans->busy) currenttrans->run();
            delete currenttrans;
         }
      }
   }
}
