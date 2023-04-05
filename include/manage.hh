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

#ifndef managehh
#define managehh

#include "relation.hh"

struct managevar
{
   int type;
   char *name;
   union
   {
      int number; 
      char *string;
   } value;
};
class manage
{
   int nvars;
   managevar *variables;
   public:
   manage();
   ~manage();
   int addvar(char *, int);
   int getvarnum(char *name);
   void incvar(int);
   void setvar(int, int);
   void setvar(int, char *);
   void decvar(int);
   void delvar(int);
   managevar *getvar(int);
   int getnvars();
   char *sprintvalue(int, char *);
};


#endif
