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
#include <string.h>
#include "../../lib/errors.h"
#include "../../lib/libdbms.h"

char sep=':';

void usage()
{
   printf("Usage: getdb [-u user] [-p pwd] [-t port] [-i] [-s separator] <host> <database> <query>\n");
   printf("  -u : specify user\n");
   printf("  -p : specify password\n");
   printf("  -t : specify TCP port to connect to\n");
   printf("  -i : prompt for user/password\n");
   printf("  -s : specify separator character\n");
   exit(0);
}
int main (int argc, char **argv)
{
   db_session *session;
   char query[1000]="", *p;
   int x, result, port=2224;
   record header, *rec;
   int ch;
   char *user=NULL, *pwd=NULL;
   while ((ch=getopt(argc,argv,"u:p:t:is:"))!=EOF) switch(ch)
   {
      case 'u': user=strdup(optarg); break;
      case 'p': pwd=strdup(optarg); break;
      case 't': port=atoi(optarg); break;
      case 'i': {
                   char pw[100],u[100];
                   printf("User: "); fgets(u,100,stdin);
                   printf("Pwd : "); fgets(pw,100,stdin);
                   u[strlen(u)-1]='\0';
                   pw[strlen(pw)-1]='\0';
                   user=strdup(u), pwd=strdup(pw);
                }
                break;
      case 's': sep=optarg[0]; break;
   }
   argc-=optind, argv+=optind;
   if (argc<3) usage();
   db_setport(port);
   db_sethostname(argv[0]);
   session=db_open_session();

   if (user) db_set_global_authentication(user,(pwd&&pwd[0])?pwd:NULL);
   if (db_open_database(session,argv[1])!=0)
   {
      fprintf(stderr,"Can't open database %s.\n",argv[1]);
      exit(1);
   }
   for (x=2;x<argc;x++)
   {
      strcat(query," ");
      strcat(query,argv[x]);
   }
   p=query;
   while (isspace(*p)) p++;
   if (query[strlen(query)-1]!=';') strcat(query,";");
   result=db_exec_sql(session, 1, "%s", query);
   if (result!=0)
   {
      fprintf(stderr,"Can't exec query '%s': %s.\n",p,errors[result]);
      exit(1);
   }
   if (session->currentheader)
   {
      char separator[2];
      separator[0]=sep, separator[1]='\0';
      while (rec=db_get_next_record(session))
      {
         for (x=0;x<session->currentheader->nvalues;x++)
         {
            db_sprintvalue(rec->values+x,query);
            printf("%s%s",x?separator:"",query);
         }
         printf("\n");
      }
   }
}
