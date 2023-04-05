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

#ifndef sqlsearchhh
#define sqlsearchhh
#include <time.h>
#include "relation.hh"
#include "sql.hh"

union globalv
{
   double floating;
   int integer;
   time_t date;
   char *string;
};
struct tabentry
{
   int parent;
   table *tab;
   tablesearch *ts;
   record *currentr;
   char name[40];
};
struct attentry
{
   char name[40];
   attribute *at;
   int tabnum, tabentry, dupped;
   int asciilen, type;
   globalv storage;
   node *tree;
};
struct forwardentry
{
   node *tree;
   int type, maxvalues, nvalues, sorttype;
   globalv *values;
};
class sqlsearch;
struct sortstruct
{
   int *handle;
   int index;
};
class sorter
{
   public:
   int ngroupbys;
   sqlsearch *parent;
   sortstruct *list;
   int *groups, ngroups;
   int *handles, nhandles, maxhandles, nelements, tables;
   forwardentry *groupbys;
   sorter(int, int, sqlsearch *); 
   ~sorter();
   void set(int, int, node *, int);
   void reset();
   globalv *addvalue(int);
   node *gettree(int);
   void checkvalue(int);
   int getamount();
   void addhandle(int);
   void sort();
   void setgroups();
};
struct subs
{
   sqlsearch *search;
   globalv value;
   int needmore, valueok, resultready, result;
};
class sqlsearch
{
   dbmanager *dbm;
   database *dbase;
   node *tree, *select, *from, *where, *groupby, *orderby;
   int permissions;
   char *permwho;
   int success, ntables, forwardsearch, amount, globaltype;
   int forward, delayed, firstentry, nrecords, nsubs, localtables;
   int groupsready, currentindex, currentgroup, orderready;
   sorter *groupbys, *orderbys;
   subs *subselect;
   globalv *globalvalue;
   tabentry *tables;
   sqlsearch *parent;
   void reset();
   void clearstorage();
   int filltables();
   int fillattr();
   void fillts();
   int currentok;
   int nextentry();
   int primaryrun();
   int groupbyrun();
   int orderbyrun();
   void activate();
   void activateincurrent();
   void activateingroup(sorter *);
   void delayedactivate();
   int getsize(value *);
   int fillall();
   int checkgroupby();
   int checkorderby();
   int parseattribute(node *);
   int checktree(node *);
   int checkaddsub(node *);
   int checkmuldiv(node *);
   int checkunary(node *);
   int checkaggregate(node *);
   int checkfunction(node *);
   int checkprocedure(node *);
   int checklike(node *);
   int checkupdate();

   int execfunction(node *);
   int execprocedure(node *);
   int exectree(node *);
   int execident(node *);
   int execaddsub(node *);
   int execattrib(node *);
   int execmuldiv(node *);
   int execuminus(node *);
   int execcolfunc(node *);

   int checkwhere(node *, int);
   int checkand(node *, int);
   int checkor(node *);
   int checkother(node *);
   int checkequals(node *, int);
   int checkinlist(node *);
   int checkin(node *);
   int checkbetween(node *);
   int checkexists(node *);
   int parsewhere(node *);
   int parseequals(node *);
   int parsenull(node *);
   int parsein(node *n);
   int parseinlist(node *);
   int parsebetween(node *);
   int parseexists(node *);
   int parselike(node *);
   int checkok(node *n);

   public:
   record *getcurrent();
   int lowcompare(int, globalv *, globalv *);
   value *currentrecord;
   int recordready, nattributes;
   attentry *attributes;
   sqlsearch(dbmanager *, database *, node *, sqlsearch *, int, char *);
   ~sqlsearch();
   int run();
   int getamount();
   void dropprevious();
   void doupdate();
   friend class transaction;
   friend class sorter;
};

#endif
