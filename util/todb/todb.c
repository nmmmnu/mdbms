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
#include <unistd.h>
#include <string.h>
#include "../../lib/libdbms.h"
#include "../../lib/errors.h"

db_session *globalsession=NULL;

void dump(char *name, FILE *io)
{
   int linenum=0;
   char line[1000];
   char buffer[10000]="";
   while (fgets(line,1000,io))
   {
      char *p;
      int result, len=strlen(line);
      line[--len]='\0';
      p=line+len-1;
      while (*p&&isspace(*p)) p--;
      linenum++;
      if (!line[0]||line[0]=='#') continue;
      if (strlen(buffer)+len+2>=1000)
      {
         printf("%s:%d Line too long while parsing file\n", name, linenum);
         exit(0);
      }
      strcat(buffer," ");
      strcat(buffer,line);
      if (*p==';')
      {
         if ((result=db_exec_sql(globalsession,1,"%s",buffer))!=0)
            printf("%s:%d %s\n",name, linenum, errors[result]);
         buffer[0]='\0';
      }
   }
}
int main(int argc, char **argv)
{
   int x, result, ch, port=2224;
   char *user=NULL, *pwd=NULL, *database=NULL, *host="localhost";
   while ((ch=getopt(argc,argv,"ih:u:p:t:d:"))!=EOF) switch(ch)
   {
      case 'h': host=strdup(optarg); break;
      case 'u': user=strdup(optarg); break;
      case 'p': pwd=strdup(optarg); break;
      case 't': port=atoi(optarg); break;
      case 'd': database=strdup(optarg); break;
      case 'i': {
                   char pw[100],u[100];
                   printf("User: "); fgets(u,100,stdin);
                   printf("Pwd : "); fgets(pw,100,stdin);
                   u[strlen(u)-1]='\0';
                   pw[strlen(pw)-1]='\0';
                   user=strdup(u), pwd=strdup(pw);
                }
                break;
   }
   argc-=optind, argv+=optind;

   db_sethostname(host);
   db_setport(port);
   globalsession=db_open_session();

   if (database) if (result=db_open_database(globalsession,database))
   {
      printf("Can't open database %s:%s\n", database, errors[result]);
      exit(1);
   }
   if (user) if (db_set_global_authentication(user, pwd))
   {
      printf("Invalid user/password\n");
      exit(1);
   }

   if (argc==0) dump("stdin", stdin); else
   for (x=0;x<argc;x++)
   {
      FILE *io=fopen(argv[x],"r");
      if (!io) perror("open"); else
      {
         dump(argv[x], io);
         fclose(io);
      }
   }
}
