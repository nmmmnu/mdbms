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

/*
 *  A skeleton C program, which uses the database library.
 *  Read libdbms.h for more information on using mdbms in a C program.
 *
 */

#include "libdbms.h"

char *hostname="localhost";      /* A hostname or ip address*/
char *database="system";               /* A database name */

char *systempw=NULL;                               /* The system password*/


void create_test_table(db_session *session)
{
   printf("now creating the table\n");
   db_exec_sql(session,1,"create table test ( index integer );");
}

void delete_test_table(db_session *session)
{
   printf("\nemptying the table\n");
   if  ( db_exec_sql(session,1,"delete from test;")!=0)
   {
      printf("could not delete values\n");
   } 
   printf("now dropping the table\n");
   db_exec_sql(session,1,"drop table test;");
}

void put_some_values(db_session *session)
{
   int x;
   printf("putting values\n");
   for (x=0;x<100;x++)
   {
      /* Execute the statement, arg 2 means don't wait for query result */
      db_exec_sql(session, 0, "insert into test values (%d)", x);
   }
}

void read_some_values(db_session *session)
{
   record *rec;
   db_exec_sql(session,1,"select * from test;");
   while (rec=db_get_next_record(session))
   {
      printf("Value : %d",rec->values[0].value.integer);
      db_free_record(rec);
   }
}

main()
{
   db_session *session;

   /* First set the hostname of the server */
   printf("set hostname\n");
   db_sethostname(hostname);

   /* Open a new database session */
   printf("open data base session\n");
   session=db_open_session();
   printf("set user\n"); 
   db_set_global_authentication("system",systempw);

   /* Try opening the database */
   printf("now opening database\n");
   if (db_open_database(session,database)!=0)
   {
      /* The database is not present, we have to create it first */
      printf("database not present\n");
      /* Set user to system user */
      db_set_global_authentication("system",systempw);

      /* Try to create it; arg 2 means:wait for result on query */
      if (db_exec_sql(session, 1, "create database %s", database)!=0)
      {
         printf("Could not create database.\n");
         exit(1);
      }
      db_open_database(session,database);
   }

   create_test_table(session);
   put_some_values(session);
   read_some_values(session);
   delete_test_table(session);
}
