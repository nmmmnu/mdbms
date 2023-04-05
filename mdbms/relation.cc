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
#include "global.hh"
#include "relation.hh"
#include "support.h"

dbmanager::dbmanager(int cachemem)
{
   table *dbtable, *tablestable, *grantstable;
   unitman=new dbunit(cachemem);
   deploysystem();
   dbtable=systemdb->gettable("databases");
   tablestable=systemdb->gettable("tables");
   grantstable=systemdb->gettable("grants");
   tablesearch dbsearcher(dbtable, 0, NULL);
   record *entry;
   while ((entry=dbsearcher.getnext())!=NULL)
   {
      int type=*(int*)entry->getvalue(1);
      if (type==DB_NORMAL)
      {
         char *dbname=(char*)entry->getvalue(0);
         record *tentry;
         value testvalue={ATT_VARCHAR, 0};
         testvalue.size=1+strlen(dbname);
         testvalue.data.string=dbname;
         database *ndb=newdatabase(dbname,(char*)entry->getvalue(2));
         tablesearch tablesearcher(tablestable, 1, &testvalue);
         while ((tentry=tablesearcher.getnext())!=NULL)
         {
            char *name=(char*)tentry->getvalue(1);
            int *page=(int*)tentry->getvalue(2);
            stream *metastream=unitman->openstream(*page);
            table *t=ndb->newtable(name,TBL_NORMAL,metastream,NULL);
            t->initgrants(dbname,grantstable);
            delete tentry;
         }
      }
      delete entry;
   }
}
dbmanager::~dbmanager()
{
   int x;
   for (x=0;x<adatabases;x++) delete databases[x];
   delete databases;
   delete unitman;
}
void dbmanager::deploysystem()
{
   adatabases=1;
   databases=(database**)malloc(sizeof(database*)*adatabases);
   databases[0]=systemdb=new database("system", "system", unitman);
   if (systemdb->deploysystem()==0)
   {
      dberror("system deploy");
      exit(1);
   }
   table *dbt=systemdb->gettable("databases");
   table *tbltbl=systemdb->gettable("tables");
   table *stt=systemdb->gettable("storage");
   table *ust=systemdb->gettable("users");
   table *grt=systemdb->gettable("grants");
   if (dbt->getnrecords()==0)
      adddatabaseentry("system","system",DB_SYSTEM);
   if (tbltbl->getnrecords()==0)
   {
      addtableentry("system","databases",-1);
      addtableentry("system","tables",-1);
      addtableentry("system","users",-1);
      addtableentry("system","storage",-1);
      addtableentry("system","grants",-1);
   }
   if (stt->getnrecords()==0)
      addstorageentry(SYSSTORAGENAME,SYSSTORAGEPS);
   if (verifyuser("system","")==-1)
      adduserentry("system","");
   int syncgrants=grt->getnrecords()==0;
   grantrevoke(systemdb,dbt,"all",GRANTSELECT,syncgrants,1);
   grantrevoke(systemdb,tbltbl,"all",GRANTSELECT,syncgrants,1);
   grantrevoke(systemdb,ust,"system",GRANTSELECT|GRANTGSELECT|GRANTGDELETE|GRANTDELETE,syncgrants,1);
   grantrevoke(systemdb,stt,"all",GRANTSELECT,syncgrants,1);
   grantrevoke(systemdb,grt,"all",GRANTSELECT,syncgrants,1);
   dbt->initgrants("system",grt);
   tbltbl->initgrants("system",grt);
   stt->initgrants("system",grt);
   ust->initgrants("system",grt);
   grt->initgrants("system",grt);
}
database *dbmanager::newdatabase(char *name, char *auth)
{
   int x,ok=0;
   database *newdb=new database(name, auth, unitman);
   for (x=0;x<adatabases;x++) if (databases[x]==NULL)
   {
      databases[x]=newdb,ok=1;
      break;
   }
   if (!ok)
   {
      if (adatabases==0) databases=(database**)malloc(sizeof(database*)*
         (adatabases=1)); else
      databases=(database**)realloc(databases,++adatabases*sizeof(database*));
      databases[adatabases-1]=newdb;
   }
   return newdb;
}
database *dbmanager::createdatabase(char *name, char *auth)
{
   char dbname[129];
   strncpy(dbname, name, 128);
   dbname[128]='\0';
   if (opendatabase(dbname))
   {
      seterror(ERR_DBEXISTS, dbname);
      return NULL;
   }
   adddatabaseentry(dbname,auth,DB_NORMAL);
   return newdatabase(dbname,auth);
}
database *dbmanager::opendatabase(char *name)
{
   int x;
   for (x=0;x<adatabases;x++) if (!strcasecmp(name,databases[x]->name))
      return databases[x];
   return NULL;
}
int dbmanager::createstorage(char *name, int pagesize)
{
   int result=unitman->createfile(name,pagesize);
   if (!result) return result;
   addstorageentry(name,pagesize);
   return result;
}
void dbmanager::adddatabaseentry(char *name, char *auth, int type)
{
   value newv;
   record newr(NULL, unitman);
   newv.type=ATT_VARCHAR, newv.id=0, newv.data.string=name;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_INT, newv.id=1, newv.data.integer=&type;
   newv.size=sizeof(type);
   newr.addvalue(&newv,1);
   newv.type=ATT_VARCHAR, newv.id=2, newv.data.string=auth;
   newv.size=auth?(strlen(newv.data.string)+1):0;
   newr.addvalue(&newv,1);
   systemdb->gettable("databases")->store(&newr);
}
void dbmanager::addtableentry(char *db, char *name, int pnum)
{
   value newv;
   record newr(NULL, unitman);
   newv.type=ATT_VARCHAR, newv.id=0, newv.data.string=db;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_VARCHAR, newv.id=1, newv.data.string=name;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_INT, newv.id=2, newv.data.integer=&pnum;
   newv.size=sizeof(int);
   newr.addvalue(&newv,1);
   systemdb->gettable("tables")->store(&newr);
}
void dbmanager::addstorageentry(char *name, int pagesize)
{
   value newv;
   record newr(NULL, unitman);
   newv.type=ATT_VARCHAR, newv.id=0, newv.data.string=name;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_INT, newv.id=1, newv.data.integer=&pagesize;
   newv.size=sizeof(pagesize);
   newr.addvalue(&newv,1);
   systemdb->gettable("storage")->store(&newr);
}
void dbmanager::addgrantentry(char *db, char *tbl, char *user, int type)
{
   record newr(NULL, unitman);
   value newv;
   newv.type=ATT_VARCHAR, newv.id=0, newv.data.string=db;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_VARCHAR, newv.id=1, newv.data.string=tbl;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_VARCHAR, newv.id=2, newv.data.string=user;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_INT, newv.id=3, newv.data.integer=&type;
   newv.size=sizeof(int);
   newr.addvalue(&newv,1);
   systemdb->gettable("grants")->store(&newr);
}
void dbmanager::grantrevoke(database *db, table *t, char *who, int type,
   int sync, int gr /*1=grant 2=revoke*/)
{
   grantstruct *temp;
   if (gr==1) t->grant(who,type); else t->revoke(who,type);
   if (sync)
   {
      record *tentry;
      value testvalue[2]={{ATT_VARCHAR, 0},{ATT_VARCHAR,1}};
      testvalue[0].size=1+strlen(db->name);
      testvalue[0].data.string=db->name;
      testvalue[1].size=1+strlen(t->name);
      testvalue[1].data.string=t->name;
      tablesearch tablesearcher(systemdb->gettable("grants"), 2, testvalue);
      while ((tentry=tablesearcher.getnext())!=NULL)
      {
         delete tentry;
         tablesearcher.destroyit();
      } 
      for (temp=t->rootgrant;temp;temp=temp->next)
         addgrantentry(db->name, t->name, temp->name, temp->type);
   }
}
void dbmanager::adduserentry(char *name, char *password)
{
   char pw[100]="";
   if (password&&password[0])
   {
      strcpy(pw,password);
      docrypt(pw);
   }
   value newv;
   record newr(NULL, unitman);
   newv.type=ATT_VARCHAR, newv.id=0, newv.data.string=name;
   newv.size=strlen(newv.data.string)+1;
   newr.addvalue(&newv,1);
   newv.type=ATT_VARCHAR, newv.id=1;
   if (pw[0])
   {
      newv.data.string=pw;
      newv.size=strlen(newv.data.string)+1;
   } else newv.data.string=NULL, newv.size=0;

   newr.addvalue(&newv,1);
   systemdb->gettable("users")->store(&newr);
}
int dbmanager::setuserpassword(char *name, char *password)
{
   record *tentry;
   value *v;
   table *usertable=systemdb->gettable("users");
   value testvalue={ATT_VARCHAR, 0};
   testvalue.size=1+strlen(name);
   testvalue.data.string=name;
   tablesearch tablesearcher(usertable, 1, &testvalue);
   tentry=tablesearcher.getnext();
   if (!tentry) return 0;
   v=tentry->getvalues()+1;
   if (v->data.string) free(v->data.string);
   if (password)
   {
      char pw[100]="";
      strcpy(pw,password);
      docrypt(pw);
      v->data.string=strdup(pw);
      v->size=strlen(pw)+1;
   } else v->data.string=NULL, v->size=0;
   usertable->store(tentry);
   delete tentry;
   return 1;
}
int dbmanager::verifyuser(char *name, char *password)
{
   int ok;
   record *tentry;
   value *v;
   value testvalue={ATT_VARCHAR, 0};
   testvalue.size=1+strlen(name);
   testvalue.data.string=name;
   tablesearch tablesearcher(systemdb->gettable("users"), 1, &testvalue);
   tentry=tablesearcher.getnext();
   if (!tentry) return -1;
   char pw[100]="";
   if (password)
   {
      strcpy(pw,password);
      docrypt(pw);
   }
   v=tentry->getvalues();
   v++;
   ok=v->size==0||strcmp(v->data.string,pw)==0;
   delete tentry;
   return ok?0:1;
}
int dbmanager::adduser(char *name, char *password)
{
   int check=verifyuser(name, password);
   if (check==0||check==1)
   {
      seterror(ERR_USEREXISTS,NULL);
      return 0;
   }
   adduserentry(name, password);
   return 1;
}
table *dbmanager::createtable(database *db, int type, char *name)
{
   char tablename[129];
   strncpy(tablename, name, 128);
   tablename[128]='\0';
   if (db->gettable(tablename))
   {
      seterror(ERR_TBLEXISTS, tablename);
      return NULL;
   }
   stream *rls=unitman->createstream(AVGRLSSIZE);
   stream *meta=unitman->createstream(AVGMETASIZE);
   table *nt=db->newtable(tablename, type, meta, rls);
   addtableentry(db->name, tablename, meta->getstreamnum());
   grantrevoke(db, nt, db->authorization, GRANTSELECT|GRANTINSERT|GRANTDELETE|
      GRANTUPDATE|GRANTDROP|GRANTGSELECT|GRANTGINSERT|GRANTGDELETE|GRANTGUPDATE|
      GRANTGDROP,1,1);
   return nt;
}
void dbmanager::sync()
{
   for (int x=0;x<adatabases;x++) databases[x]->sync();
   unitman->sync();
}
dbunit *dbmanager::getdbunit()
{
   return unitman;
}
int dbmanager::destroydatabase(database *db, int restrict, char *user)
{
   int n=db->getntables(), x;
   seterror(ERR_OK,NULL);
   if (n&&restrict)
   {
      seterror(ERR_DBNOTEMPTY,NULL);
      return 0;
   }
   for (x=0;x<n;x++)
   {
      table *tab=db->gettable(x);
      if (tab) db->destroytable(tab,0);
   }
   tablesearch *search;
   record *r;
   value checks;
   checks.type=ATT_VARCHAR; checks.id=0; checks.data.string=db->name;
   checks.size=strlen(db->name)+1;
   search=new tablesearch(systemdb->gettable("tables"),1,&checks);
   while ((r=search->getnext())!=NULL)
   {
      delete r;
      search->destroyit();
   }
   delete search;
   search=new tablesearch(systemdb->gettable("databases"),1,&checks);
   while ((r=search->getnext())!=NULL)
   {
      delete r;
      search->destroyit();
   }
   delete search;
   search=new tablesearch(systemdb->gettable("grants"),1,&checks);
   while ((r=search->getnext())!=NULL)
   {
      delete r;
      search->destroyit();
   }
   delete search;
   x=0;
   while (databases[x]!=db) x++;
   for (;x<adatabases-1;x++) databases[x]=databases[x+1];
   adatabases--;
   delete db;
   return 1;
}
int dbmanager::destroytable(database *db, table *tab, int restrict)
{
   char *name=strdup(tab->getname());
   seterror(ERR_OK,NULL);
   if (!db->destroytable(tab,restrict))
   {
      free(name);
      return 0;
   }
   tab=NULL;
   tablesearch *search;
   record *r;
   value checks[2];
   checks[0].type=ATT_VARCHAR; checks[0].id=0; checks[0].data.string=db->name;
   checks[0].size=strlen(db->name)+1;
   checks[1].type=ATT_VARCHAR; checks[1].id=1; checks[1].data.string=name;
   checks[1].size=strlen(name)+1;
   search=new tablesearch(systemdb->gettable("tables"),2,checks);
   while ((r=search->getnext())!=NULL)
   {
      delete r;
      search->destroyit();
   }
   delete search;
   search=new tablesearch(systemdb->gettable("grants"),2,checks);
   while ((r=search->getnext())!=NULL)
   {
      delete r;
      search->destroyit();
   }
   delete search;
   free(name);
   return 1;
}
void dbmanager::checkcache()
{
   unitman->checkcache();
}
/*************************************************************************/

