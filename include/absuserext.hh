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

#ifndef absuserexthh
#define absuserexthh
class user;
class absuserext
{
   protected:
   database *currentdb;
   user *parent;
   public:

   virtual void run();
   absuserext(user *);
   virtual ~absuserext();
   virtual void parse(char *);
   virtual void sendping();
   database *getcurrentdb();
};
#endif
