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

#ifndef dbmshh
#define dbmshh
#include "relation.hh"
#include "interface.hh"
#include <time.h>
class dbms
{
   time_t prev_sync, prev_busy, prev_cache;
   int busy, needsync;
   int cachevareff, cachevarhit, cachevarfail;
   int convarcurr, convartotal, convartimeout;
   dbmanager *dbman;            
   tcpinterface *tcpi, *quicki;
   void initmanagement(int);
   void calccache();

   public:
   dbms(int, int, int);
   ~dbms();
   void run();
   dbmanager *getdbman();
   void sync();
   void setbusy();
   void setfree();
   int isbusy();
   int dbinuse(database *, absuserext *);
};
#endif
