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

typedef struct variable
{
   char *name;
   char *cont;
} variable;
typedef struct query
{
   char *name;
   int nrecords, counter, defaultindex;
   record **records;
   record header;
   struct query *next, *prev;
} query;
typedef struct connection
{
   int fd;
   char *name;
   struct connection *next, *prev;
} connection;
extern int nvars;
extern variable *vars;
extern query *rootquery;
extern connection *rootconnection;
extern int filelevel;
extern db_session *globalsession;