database::database(char *n, char *auth, dbunit *um)
{
   char varname[200];
   name=strdup(n);
   unitman=um, authorization=strdup(auth);
   ntables=0, tables=NULL;
   sprintf(varname,"database.%s.tables", name);
   ntablesvar=manager->addvar(varname,ATT_INT);
}
database::~database()
{
   free(name); free(authorization);
   for (int x=0;x<ntables;x++) if (tables[x]) delete tables[x];
   manager->delvar(ntablesvar);
}
int database::destroytable(table *t, int restrict)
{
   for (int x=0;x<ntables;x++) if (tables[x]==t)
   {
      if (restrict)
      {
         tablesearch test(t,0,NULL);
         record *temp=test.getnext();
         if (temp)
         {
            delete temp;
            seterror(ERR_TBLNOTEMPTY,NULL);
            return 0;
         }
      }
      t->alive=0;
      delete tables[x];
      tables[x]=NULL;
      manager->decvar(ntablesvar);
      return 1;
   }
   return 1;
}
void database::sync()
{
   for (int x=0;x<ntables;x++) if (tables[x]) tables[x]->sync();
}
char *database::getauth()
{
   return authorization;
}
int database::deploysystem()
{
   stream *rls[5];
   attribute at;
   for (int x=0;x<5;x++)
   {
      rls[x]=unitman->openstream(x);
      if (rls[x]==NULL) rls[x]=unitman->createstream(AVGRLSSIZE);
      if (rls[x]==NULL)
      {
         if (dberrno!=ERR_NODBFILES) return 0;
         unitman->createfile(SYSSTORAGENAME,SYSSTORAGEPS);
         rls[x]=unitman->createstream(AVGRLSSIZE);
         if (rls[x]==NULL) return 0;
      }
   }
   tables[0]=newtable("databases",TBL_SYSTEM,NULL,rls[0]);
   tables[1]=newtable("users",TBL_SYSTEM,NULL,rls[1]);
   tables[2]=newtable("tables",TBL_SYSTEM,NULL,rls[2]);
   tables[3]=newtable("storage",TBL_SYSTEM,NULL,rls[3]);
   tables[4]=newtable("grants",TBL_SYSTEM,NULL,rls[4]);
/* Database table: */
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"name"); tables[0]->addattribute(&at);
   at.size=sizeof(int), at.type=ATT_INT, at.primary=0, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"type"); tables[0]->addattribute(&at);
   at.size=20, at.type=ATT_VARCHAR, at.primary=0, at.nulls=1, at.foreignkey=-1;
   strcpy(at.name,"authorization"); tables[0]->addattribute(&at);
