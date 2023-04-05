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

#include "userext.hh"
absuserext::absuserext(user *u)
{
   parent=u, currentdb=NULL;
}
absuserext::~absuserext() { }
void absuserext::run() { }
void absuserext::parse(char *) { }
void absuserext::sendping() { }
database *absuserext::getcurrentdb()
{
   return currentdb;
}
