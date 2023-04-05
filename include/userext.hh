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

#ifndef userexthh
#define userexthh
#include "interface.hh"
#include "absuserext.hh"
#include "relation.hh"
#include "sql.hh"
extern char *errors[100];
extern int nerrors;
class userext : public absuserext
{
   dbms *dbm;
   sqltree *tree;
   int bufsize;
   char *buf, *username;
   transaction *trans;
   void addline(char *s);
   void doparse();
   void help(char *);
   void status();
   void list(char *);
   char *sprintheader(char *, sqlsearch *);
   char *sprintrecord(char *, sqlsearch *);
   char *closeheader(char *, sqlsearch *);
   void sprintmoney(char *, double, int);
   void sprintfdate(char *buf, time_t date);
   public:

   virtual void run();
   userext(user *, dbms *);
   virtual ~userext();
   virtual void parse(char *);
   virtual void sendping();
};
#endif