/* Users table: */
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"name"); tables[1]->addattribute(&at);
   at.size=13, at.type=ATT_VARCHAR, at.primary=0, at.nulls=1, at.foreignkey=-1;
   strcpy(at.name,"passwd"); tables[1]->addattribute(&at);
/* Tables table: */
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"dbname"); tables[2]->addattribute(&at);
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"tblname"); tables[2]->addattribute(&at);
   at.size=sizeof(int), at.type=ATT_INT, at.primary=0, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"page"); tables[2]->addattribute(&at);
/* Storage table: */
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"name"); tables[3]->addattribute(&at);
   at.size=sizeof(int), at.type=ATT_INT, at.primary=0, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"pagesize"); tables[3]->addattribute(&at);
/* Grants table: */
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"dbname"); tables[4]->addattribute(&at);
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"tblname"); tables[4]->addattribute(&at);
   at.size=20, at.type=ATT_VARCHAR, at.primary=1, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"username"); tables[4]->addattribute(&at);
   at.size=sizeof(int), at.type=ATT_INT, at.primary=0, at.nulls=0, at.foreignkey=-1;
   strcpy(at.name,"type"); tables[4]->addattribute(&at);
   return 1;
}
table *database::gettable(int num)
{
   if (num>=ntables) return NULL;
   return tables[num];
}
table *database::gettable(char *name)
{
   for (int x=0;x<ntables;x++) if (tables[x]&&!strcmp(name,tables[x]->name))
      return tables[x]; 
   return NULL;
}
table *database::newtable(char *name, unsigned char type, stream *ms, stream *rls)
{
   ntables++;
   manager->incvar(ntablesvar);
   if (!tables) tables=(table**)malloc(sizeof(table*)*ntables); else
      tables=(table**)realloc(tables,sizeof(table*)*ntables);
   return tables[ntables-1]=new table(name, type, unitman, ms, rls, this->name);
}
char *database::getname()
{
   return name;
}
int database::getntables()
{
   return ntables;
}

