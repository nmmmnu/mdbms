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

#ifndef fastuserexthh
#define fastuserexthh
#include "interface.hh"
#include "absuserext.hh"
#include "relation.hh"
#include "sql.hh"
extern char *errors[100];
extern int nerrors;
class fastuserext : public absuserext
{
   int transnum;
   dbms *dbm;
   sqltree *tree;
   char *username;
   transaction *trans;
   void showcode(int);
   void doparse(char *);
   void dohelp(char *);
   void sprintrecord(char *, sqlsearch *);
   public:

   virtual void run();
   fastuserext(user *, dbms *);
   virtual ~fastuserext();
   virtual void parse(char *);
   virtual void sendping();
};
#endif
