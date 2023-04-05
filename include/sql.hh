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

#ifndef sqlhh
#define sqlhh
#include "nodes.h"
#include "relation.hh"
#include "sqlsearch.hh"
struct error
{
   int num; 
   char *string;
};
struct refentry
{
   int key, action;
   int nkeys;
   char **names, *tabname;
};
class transaction;
class sqlsearch;
class dbms;
class absuserext;
class refdelete
{
   absuserext *parent;
   int nentrys, *indexes, nkeys;
   refentry *entrys;
   transaction *currenttrans;
   sqlsearch *currentsearch;
   database *cdb;
   dbms *cdbms;
   char **currentuser;
   public:
   int getnentrys();
   void run();
   int checkrestrict();
   refdelete(database *,char *,sqlsearch *, dbms *, char **, absuserext *);
   ~refdelete();
};
class sqltree;
class transaction
{
   dbms *dbmsp;
   dbmanager *dbm;
   database **currentdb;
   absuserext *parent;
   table *currenttable;
   char **user;
   int opstat;
   sqltree *thistree;
   refdelete *currentref;
   int nocurrentdb();
   void setopstat();
   void start(node *);
   void rootnode(node *);
   void nodecreate(node *);
   void docreatedb(node *);
   int checkprimary(node *);
   int checkforeigns(node *);
   int checkforeignkey(table *, node *, node *);
   void docreatetbl(node *);
   void docreateview(node *);
   void docreateuser(node *);
   void nodeopen(node *);
   void nodeuser(node *);
   void nodeinsert(node *);
   int checkandadd(node *, record *, table *);
   void nodehelp(char *);
   int startsearch(node *, int);
   void nodedrop(node *);
   void dodroptable(node *);
   void dodropdb(node *);
   void nodedelete(node *);
   void nodeset(char *, char *);
   void nodegr(node *, int);

   public:
   int type, amount;
   sqlsearch *currentsearch;
   int run();
   int busy, recordready;
   sqlsearch *getsearch();
   transaction(dbms *, database **, char **, absuserext *);
   ~transaction();
   void execute(sqltree *);
};
class sqltree
{
   node *tree;
   error *err;
   char *messages;
   int nerrors;
   void addmessage(char *);
   public:

   int getnerrors();
   error *geterror(int);
   char *getserror(int);
   char *getmessages();
   void adderror();
   sqltree(char *);
   ~sqltree();
   friend class transaction;
};
#endif
