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

#ifndef interfacehh
#define interfacehh
#include <sys/types.h>
#include "relation.hh"
#include "absuserext.hh"
class user;
class dbms;
class tcpinterface
{
   int sock, type;
   int varcurrent, vartotal, vartimeout;
   user *rootuser;
   dbmanager *dbm;
   dbms *parent;
   char *prompt;
   int doselect(int);
   void resetmasks();
   void newuser();
   void removeuser(user *);
   fd_set rmask, wmask, rrmask, rwmask;
   public:
   tcpinterface(int, dbms *, int, int, int, int);
   ~tcpinterface();
   void setprompt(char *);
   void run(int);
   void setbusy();
   void setfree();
   int dbinuse(database *, absuserext *);
   friend class user;
};
class user
{
   protected:
   int fd, alive, insize, outsize;
   time_t lastactive, lastping;
   int timeout;
   int blocked;
   user *next, *prev;
   char *inbuf, *outbuf;
   char *prompt;
   absuserext *ext;
   void setmasks();
   void input();
   void output();
   void run();
   void setprompt(char *);
   tcpinterface *parent;

   public:
   user(int, dbms *, tcpinterface *);
   ~user();
   void setload();
   void unload();
   int nextline(char *, char *, int);
   void uprintf(char *, ...);
   void block();
   void unblock();
   void printprompt();
   void setactive();
   void sendping();
   friend class tcpinterface;
   friend class fastuserext;
   friend class userext;
};
#include "userext.hh"
#endif
