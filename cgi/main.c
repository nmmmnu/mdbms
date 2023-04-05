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
#include <string.h>
#include <stdlib.h>
#include "../lib/libdbms.h"
#include "../lib/protocol.h"
#include "global.h"

int nvars=0, startmeal=0;
variable *vars=NULL;
extern FILE *yyin;
char *varlookup(char *);
void setvar(char *, char *);
db_session *globalsession=NULL;
char *conv(char *s,char *name)
{
   char data[10000],*p=data;
   while (*name)
   {
      if (*name=='+') *(p++)=' ', name++;
      else if (*name=='%')
      {
         int n;
         char num[10];
         name++;
         strncpy(num,name,2);
         sscanf(num,"%x",&n);
         name+=2;
         *(p++)=n;
      } else *(p++)=*(name++);
   }
   *p='\0';
   strcat(s,data);
}
void addvar(char *s)
{
   char *eq=strchr(s,'='), buf[10000]="";
   if (!eq) return;
   *(eq++)='\0';
   conv(buf,eq);
   setvar(strdup(s),strdup(buf));
}
void fillvars(char *par)
{
   char *where=par, *next;
   while ((next=strchr(where,'&'))!=NULL)
   { 
      *next='\0';
      addvar(where);
      where=next+1;
   }
   if (*where) addvar(where);
}
void setvariables()
{
   char s[100];
   time_t now=time(NULL);
   struct tm *tmnow=localtime(&now);
   sprintf(s,"(date) %ld",now);
   setvar(strdup("NOW"),strdup(s));
   sprintf(s,"%d %s %d",tmnow->tm_mday, months[tmnow->tm_mon], 1900+tmnow->tm_year);
   setvar(strdup("DATE"), strdup(s));
   sprintf(s,"%02d:%02d:%02d", tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);
   setvar(strdup("TIME"), strdup(s));
}
void main(int argc, char **argv)
{
   char *p, *filename;
   printf("Content-type: text/html\n\n");
   p=getenv("QUERY_STRING");
   if (p&&*p) fillvars(p); else
   {
      char parameters[10000]="", *p;
      fgets(parameters,10000,stdin);
      p=parameters+strlen(parameters)-1;
      if (*p=='\n') parameters[strlen(parameters)-1]='\0';
      fillvars(parameters);
   } 
   filename=varlookup("file");
   if (!filename[0])
   {
      printf("No file name given. Pass the parameter 'file' to specify a file.\n");
      exit(1);
   }
   if (filename[0]=='/'||strstr(filename,".."))
   {
      printf("Invalid file name.\n");
      exit(1);
   }
   yyin=fopen(filename,"r");
   if (!yyin)
   {
      printf("Can't open file '%s'.\n",filename);
      exit(1);
   }
   globalsession=db_open_session();
   setvariables();
   yyparse();
}
