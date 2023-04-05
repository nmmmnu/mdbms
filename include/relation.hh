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

#ifndef relationhh
#define relationhh
#include "dbunits.hh"
#include "attributes.h"
#include <time.h>

#define SYSSTORAGENAME "system"
#define SYSSTORAGEPS   100
#define SEARCHCACHE    1000

#define AVGMETASIZE    100
#define AVGRLSSIZE     100

#define GRANTSELECT    1
#define GRANTINSERT    2
#define GRANTDELETE    4
#define GRANTUPDATE    8
#define GRANTDROP      16
#define GRANTGSELECT   32
#define GRANTGINSERT   64
#define GRANTGDELETE   128
#define GRANTGUPDATE   256
#define GRANTGDROP     512


/*
Metastream:
   
      Table type
      #Indexes            
      MetaIndexstream ID 
      Recordliststream
      #Attributes       
   Repeat:             
      Attribute structure
*/
struct grantstruct
{
   grantstruct *next, *prev;
   char *name;
   int type;
};
struct attribute
{
   int id, size, foreignkey;
   char name[40];
   unsigned char type;
   unsigned primary : 1;
   unsigned nulls   : 1;
};
struct value
{
   unsigned char type;
   int id, size;
   union
   {
      char *string;
      int *integer;
      time_t *date;
      double *floating;
   } data;
};
class record
{
   int nvalues, maxvalues;
   value *values;
   int getsize();
   void initvaluestorage(int);
   stream *source;
   dbunit *dbu;
   public:

   char *sprintvalue(int);
   record(stream *, dbunit *);
   ~record();
   int getnvalues();
   void closestream();
   void addvalue(value *, int);
   void *getvalue(int);
   value *getvalues();
   stream *getsource();
   stream *tostream();
};

/*************************************
   Table classes:
   *  One for every table in a database
   *  Managed by database class
   *  Permanent in memory
   *  Class is shared
*/
class database;
class table
{
   dbunit *unitman;
   int alive, fragmentation;
   int readvar, writevar;
   unsigned char type;
   grantstruct *rootgrant;
   char *name;
   int nforeigns, *actions;
   char (*foreignkeys)[40];
   stream *metastream; /* stream containing table meta data */
   stream *recordliststream;
   int dirty;
   int nrecords;
   int nattributes;
   attribute *attributes;
   void initfromstream();
   void initgrants(char *, table *);
   void freerecord(record *);
   void syncmeta();
   void sync();
   void destroy();
   void defrag();
   public:

   char *getname();
   dbunit *getdbunit();
   int getnrecords();
   table(char *, unsigned char, dbunit *, stream *, stream *, char *);
   ~table();
   int store(record *);
   int getnkeys();
   char *getfkey(int);
   int getaction(int);
   void addattribute(attribute *a);
   void addfkey(char *, int);
   attribute *getattribute(int id);
   int getnattributes();
   int isgranted(char *, int);
   void grant(char *, int);
   void revoke(char *, int);
   grantstruct *getrootgrant();
   friend class database;
   friend class dbmanager;
   friend class tablesearch;
};
class tablesearch
{
   table *parent;
   char storage[1024];
   int ntests, location; 
   dbunit *dbu;
   value **tests;
   int streamcache[SEARCHCACHE], masteroffset, acache;
   void initcache(int);
   int nextstream();
   int valid(stream *);
   public: 

   void destroyit();
   tablesearch(table *, int, value *);
   ~tablesearch();
   void addvalue(value *);
   void rewind();
   int gethandle();
   void sethandle(int);
   record *getnext();
};
/*******************************
   Dbmanager class:
   *  One per DBMS
   *  Controls database classes
*/
class dbmanager
{
   dbunit *unitman;
   int adatabases;
   database *systemdb;
   database **databases;
   database *newdatabase(char *, char *);
   void adddatabaseentry(char *, char *, int);
   void addtableentry(char *, char *, int);
   void addstorageentry(char *, int);
   void adduserentry(char *, char *);
   void addgrantentry(char *, char *, char *, int);
   void deploysystem();
   public:

   dbmanager(int);
   ~dbmanager();
   database *opendatabase(char *);
   int createstorage(char *, int);
   database *createdatabase(char *, char *);
   table *createtable(database *, int, char *);
   dbunit *getdbunit();
   int destroytable(database *, table *, int);
   int destroydatabase(database *, int, char *);
   void sync();
   void checkcache();
   int verifyuser(char *, char *);
   int adduser(char *, char *);
   int setuserpassword(char *, char *);
   void grantrevoke(database *, table *, char *, int, int, int);
};
/****************************************
   Database classes:
   *  One for every database in the dbms
   *  Managed by dbmanager
   *  Permanent in memory
   *  Class is shared
*/
class database
{
   char *name, *authorization;
   int ntables;
   int ntablesvar;
   table **tables;
   dbunit *unitman;
   table *newtable(char *, unsigned char, stream *, stream *);
   int deploysystem();
   void sync();
   int destroytable(table *, int);
   public: 

   database(char *, char *, dbunit *);
   ~database();
   int getntables();
   table *gettable(char *);
   table *gettable(int);
   char *getname();
   char *getauth();
   friend class dbmanager;
};
#endif
