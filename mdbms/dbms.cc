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
#include "support.h"
#include "global.hh"
#include "dbms.hh"

manage *manager=NULL;

dbms::dbms(int cachemem, int hip, int fop)
{
   busy=0, needsync=0, prev_sync=0, prev_busy=0, prev_cache=time(NULL);
   manager=new manage;
   initmanagement(cachemem);
   dbman=new dbmanager(cachemem);
   tcpi=new tcpinterface(hip,this,0,convarcurr,convartotal,convartimeout);
   quicki=new tcpinterface(fop,this,1,convarcurr,convartotal,convartimeout);
   tcpi->setprompt("* ");
}
dbms::~dbms()
{
   delete tcpi;
   delete quicki;
   delete dbman;
   delete manager;
}
void dbms::initmanagement(int cachemem)
{
   char *physversion=getversion();
   char string[]="Not determined";
   int varnum;
   varnum=manager->addvar("version.system",ATT_VARCHAR);
   manager->setvar(varnum, VERSION);
   varnum=manager->addvar("version.FOP",ATT_VARCHAR);
   manager->setvar(varnum, FOPVERSION);
   varnum=manager->addvar("version.physical.supported",ATT_VARCHAR);
   manager->setvar(varnum, PHYSVERSION);
   varnum=manager->addvar("version.physical.detected",ATT_VARCHAR);
   manager->setvar(varnum, physversion?physversion:string);
   if (physversion) free(physversion);
   varnum=manager->addvar("cache.size",ATT_INT);
   manager->setvar(varnum, cachemem);
   cachevarhit=manager->addvar("cache.hits",ATT_INT);
   cachevarfail=manager->addvar("cache.fails",ATT_INT);
   cachevareff=manager->addvar("cache.effectiveness",ATT_INT);
   convarcurr=manager->addvar("interface.connections.current",ATT_INT);
   convartotal=manager->addvar("interface.connections.total",ATT_INT);
   convartimeout=manager->addvar("interface.connections.timeouts",ATT_INT);
}
void dbms::calccache()
{
   int hits=manager->getvar(cachevarhit)->value.number;
   int fails=manager->getvar(cachevarfail)->value.number;
   int effective;
   if (hits<0||fails<0)
   {
      manager->setvar(cachevarhit,0);
      manager->setvar(cachevarfail,0);
      return;
   }
   if (hits==0&&fails==0) return;
   effective=(int)((hits/((double)(hits+fails)))*100);
   manager->setvar(cachevareff,effective);
}
void dbms::run()
{
   tcpi->run(busy);
   quicki->run(busy);
   time_t now=time(NULL);
   if (!busy)
   {
      if ((now-prev_cache)>=10)
      {
         prev_cache=now;
         dbman->checkcache();
      }
      if (!needsync&&((now-prev_sync)>=60)) needsync=1;
      if (needsync==1&&((now-prev_sync)>=600)) needsync=2;
      if (needsync&&((now-prev_busy)>5||(needsync==2)))
      {
         prev_sync=time(NULL);
         needsync=0;
         dbman->sync();
         calccache();
      }
   }
}
void dbms::setbusy()
{
   busy++;
   prev_busy=time(NULL);
}
void dbms::setfree()
{
   busy--;
}
dbmanager *dbms::getdbman()
{
   return dbman;
}
void dbms::sync()
{
   needsync=2;
}
int dbms::isbusy()
{
   return busy;
}
int dbms::dbinuse(database *db, absuserext *ext)
{
   return tcpi->dbinuse(db,ext)||quicki->dbinuse(db,ext);
}
