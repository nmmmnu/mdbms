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

%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <syslog.h>
#include "nodes.h"
#include "global.hh"
#include "support.h"
#include "y.tab.h"
#include "lexyacc.h"
int lines;

void parsedate(struct tm *timeptr);
void parsetime(struct tm *timeptr);
char *parsestring(char *s);
%}

identifier		([a-zA-Z_][a-zA-Z0-9_]*)
number			([0-9]+)
float			([0-9]*\.[0-9]+)
string			(\'(([^\'])|(\\.))*\')
month			(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)
date			({number}[ ]+{month}[ ]+{number})
time			({number}\:{number}(\:{number})?([ ]*([AP]M))?)
timedate		({date}[ ]+{time})
money			(\$[ ]*[+-]?{number}(\.{number})?)

%%
all			return ALL;
and			return AND;
as			return AS;
ascending		return ASCENDING;
authorization		return AUTHORIZATION;
avg			return AVG;
between			return BETWEEN;
by			return BY;
cascade			return CASCADE;
char			return CHAR;
count			return COUNT;
create			return CREATE;
database		return DATABASE;
date			return DATE;
delete			return DELETE;
descending		return DESCENDING;
drop			return DROP;
exists			return EXISTS;
float			return FLOAT;
foreign			return FOREIGN;
from			return FROM;
grant			return GRANT;
group			return GROUP;
having			return HAVING;
help			return HELP;
in			return IN;
insert			return INSERT;
integer			return INTEGER;
into			return INTO;
is			return IS;
key			return KEY;
like			return LIKE;
max			return MAX;
min			return MIN;
money			return MONEY;
null			return TNULL;
not			return NOT;
on			return ON;
open			return OPEN;
option			return OPTION;
or			return OR;
order			return ORDER;
password		return PASSWORD;
primary			return PRIMARY;
privileges		return PRIVILEGES;
references		return REFERENCES;
restrict		return RESTRICT;
revoke			return REVOKE;
rownum			return ROWNUM;
schema			return DATABASE;
select			return SELECT;
set			return SET;
sum			return SUM;
sync			return SYNC;
table			return TABLE;
to			return TO;
update			return UPDATE;
user			return USER;
values			return VALUES;
varchar			return VARCHAR;
view			return VIEW;
with			return WITH;
where			return WHERE;

year			{ yylval.d_value=FUNC_YEAR; return FUNCTION;}
month			{ yylval.d_value=FUNC_MONTH; return FUNCTION;}
day			{ yylval.d_value=FUNC_DAY; return FUNCTION;}
hour			{ yylval.d_value=FUNC_HOUR; return FUNCTION;}
minute			{ yylval.d_value=FUNC_MINUTE; return FUNCTION;}
second			{ yylval.d_value=FUNC_SECOND; return FUNCTION;}
stripdate		{ yylval.d_value=FUNC_STRIPDATE; return FUNCTION;}
striptime		{ yylval.d_value=FUNC_STRIPTIME; return FUNCTION;}
dayofweek		{ yylval.d_value=FUNC_DAYOFWEEK; return FUNCTION;}
weeknum 		{ yylval.d_value=FUNC_WEEKNUM; return FUNCTION;}
"<"			return LESS;
">"			return MORE;
"<="			return LEQ;
">="			return MOQ;
"<>"			return NEQ;
"!="			return NEQ;
"("			return OPENPAREN;
")"			return CLOSEPAREN;
"*"			return STAR;
";"			return SEMICOLON;
"="			return EQUALS;
","			return COMMA;
"."			return DOT;
\+			return PLUS;
"-"			return MINUS;
"/"			return SLASH;

{date}			{
			   struct tm timeptr;
			   memset(&timeptr, 0, sizeof(timeptr));
   			   timeptr.tm_isdst=-1;
                           parsedate(&timeptr);
                           yylval.d_value=mktime(&timeptr);
			   return DATEVAL;
			}
{time}			{
			   struct tm timeptr;
			   memset(&timeptr, 0, sizeof(timeptr));
   			   timeptr.tm_isdst=-1;
			   timeptr.tm_year=2;
			   timeptr.tm_mday=1;
                           parsetime(&timeptr);
                           yylval.d_value=mktime(&timeptr);
			   return DATEVAL;
			}
{timedate}		{
			   struct tm timeptr;
			   memset(&timeptr, 0, sizeof(timeptr));
   			   timeptr.tm_isdst=-1;
                           parsedate(&timeptr);
                           parsetime(&timeptr);
                           yylval.d_value=mktime(&timeptr);
			   return DATEVAL;
			}
"(date)"[ ]*{number}	{
			   char *p=mstrchr(yytext,')')+1;
                           sscanf(p,"%lu",&yylval.d_value);
			   return DATEVAL;
			}
{money}			{
			   sscanf(yytext+1,"%lf",&yylval.f_value);
			   return MONEYVAL;
			}
{identifier}		{
			   yylval.s_value=strdup(yytext);
			   return IDENTIFIER;
			}
{string}		{
			   yylval.s_value=parsestring(yytext);
			   return strlen(yylval.s_value)==1?STRING1:STRING;
			}
{number}		{
			   yylval.i_value=atoi(yytext);
			   return NUMBER;
			}
{float}			{
			   sscanf(yytext,"%lf",&yylval.f_value);
			   return FLOATVAL;
			}
\n			lines++;
"#".*\n			lines++;
[ \t]+			;
.			return yytext[0];

%%
int yywrap(void) { return 1; }
extern int yyparse();
char *errorstrings[100];
int nerrors;
void parse(char *what)
{
   YY_BUFFER_STATE state;
/*  syslog(LOG_NOTICE,"%s",what);*/
   state=yy_scan_string(what);
   lines=1, nerrors=0;
   yyparse();
   yy_delete_buffer(state);
}
void parsedate(struct tm *timeptr)
{			   
   int day, year, mnum;
   char month[10];
   sscanf(yytext,"%d %s %d",&day,month,&year);
   for (mnum=0;mnum<12;mnum++)
      if (!strcasecmp(months[mnum],month)) break;
   timeptr->tm_mday=day, timeptr->tm_year=year-1900;
   timeptr->tm_mon=mnum, timeptr->tm_isdst=-1;
}
void parsetime(struct tm *timeptr)
{
   int a;
   char *p=mstrchr(yytext,':');
   while (!isspace(*p)&&p>yytext) p--;
   a=sscanf(p,"%d:%d:%d", &timeptr->tm_hour, &timeptr->tm_min,&timeptr->tm_sec);
   if (a==2) timeptr->tm_sec=0;
   if (timeptr->tm_hour<12&&yytext[yyleng-2]=='P') timeptr->tm_hour+=12;
}
char *parsestring(char *s)
{
   int len=strlen(s+1),pos=0;
   char *res=(char*)malloc(len), *p;
   s[len]='\0';
   for (p=s+1;*p;p++)
   {
      if (*p=='\\') continue;
      res[pos++]=*p;
   }
   res[pos++]='\0';
   return res;
}