/*************************************************************************/
record::~record()
{
   for (int x=0;x<nvalues;x++)
   {
      char *s=values[x].data.string;
      if (s==NULL) continue;
      free(s);
   }
   if (values) free(values);
   if (source) delete source;
}
record::record(stream *s, dbunit *d)
{
   dbu=d, source=s;
   nvalues=0, values=NULL, maxvalues=0;
   if (source)
   {
      int c,av;
      source->seek(0);
      source->read((char*)&av,sizeof(int));
      initvaluestorage(av);
      for (c=0;c<av;c++)
      {
         value v;
         source->read((char*)&v.id,sizeof(int));
         source->read((char*)&v.size,sizeof(int));
         source->read((char*)&v.type,1);
         if (v.size==0) v.data.string=NULL; else
            v.data.string=(char*)malloc(v.size);
         source->read(v.data.string,v.size);
         addvalue(&v,0);
      }
   }
}
char *record::sprintvalue(int x)
{
   if (values[x].size==0) return strdup("NULL");
   char tempbuf[100], *b, *b2;
   switch (values[x].type)
   {
      case ATT_FLOAT: sprintf(tempbuf,"%f",*values[x].data.floating);
                      return strdup(tempbuf); 
      case ATT_MONEY: sprintf(tempbuf,"$ %.2f",*values[x].data.floating);
                      return strdup(tempbuf);
      case ATT_NULL : return strdup("NULL"); 
      case ATT_INT  : sprintf(tempbuf,"%d",*values[x].data.integer);
                      return strdup(tempbuf);
      case ATT_VARCHAR:
      case ATT_CHAR : b=(char*)malloc(values[x].size+3);
                      b2=(char*)malloc(values[x].size+3);
                      strncpy(b2,values[x].data.string,values[x].size);
                      sprintf(b,"'%s'",b2); free(b2);
                      return b;

      case ATT_DATE :
         {
            char b[100];
            struct tm *timep=localtime(values[x].data.date);
            tempbuf[0]='\0';
            if (timep->tm_year!=2||timep->tm_mday!=1||timep->tm_mon!=0)
               sprintf(tempbuf,"%d %s %d",timep->tm_mday, months[timep->tm_mon],
               timep->tm_year+1900);
            if (timep->tm_hour||timep->tm_sec||timep->tm_min)
            {
               sprintf(b,"%s%02d:%02d:%02d", tempbuf[0]?" ":"",
                  timep->tm_hour, timep->tm_min, timep->tm_sec);
               strcat(tempbuf,b);
            }
            return strdup(tempbuf);
         }
   }
   return NULL;
}
void record::initvaluestorage(int amount)
{
   int oldamount=maxvalues,x;
   if (amount<nvalues) return;
   if (values) values=(value*)realloc(values,sizeof(value)*amount); else
      values=(value*)malloc(sizeof(value)*amount);
   maxvalues=amount;
   for (x=oldamount;x<maxvalues;x++) values[x].type=0;
}
int record::getnvalues()
{
   return nvalues;
}
int record::getsize()
{
   int x, size=nvalues*(1+2*sizeof(int))+sizeof(int);
   for (x=0;x<nvalues;x++) size+=values[x].size;
   return size;
}
void record::closestream()
{
   if (source) delete source;
   source=NULL;
}
stream *record::tostream()
{
   int x, size=getsize(), truncto=-1;
   if (source==NULL) source=dbu->createstream(size); else
   {
      source->seek(0);
      truncto=size;
   }
   source->write((char*)&nvalues,sizeof(int));
   for (x=0;x<nvalues;x++)
   {
      value *v=values+x;
      source->write((char*)&v->id,sizeof(int));
      source->write((char*)&v->size,sizeof(int));
      source->write((char*)&v->type,1);
      if (v->size) source->write(v->data.string,v->size);
   }
   if (truncto!=-1) source->trunc(truncto);
   return source;
}
stream *record::getsource()
{
   return source;
}
void record::addvalue(value *v, int copydataarea)
{
   if (v->id>=nvalues)
   {
      nvalues=v->id+1;
      if (nvalues>maxvalues) initvaluestorage(nvalues);
   }
   memcpy(values+v->id,v,sizeof(value));
   if (copydataarea&&v->size)
   {
      values[v->id].data.string=(char*)malloc(v->size);
      memcpy(values[v->id].data.string, v->data.string, v->size);
   }
}
void *record::getvalue(int x)
{
   if (x>=nvalues) return NULL;
   return values[x].data.string;
}
value *record::getvalues()
{
   return values;
}
/*************************************************************************/
tablesearch::tablesearch(table *t, int amount, value *values)
{
   parent=t, ntests=amount, dbu=t->getdbunit();
   tests=(value**)(amount?malloc(sizeof(value*)*amount):NULL);
   if (amount) for (int x=0;x<amount;x++)
   {
      tests[x]=(value*)malloc(sizeof(value));
      *tests[x]=values[x];
   }
   masteroffset=acache=location=0;
   initcache(0);
   manager->incvar(t->readvar);
}
tablesearch::~tablesearch()
{
   if (!tests) return;
   for (int x=0;x<ntests;x++) free(tests[x]);
   free(tests);
}
void tablesearch::addvalue(value *val)
{
   ntests++;
   if (tests) tests=(value**)realloc(tests,sizeof(value*)*ntests); else
      tests=(value**)malloc(sizeof(value*));
   tests[ntests-1]=(value*)malloc(sizeof(value));
   *tests[ntests-1]=*val;
}
void tablesearch::initcache(int offset)
{
   stream *rls=parent->recordliststream;
   int toread=parent->nrecords-offset;
   acache=(toread<SEARCHCACHE)?toread:SEARCHCACHE;
   masteroffset=offset;
   if (acache)
   {
      rls->seek(sizeof(int)+offset*sizeof(int));
      rls->read((char*)streamcache,acache*sizeof(int));
   }
}
int tablesearch::nextstream()
{
   int num;
   do
   {
      if (location>=parent->nrecords) return -1;
      if (location>=(acache+masteroffset)||location<masteroffset)
         initcache(masteroffset=location);
      num=streamcache[location-masteroffset];
      location++;
   } while (num==-1);
   return num;
}
int tablesearch::valid(stream *s)
{
   int amount,x,y,tomeet=ntests;
   if (!ntests) return 1;
   s->read((char*)&amount,sizeof(int));
   for (x=0;x<amount;x++)
   {
      char *buf;
      struct
      {
         int id;
         int size;
         unsigned char type;
      } temp;
      int ok=0;
      s->read((char*)&temp,2*sizeof(int)+1);
      if (temp.size==0) return 0;
      for (y=0;y<ntests;y++) if (temp.id==tests[y]->id)
      {
         int cmpbytes=tests[y]->size<temp.size?tests[y]->size:temp.size;
         if (temp.size<=1024) buf=storage; else buf=(char*)malloc(temp.size);
         s->read(buf,temp.size);
         if (memcmp(buf,tests[y]->data.string,cmpbytes))
         {
            if (buf!=storage) free(buf);
            return 0;
         }
         if (buf!=storage) free(buf);
         if (!--tomeet) return 1;
         ok=1;
      }
      if (!ok) s->read(NULL,temp.size);
   }
   return 0;
}
record *tablesearch::getnext()
{
   int next=nextstream();
   while (next!=-1)
   {
      stream *s=dbu->openstream(next);
      if (s)
      {
         if (valid(s)) return new record(s,dbu);
         delete s;
      }
      next=nextstream();
   }
   return NULL;
}
void tablesearch::destroyit()
{
   int num=-1, streamnum;
   if (!location) return;
   stream *rls=parent->recordliststream;
   location--;
   rls->seek(sizeof(int)+location*sizeof(int));
   rls->write((char*)&num,sizeof(int));
   streamnum=streamcache[location-masteroffset];
   streamcache[location-masteroffset]=-1;
   location++;
   parent->fragmentation=1;
   dbu->destroystream(dbu->openstream(streamnum));
   manager->incvar(parent->writevar);
}
void tablesearch::rewind()
{
   location=0;
   initcache(0);
}
int tablesearch::gethandle()
{
   return location-1;
}
void tablesearch::sethandle(int handle)
{
   location=handle;
}
/*************************************************************************/
table::table(char *n, unsigned char t, dbunit *dbu, stream *mstream,
    stream *rls, char *dbname)
{
   name=strdup(n), metastream=mstream, recordliststream=rls, alive=1;
   nattributes=0, attributes=NULL, unitman=dbu, type=t, dirty=0;
   fragmentation=0, foreignkeys=NULL, nforeigns=0, actions=NULL;
   rootgrant=NULL;
   if (metastream) initfromstream();
   recordliststream->seek(0);
   if (recordliststream->read((char*)&nrecords,sizeof(int))!=sizeof(int)) 
      nrecords=0;
   char varname[300];
   sprintf(varname,"database.%s.%s.reads",dbname,name);
   readvar=manager->addvar(varname,ATT_INT);
   sprintf(varname,"database.%s.%s.writes",dbname,name);
   writevar=manager->addvar(varname,ATT_INT);
}
void table::grant(char *who, int type)
{
   grantstruct *temp;
   for (temp=rootgrant;temp;temp=temp->next) if (!strcmp(who,temp->name))
   {
      temp->type|=type;
      return;
   }
   temp=(grantstruct*)malloc(sizeof(grantstruct));
   temp->prev=NULL, temp->next=rootgrant, temp->name=strdup(who);
   temp->type=type;
   if (rootgrant) rootgrant->prev=temp;
   rootgrant=temp;
}
void table::revoke(char *who, int type)
{
   grantstruct *temp;
   for (temp=rootgrant;temp;temp=temp->next) if (!strcmp(who,temp->name))
   {
      temp->type&=(unsigned)~(unsigned)type;
      /*(unsigned)temp->type&=(unsigned)~(unsigned)type;*/
      break;
   }
   if (!temp) return;
   if (temp->type==0)
   {
      if (temp->next) temp->next->prev=temp->prev;
      if (temp->prev) temp->prev->next=temp->next; else rootgrant=temp->next;
      free(temp->name);
      free(temp);
   }
}
table::~table()
{
   if (alive) sync(); else destroy();
   if (metastream) delete metastream;
   delete recordliststream;
   free(name);
   if (attributes) delete attributes;
   if (foreignkeys) free(foreignkeys);
   if (actions) free(actions);
   grantstruct *temp=rootgrant;
   while (temp)
   {
      grantstruct *t2=temp->next;
      free(temp->name);
      free(temp);
      temp=t2;
   }
   manager->delvar(readvar);
   manager->delvar(writevar);
}
void table::defrag()
{
   int *buf=(int*)malloc(sizeof(int)*nrecords), storeat=0;
   recordliststream->seek(sizeof(int));
   recordliststream->read((char*)buf,nrecords*sizeof(int));
   for (int x=0;x<nrecords;x++)
   {
      if (buf[x]==-1) continue;
      if (storeat!=x) buf[storeat]=buf[x];
      storeat++;
   }
   recordliststream->seek(0);
   recordliststream->write((char*)&storeat,sizeof(int));
   recordliststream->write((char*)buf,sizeof(int)*storeat);
   recordliststream->trunc(sizeof(int)*(1+storeat));
   nrecords=storeat;
   free(buf);
   fragmentation=0;
}
void table::sync()
{
   if (fragmentation) defrag();
   if (dirty) syncmeta();
}
void table::addattribute(attribute *a)
{
   int x=0;
   while (getattribute(x)) x++;
   a->id=x;
   nattributes++;
   if (attributes) attributes=(attribute*)realloc(attributes,
      sizeof(attribute)*nattributes); else
      attributes=(attribute*)malloc(sizeof(attribute));
   memcpy(attributes+nattributes-1,a,sizeof(attribute));
   dirty=1;
}
void table::addfkey(char *s, int action)
{
   nforeigns++;
   if (foreignkeys) foreignkeys=(char (*)[40])realloc(foreignkeys,40*nforeigns);
      else foreignkeys=(char (*)[40])malloc(40);
   if (actions) actions=(int*)realloc(actions,sizeof(int)*nforeigns);
      else actions=(int*)malloc(sizeof(int));
   strcpy(foreignkeys[nforeigns-1],s);
   actions[nforeigns-1]=action;
   dirty=1;
}
int table::isgranted(char *who, int granttype)
{
   grantstruct *temp;
   if (who&&!strcmp(who,"system")&&type!=TBL_SYSTEM) return 1;
   if (who) for (temp=rootgrant;temp;temp=temp->next)
      if (!strcmp(temp->name,who)) return (granttype&temp->type)==granttype;
   for (temp=rootgrant;temp;temp=temp->next)
      if (!strcmp(temp->name,"all")) return (granttype&temp->type)==granttype;
   return 0;
}
grantstruct *table::getrootgrant()
{
   return rootgrant;
}
int table::getnkeys()
{
   return nforeigns;
}
char *table::getfkey(int x)
{
  if (x>=nforeigns) return NULL;
  return foreignkeys[x];
}
int table::getaction(int x)
{
  if (x>=nforeigns) return 0;
  return actions[x];
}
attribute *table::getattribute(int id)
{
   if (id>=nattributes) return NULL;
   return attributes+id;
}
void table::initfromstream()
{
   int dum, rls;
   if (metastream->getsize()==0)
   {
      dirty=1;
      return;
   }
   metastream->read((char*)&type,1);
   metastream->read((char*)&dum,sizeof(dum));
   metastream->read((char*)&dum,sizeof(dum));
   metastream->read((char*)&rls,sizeof(rls));
   metastream->read((char*)&nattributes,sizeof(rls));
   attributes=(attribute*)malloc(sizeof(attribute)*nattributes);
   metastream->read((char*)attributes,nattributes*sizeof(attribute));
   metastream->read((char*)&nforeigns,sizeof(nforeigns));
   if (nforeigns)
   {
      foreignkeys=(char (*)[40])malloc(40*nforeigns);
      actions=(int*)malloc(sizeof(int)*nforeigns);
      metastream->read((char*)foreignkeys,sizeof(*foreignkeys)*nforeigns);
      metastream->read((char*)actions,sizeof(int)*nforeigns);
   }
   recordliststream=unitman->openstream(rls);
}
void table::initgrants(char *dbname, table *gt)
{
   record *tentry;
   value testvalue[2]={{ATT_VARCHAR, 0},{ATT_VARCHAR,1}};
   testvalue[0].size=1+strlen(dbname);
   testvalue[0].data.string=dbname;
   testvalue[1].size=1+strlen(name);
   testvalue[1].data.string=name;
   tablesearch tablesearcher(gt, 2, testvalue);
   while ((tentry=tablesearcher.getnext())!=NULL)
   {
      value *v=tentry->getvalues();
      grant(v[2].data.string,*v[3].data.integer);
      delete tentry;
   }
}
int table::store(record *r)
{
   stream *source=r->getsource();
   stream *ns=r->tostream();
   manager->incvar(writevar);
   if (source) return 1;
   int num=ns->getstreamnum();
   nrecords++;
   recordliststream->seek(0);
   recordliststream->write((char*)&nrecords,sizeof(int));
   recordliststream->toend();
   recordliststream->write((char*)&num,sizeof(int));
   return 1;
}
int table::getnattributes()
{
   return nattributes;
}
int table::getnrecords()
{
   return nrecords;
}
void table::syncmeta()
{
   if (!metastream) return;
   int dum=0, rls=recordliststream->getstreamnum();
   metastream->seek(0);
   metastream->write((char*)&type,1);
   metastream->write((char*)&dum,sizeof(dum));
   metastream->write((char*)&dum,sizeof(dum));
   metastream->write((char*)&rls,sizeof(rls));
   metastream->write((char*)&nattributes,sizeof(rls));
   metastream->write((char*)attributes,nattributes*sizeof(attribute));
   metastream->write((char*)&nforeigns,sizeof(nforeigns));
   if (nforeigns)
   {
      metastream->write((char*)foreignkeys,sizeof(*foreignkeys)*nforeigns);
      metastream->write((char*)actions,sizeof(int)*nforeigns);
   }
   dirty=0;
}
dbunit *table::getdbunit()
{
   return unitman;
}
void table::destroy()
{
   if (nrecords)
   {
      int x, *cache;
      cache=(int*)malloc(sizeof(int)*nrecords);
      recordliststream->seek(sizeof(int));
      recordliststream->read((char*)cache,sizeof(int)*nrecords);
      for (x=0;x<nrecords;x++) if (cache[x]!=-1)
      {
         stream *temp=unitman->openstream(cache[x]);
         unitman->destroystream(temp);
         delete temp;
      }
      free(cache);
   }
   unitman->destroystream(recordliststream);
   unitman->destroystream(metastream);
}
char *table::getname()
{
   return name;
}
