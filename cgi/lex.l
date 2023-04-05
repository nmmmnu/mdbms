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
#include <malloc.h>
#include <string.h>
#include "y.tab.h"
#define MAXINCL  500

extern int startmeal;
extern int linenum;
int eatlevel, filelevel=0;
YY_BUFFER_STATE includestack[MAXINCL];
char *filenames[MAXINCL];
int linenumbers[MAXINCL];
void startfile(char *);
%}

identifier		([[:alnum:]][[:alnum:]_]*)
string                  (\"([^\"]|(\\\"))*\")
float                   ([0-9]+(\.[0-9]+)?)
mdbms			("<!"[ \t]*"mdbms"[ \t]*)


%x MDBMS
%x EAT
%%
			if (startmeal)
			{
			   eatlevel=startmeal=0;
			   BEGIN(EAT);
			}

{mdbms}			BEGIN(MDBMS);
<MDBMS>">"\n		{ BEGIN(INITIAL); linenum++; return EOL; }
<MDBMS>">"		{ BEGIN(INITIAL); return EOL; }
<MDBMS>connect		return CONNECT;
<MDBMS>crypt		return CRYPT;
<MDBMS>database		return DATABASE;
<MDBMS>setdefault	return DEFAULT;
<MDBMS>exec		return EXEC;
<MDBMS>free		return FREE;
<MDBMS>if		return IF;
<MDBMS>then		return THEN;
<MDBMS>else		{
			   eatlevel=0;
			   BEGIN(EAT);
			   return ELSE;
			}
<MDBMS>fi		return FI;
<MDBMS>set		return SET;
<MDBMS>getrow		return GETROW;
<MDBMS>opentcp		return OPENTCP; 
<MDBMS>pipe             return PIPE;
<MDBMS>print		return PRINT;
<MDBMS>print_attributes	return PRINTATTR;
<MDBMS>print_header	return PRINTHEADER;
<MDBMS>print_rows	return PRINTROWS;
<MDBMS>query		return QUERY;
<MDBMS>readtcp		return READTCP;
<MDBMS>writetcp		return WRITETCP;
<MDBMS>user		return USER;
<MDBMS>"("		return OPENPAREN;
<MDBMS>")"		return CLOSEPAREN;
<MDBMS>"+"		return PLUS;
<MDBMS>"-"		return MINUS;
<MDBMS>"*"		return STAR;
<MDBMS>"/"		return SLASH;
<MDBMS>"!"		return NOT;
<MDBMS>"<"		return LESS;
<MDBMS>"<="		return LESSEQ;
<MDBMS>">>"		return MORE;
<MDBMS>">="		return MOREEQ;
<MDBMS>"!="		return NOTEQ;
<MDBMS>"=="		return EQ;
<MDBMS>"&&"		return AND;
<MDBMS>"||"		return OR;
<MDBMS>{float}		{
			   sscanf(yytext,"%lf",&yylval.f_value);
			   return NUMBER;
			}
<MDBMS>{identifier}	{
			   yylval.s_value=strdup(yytext);
			   return IDENTIFIER;
			}
<MDBMS>\${identifier}	{
			   yylval.s_value=strdup(yytext+1);
			   return VARIABLE;
			}
<MDBMS>\=		return EQUALS;
<MDBMS>{string}		{
			   yytext[strlen(yytext)-1]='\0';
			   yylval.s_value=strdup(yytext+1);
			   return STRING;
			}
<MDBMS>\n		linenum++;
<MDBMS>.		;
<EAT>\n			linenum++;
<EAT>.			;
<EAT>{string}		;
<EAT>{mdbms}"if"	eatlevel++;
<EAT>{mdbms}"fi"[ /t]*	{
			   if (eatlevel==0)
			   {
			      BEGIN(MDBMS);
			      return FI;
			   }
			   eatlevel--;
			}
<EAT>{mdbms}"else"[ /t]*">"	{
				   if (eatlevel==0)
				   {
				      BEGIN(INITIAL);
				      return ELSE;
				   }
				}
<MDBMS>"include"[ /t]*{string}[ /t]*">"	 {
				   char *p=strchr(yytext,'\"');
				   char *p2=strchr(p+1,'\"');
				   *p2='\0';
				   startfile(p+1);
				   BEGIN(INITIAL);
				}
<<EOF>>			{
			   if (--filelevel<0) yyterminate(); else
			   {
			      free(filenames[filelevel]);
			      linenum=linenumbers[filelevel];
			      yy_delete_buffer(YY_CURRENT_BUFFER);
			      yy_switch_to_buffer(includestack[filelevel]);
			   }
			   
			}


{string}		ECHO;
\n			linenum++; ECHO;
.			ECHO;
#.*\n			linenum++;
%%
int yywrap(void)
{
    return 1;
}
void startfile(char *s)
{
   FILE *io;
   if (filelevel==MAXINCL)
   {
      printf("Includes nested too deep.\n");
      return;
   }
   io=fopen(s,"r");
   if (!io)
   { 
      printf("Can't open file '%s'.\n",s);
      return;
   }
   includestack[filelevel]=YY_CURRENT_BUFFER;
   filenames[filelevel]=strdup(s);
   linenumbers[filelevel]=linenum;
   filelevel++, linenum=1;
   yy_switch_to_buffer(yy_create_buffer(yyin=io, YY_BUF_SIZE));
   BEGIN(INITIAL);
}
