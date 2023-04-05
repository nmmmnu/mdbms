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

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "global.hh"
#include "manage.hh"

manage::manage()
{
   nvars=0, variables=NULL;
}
manage::~manage()
{
   int x;
   for (x=0;x<nvars;x++) if (variables[x].name)
   {
      free(variables[x].name);
      if (variables[x].type==ATT_VARCHAR) free(variables[x].value.string);
   }
   free(variables);
}

int manage::addvar(char *name, int type)
{
   int x, num=-1;
   for (x=0;x<nvars;x++) if (variables[x].name==NULL)
   {
      num=x;
      break;
   }
   if (num==-1)
   {
      num=nvars;
      if (variables) variables=(managevar*)realloc(variables,sizeof(managevar)*++nvars); else
        variables=(managevar*)malloc(sizeof(managevar)*++nvars);
   }
   memset(&variables[num], 0, sizeof(managevar));
   variables[num].name=strdup(name);
   variables[num].type=type;
   return num;
}
void manage::delvar(int num)
{
   if (variables[num].name) free(variables[num].name);
   variables[num].name=NULL;
}
void manage::incvar(int num)
{
   variables[num].value.number++;
}
void manage::decvar(int num)
{
   variables[num].value.number--;
}
void manage::setvar(int num, int val)
{
   variables[num].value.number=val;
}
void manage::setvar(int num, char *string)
{
   if (variables[num].value.string) free(variables[num].value.string);
   variables[num].value.string=string?strdup(string):(char*)NULL;
}
managevar *manage::getvar(int num)
{
   if (num>=nvars) return NULL;
   return variables+num;
}
int manage::getnvars()
{
   return nvars;
}
char *manage::sprintvalue(int num, char *buf)
{
   if (num>=nvars||variables[num].name==NULL) return NULL;
   switch (variables[num].type)
   {
      case ATT_VARCHAR: sprintf(buf,"\"%s\"",variables[num].value.string); break;
      case ATT_INT    : sprintf(buf,"%d",variables[num].value.number); break;
   }
   return buf;
}
int manage::getvarnum(char *name)
{
   int x;
   for (x=0;x<nvars;x++) if (variables[x].name)
      if (!strcmp(variables[x].name,name)) return x;
   return -1;
}
