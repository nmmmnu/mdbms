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
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "global.hh"
#include "support.h"

char *months[]=
{
   "jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"
};
char *funcnames[]=
{
   "", "avg","count","max","min","sum","year","month","day","hour","minute",
   "second","date","stripdate","striptime"
};
char *attnames[]=
{
   "",
   "integer", "float", "money", "char", "varchar", "date", "blob", "", "",
   "null"
};
char *grantnames[]=
{
   "SELECT", "INSERT", "DELETE", "UPDATE", "DROP", "GRANT SELECT",
   "GRANT INSERT", "GRANT DELETE", "GRANT UPDATE", "GRANT DROP"
};
void seterror(int no, char *con)
{
   if (dbcontext) free(dbcontext);
   dbcontext=con?strdup(con):(char*)NULL;
   dberrno=no;
}
void dberror(char *s)
{
   fprintf(stderr,"%s: %s\n",s,errors[dberrno]);
}
char *serror(char *s)
{
   s[0]='\0';
   if (dbcontext) sprintf(s,"%s: ",dbcontext);
   strcat(s,errors[dberrno]);
   strcat(s,".\n");
   return s;
}
char *mstrchr(char *s, int num)
{
   for (;*s;s++) if (*s==(char)num) return s;
   return NULL;
}
#define ascii_to_bin(c) ((c)>='a'?(c-59):(c)>='A'?((c)-53):(c)-'.')
#define bin_to_ascii(c) ((c)>=38?((c)-38+'a'):(c)>=12?((c)-12+'A'):(c)+'.')
extern "C"
{
char *mcrypt(char *, char *);
}
void docrypt(char *data)
{
   char salt[2], *cryptstr;
   salt[0] = bin_to_ascii(0x21);
   salt[1] = bin_to_ascii(0x12);
   cryptstr = mcrypt(data, salt);
   strcpy(data,cryptstr);
}

char *getversion()
{
   int fd=open(METAFILE,O_RDONLY);
   if (fd==-1) return NULL;
   char name[50];
   read(fd,name,50);
   close(fd);
   if (name[0]!='V') return NULL;
   return strdup(name);
}

int getweekno(struct tm *tm)
{
   time_t ref;
   int yearday=tm->tm_yday;
   tm->tm_sec=tm->tm_min=tm->tm_hour=0;
   tm->tm_mon=0, tm->tm_mday=1;
   ref=mktime(tm);
   tm=localtime(&ref);
   yearday+=tm->tm_wday;
   return 1+(yearday/7);
}
