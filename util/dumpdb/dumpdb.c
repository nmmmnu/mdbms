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
#include "../../lib/protocol.h"
#include "../../lib/libdbms.h"

db_session *globalsession;
void usage()
{
   printf("Usage: dumpdb [-u user] [-p pwd] [ -t port] [-i] <hostname> <database> [table]\n");
   printf("  -u : specify user\n");
   printf("  -p : specify password\n");
   printf("  -t : specify TCP port to connect to\n");
   printf("  -i : prompt for user/password\n");
   exit(0);
}
void createtable(char *name)
{
   char cont[1000]="", primary[1000]="", primaryclause[1000]="";
   int x;
   tabledef *def=db_get_table_def(globalsession,name);
   if (!def)
   {
      printf("Cant open table '%s'\n",name);
      return;
   }
   for (x=0;x<def->nattributes;x++)
   {
      char part[100], type[100];
      switch (def->attributes[x].type)
      {
         case ATT_INT: sprintf(type,"integer"); break;
         case ATT_FLOAT: sprintf(type,"float"); break;
         case ATT_MONEY: sprintf(type,"money"); break;
         case ATT_CHAR: sprintf(type,"char"); break;
         case ATT_DATE: sprintf(type,"date"); break;
         case ATT_VARCHAR: sprintf(type,"varchar(%d)",def->attributes[x].size);
                           break;
      }
      if (def->attributes[x].nulls==0) strcat(type," not null");
      sprintf(part,"%s%s %s", cont[0]?", ":"", def->attributes[x].name,
         type);
      strcat(cont, part);
      if (def->attributes[x].primary)
      {
         if (primary[0]) strcat(primary,", ");
         strcat(primary,def->attributes[x].name);
      }
   }
   if (primary[0]) sprintf(primaryclause,"primary key (%s)",primary);
   printf("create table %s (%s %s",name,cont,primaryclause);
   for (x=0;x<def->nfkeys;x++)
   {
      int y, action;
      primary[0]='\0';
      for (y=0;y<def->nattributes;y++) if (def->attributes[y].foreign==x)
      {
         if (primary[0]) strcat(primary,", ");
         strcat(primary,def->attributes[y].name);
      }
      action=def->fkeys[x].action;
      printf(" foreign key (%s) references %s on delete %s", primary,
         def->fkeys[x].identifies, action==0?"restrict":
         action==1?"cascade":"set null");
   }
   printf(");\n");
}
void fixquotes(char *buf)
{
   char temp[1000], *p=buf, *w;
   strcpy(temp,buf);
   for (w=temp;*w;w++) if (*w!='\'') *(p++)=*w; else
   {
      *(p++)='\\';
      *(p++)=*w;
   }
   *w='\0';
}
void createdata(char *name)
{
   record *data;
   while (data=db_get_next_record(globalsession))
   {
      int x;
      char cont[10000]="";
      for (x=0;x<data->nvalues;x++)
      {
         char part[1000],buf[1000];
         db_sprintvalue(data->values+x,buf);
         fixquotes(buf);
         if (data->values[x].type==ATT_CHAR||data->values[x].type==ATT_VARCHAR)
            sprintf(part,"%s'%s'",cont[0]?", ":"",buf);
         else if (data->values[x].type==ATT_NULL)
            sprintf(part,"%sNULL",cont[0]?", ":"");
         else sprintf(part,"%s%s",cont[0]?", ":"",buf);
         strcat(cont,part);
      }
      printf("insert into %s values (%s);\n",name,cont);
      db_free_record(data);
   }
}
void dumptable(char *tabname)
{
   char query[100];
   int result;
   record header;
   createtable(tabname);
   if ((result=db_exec_sql(globalsession,1,"select * from %s;",tabname))!=0)
   {
      fprintf(stderr,"Can't read table %s: %s.\n",tabname,errors[result]);
      return;
   }
   createdata(tabname);
}
int main(int argc, char **argv)
{
   int ch, port=2224;
   char *user=NULL, *pwd=NULL;
   while ((ch=getopt(argc,argv,"u:p:t:i"))!=EOF) switch(ch)
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
   }
   argc-=optind, argv+=optind;
   if (argc!=2&&argc!=3) usage();

   db_setport(port);
   db_sethostname(argv[0]);
   globalsession=db_open_session();
   if (db_open_database(globalsession,argv[1])!=0)
   {
      fprintf(stderr,"Can't open database '%s'\n",argv[1]);
      exit(1);
   }
   if (user)
   {
      if (db_set_global_authentication(user,(pwd&&pwd[0])?pwd:NULL)!=0)
      {
         printf("Invalid user/password\n");
         exit(1);
      }
   }
   if (argc==3) dumptable(argv[2]); else
   {
      char *tblnames[1000], query[1000];
      int anames=0,x;
      record *rec;
      db_open_database(globalsession,"system");
      db_exec_sql(globalsession,1,"select tblname from tables where dbname='%s'",
         argv[1]);
      while (rec=db_get_next_record(globalsession))
      {
         tblnames[anames++]=strdup(rec->values[0].value.string);
         db_free_record(rec);
      }
      db_open_database(globalsession,argv[1]);
      for (x=0;x<anames;x++) dumptable(tblnames[x]);
   }
}
