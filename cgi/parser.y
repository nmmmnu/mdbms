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
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdarg.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "../lib/libdbms.h"
#include "../lib/errors.h"
#include "../lib/protocol.h"
#include "global.h"

void yyerror(char *);
void doerror(char *s, ...);
char *varlookup(char *s);
void setvar(char *, char *);
char *calc(char *, char *, int );
#include <stdarg.h>
char *printstring(char *);
int dofree(char *);
void opendb(char *);
void doquery(char *, char *);
void printrows(char *, char *);
void printattr(char *, char *, char *, char *);
void printheader(char *, char *);
int expr(char *, char *, int);
void printnum(char *, double);
void nextrow(char *);
void dodefault(char *, char *);
int doexec(char *,int);
void dopipe(char *, char *);
void docrypt(char *);
void dosetuser(char *);
void doopentcp(char *, char *, int);
void doreadtcp(char *, char *);
void dowritetcp(char *, char *);
int dosethostname(char *);
extern int startmeal;
extern char *yytext;
extern int *linenumbers;
extern char *filenames[];
extern int filelevel;
int linenum=1;
int connected=0;

query *rootquery=NULL;
connection *rootconnection=NULL;

%}


%union
{
   char *s_value;
   double f_value;
   int i_value;
}

%token CONNECT SET EQUALS PRINT OPENPAREN CLOSEPAREN QUERY DATABASE FREE THEN
%token PRINTROWS NOT AND OR EQ NOTEQ LESS LESSEQ MORE MOREEQ IF ELSE FI EOL
%token GETROW EXEC CRYPT USER PRINTATTR PRINTHEADER DEFAULT PIPE OPENTCP
%token READTCP WRITETCP
%left PLUS MINUS
%left STAR SLASH
%token <s_value> VARIABLE STRING IDENTIFIER
%token <f_value> NUMBER

%type <s_value> expression literal value
%type <i_value> single_condition condition

%%


start			:
			| statement_list
			;
statement_list		: statement EOL
			| statement_list statement EOL
			;
statement		: connect_statement
			| set_statement
			| print_statement
			| printrows_statement
			| printattr_statement
			| printheader_statement
			| query_statement
			| database_statement
			| free_statement
			| if_statement
			| nextrow_statement
			| exec_statement
			| pipe_statement
			| crypt_statement
			| user_statement
			| default_statement
			| opentcp_statement
			| readtcp_statement
			| writetcp_statement
			;
connect_statement	: CONNECT
			   { connected=dosethostname("localhost"); }
			| CONNECT STRING
			   { connected=dosethostname($2); free($2); }
			| CONNECT STRING NUMBER
			   { db_setport((int)$3);
			       connected=dosethostname($2); free($2); }
			;
database_statement	: DATABASE IDENTIFIER
			  {
			     opendb($2);
			     free($2);
			  };
free_statement		: FREE IDENTIFIER
			  {
			     if (!dofree($2))
			        doerror("No query '%s'<BR>\n",$2);
			     free($2);
			  }
			;
if_statement		: IF condition EOL THEN EOL
			    { if ($2==0) { startmeal=1; yyclearin; } }
			  if_part2
			;
if_part2		: start FI
			| start ELSE start FI
			;

print_statement		: PRINT expression
			  {
			     char *v=printstring($2);
			     printf("%s\n",v);
			     free(v);
			  }
			;
printrows_statement	: PRINTROWS IDENTIFIER STRING
			  {
			     printrows($2, $3);
			     free($2);
			     free($3);
			  }
			;
printattr_statement	: PRINTATTR IDENTIFIER STRING STRING STRING
			   {
			      printattr($2,$3,$4,$5);
			      free($2);
			      free($3);
			      free($4);
			      free($5);
			   }
			;
printheader_statement	: PRINTHEADER IDENTIFIER STRING
			   {
			      printheader($2, $3);
			      free($2);
			      free($3);
			   }
nextrow_statement	: GETROW IDENTIFIER
			   {
			      nextrow($2);
			      free($2);
			   }
			;
set_statement		: SET VARIABLE EQUALS expression
			   { setvar($2,$4); }
			;
crypt_statement		: CRYPT VARIABLE
			   { docrypt($2); }
			;
query_statement		: QUERY STRING IDENTIFIER
			   { doquery($2,$3); }
			| QUERY VARIABLE IDENTIFIER
			   {
			      char *s=printvar($2);
			      free($2);
			      s=strdup(s?s:"");
			      doquery(s,$3);
			   }
			;
default_statement	: DEFAULT IDENTIFIER STRING
			   {
			      dodefault($2,$3);
			      free($2); free($3);
			   }
			;
exec_statement		: EXEC STRING
			   {
			      doexec($2,0);
			   }
			| EXEC VARIABLE EQUALS STRING
			   {
			      char cont[10];
			      sprintf(cont,"%d",doexec($4,1));
			      setvar($2,strdup(cont));
			   }
			;
pipe_statement		: PIPE VARIABLE EQUALS STRING
			   {
			      dopipe($2, $4);
			   }

opentcp_statement	: OPENTCP IDENTIFIER EQUALS STRING STRING
			   {
			      int port;
			      if (sscanf($5, "%d")!=1)
			      {
			         doerror("OPENTCP: Invalid port: %s", $5);
			         free($2);
			      } else doopentcp($2, $4, port);
			      free($4);
			      free($5);
			   }
			| OPENTCP IDENTIFIER EQUALS STRING NUMBER
			   {
			      doopentcp($2, $4, (int)$5);
			      free($4);
			   }
			;
readtcp_statement	: READTCP IDENTIFIER VARIABLE
			   {
			      doreadtcp($3, $2);
			      free($2);
			      free($3);
			   }
			;
writetcp_statement	: WRITETCP IDENTIFIER STRING
			   {
			      dowritetcp($3, $2);
			      free($2);
			      free($3);
			   }
			;

condition		: single_condition
			| single_condition AND condition
			   { $$=$1&&$3; }
			| single_condition OR condition
			   { $$=$1||$3; }
			| NOT single_condition
			   { $$=!$2; }
			;
single_condition	: expression EQ expression
			   { $$=expr($1, $3, EQ); }
			| expression LESS expression
			   { $$=expr($1, $3, LESS); }
			| expression MORE expression
			   { $$=expr($1, $3, MORE); }
			| expression LESSEQ expression
			   { $$=expr($1, $3, LESSEQ); }
			| expression MOREEQ expression
			   { $$=expr($1, $3, MOREEQ); }
			| expression NOTEQ expression
			   { $$=expr($1, $3, NOTEQ); }
			| OPENPAREN condition CLOSEPAREN
			   { $$=$2; }
			;

expression		: value
			| expression PLUS expression
			   { $$=calc($1,$3,PLUS); }
			| expression MINUS expression
			   { $$=calc($1,$3,MINUS); }
			| expression STAR expression
			   { $$=calc($1,$3,STAR); }
			| expression SLASH expression
			   { $$=calc($1,$3,SLASH); }
			| OPENPAREN expression CLOSEPAREN
			   { $$=$2; }
			;
value			: literal
			| VARIABLE
			  {
			     $$=varlookup($1);
			     free($1);
			  }
			;
literal			: STRING
			  {
			     $$=printstring($1);
			     free($1);
			  }
			| NUMBER
			  {
			     char buf[100];
			     printnum(buf,$1);
			     $$=strdup(buf);
			  }
			;
user_statement		: USER STRING
			   {
			       dosetuser($2);
			       free($2);
			   } 
			;

%%
void yyerror(char *s)
{
   printf("<table bgcolor=\"#ff0000\"><tr><td>");
   printf("File %s, Line %03d, Near '%s': %s\n",filelevel?
      filenames[filelevel-1]:varlookup("file"),linenum,yytext,s);
   printf("</td></tr></table>");
}
void doerror(char *s, ...)
{
   char buf[10000];
   va_list ap;
   va_start(ap,s);
   vsprintf(buf,s,ap);
   va_end(ap);
   yyerror(buf);
}
int getvar(char *name)
{
   int x;
   for (x=0;x<nvars;x++) if (!strcasecmp(name,vars[x].name))
      return x;
   return -1;
}
void printnum(char *buf, double num)
{
   if (floor(num)==num) sprintf(buf,"%d",(int)num); else
      sprintf(buf,"%lf",num);
}
char *varlookup(char *s)
{
   int x=getvar(s);
   if (x!=-1) return strdup(vars[x].cont);
   return strdup("");
}
void setvar(char *name, char *cont)
{
   int x=getvar(name);
   if (x==-1)
   { 
      nvars++;
      vars=(variable*)realloc(vars,sizeof(variable)*nvars);
      vars[nvars-1].name=name;
      vars[nvars-1].cont=cont;
   } else
   { 
      free(vars[x].cont);
      vars[x].cont=cont;
      free(name);
   } 
}
char *printvar(char *s)
{
   int x=getvar(s);
   if (x!=-1) return(vars[x].cont);
   return NULL;
}
void checksize(char **buf, int n, int *max)
{
   if (n>*max) *buf=(char*)realloc(*buf,*max=n);
}
char *printstring(char *s)
{
   char *res=(char*)calloc(1000,1);
   char varname[100]="", *backup=s;
   int mode=0, p, len=0, max=1000;
   for (;*s;s++) if (mode==0)
   {
      if (*s=='$') varname[0]='\0', p=0, mode=1; else
      {
         if (*s=='\\'&&s[1]=='\"') continue;
         checksize(&res, len+2, &max);
         res[len++]=*s;
         res[len]='\0';
      }
   } else
   {
      if (isalnum(*s)||(*s=='_'))
      {
         varname[p++]=*s;
         varname[p]='\0';
      } else
      {
         char *var=*varname?printvar(varname):"$";
         if (var)
         {
            checksize(&res, 1+len+strlen(var), &max);
            strcat(res, var);
            len+=strlen(var);
         }
         if (*s!='\\'||s[1]!='\"') 
         {
            checksize(&res, len+2, &max);
            res[len++]=*s;
            res[len]='\0';
         }
         varname[0]='\0', mode=0;
      }
   }
   if (varname[0])
   {
      char *var=printvar(varname);
      if (var)
      {
         checksize(&res, 1+len+strlen(var), &max);
         strcat(res, var);
         len+=strlen(var);
      }
   }
   checksize(&res, len+1, &max);
   res[len++]='\0';
   return res;
   free(backup);
}
char *calc(char *s1, char *s2, int code)
{
   char buf[1000];
   double num1, num2;
   int ok1, ok2;
   ok1=sscanf(s1,"%lf",&num1)==1;
   ok2=sscanf(s2,"%lf",&num2)==1;
   if ((code==SLASH||code==STAR||code==MINUS)&&(!ok1||!ok2))
   {
      free(s1);
      free(s2);
      return strdup("");
   }
   switch(code)
   {
      case SLASH: printnum(buf,num1/num2);
                  free(s1); free(s2); return strdup(buf);
      case STAR : printnum(buf,num1*num2);
                  free(s1); free(s2); return strdup(buf);
      case MINUS: printnum(buf,num1-num2);
                  free(s1); free(s2); return strdup(buf);
      case PLUS:  if (ok1&&ok2)
                  {
                     printnum(buf,num1+num2);
                     free(s1); free(s2); return strdup(buf);
                  } else
                  {
                     char *b=(char*)malloc(strlen(s1)+strlen(s2)+1);
                     strcpy(b,s1); strcat(b,s2); return b;
                  }
   }
}
void doquery(char *s, char *name)
{
   int result;
   char num[100], *q;
   record *r;
   query *newq;
   if (!connected)
   {
      doerror("Not connected to database.\n");
      return;
   }
   q=printstring(s);
   newq=(query*)calloc(1,sizeof(query));
   newq->next=rootquery;
   if (newq->next) newq->next->prev=newq;
   newq->name=name;
   rootquery=newq;
   newq->defaultindex=-1;
   if ((result=db_exec_sql(globalsession,1,"%s",q))!=0)
   {
      doerror("%s: %s",q,result==-1?"Not connected":errors[result]);
      free(q);
      return;
   }
   if (globalsession->dataquery)
   {
      int x;
      record *h=globalsession->currentheader;
      newq->header.nvalues=h->nvalues;
      newq->header.values=(value *)calloc(h->nvalues,sizeof(value));
      for (x=0;x<h->nvalues;x++)
         newq->header.values[x].value.string=strdup(h->values[x].value.string);
      while (r=db_get_next_record(globalsession))
      {
         newq->nrecords++;
         if (!newq->records) newq->records=(record**)malloc(sizeof(record));
            else newq->records=(record**)realloc(newq->records,sizeof(record*)*newq->nrecords);
         newq->records[newq->nrecords-1]=r;
      }
   }
   sprintf(num,"%d",newq->nrecords);
   setvar(strdup("NUM_ROWS"),strdup(num));
   free(q);
}
query *getquery(char *name)
{
   query *q;
   for (q=rootquery;q;q=q->next) if (!strcmp(q->name,name))
      return q;
   return NULL;
}
connection *getconnection(char *name)
{
   connection *c;
   for (c=rootconnection;c;c=c->next) if (!strcmp(c->name, name))
      return c;
   return NULL;
}
int dofree(char *name)
{
   int x;
   query *q=getquery(name);
   if (!q) return 0;
   if (q->next) q->next->prev=q->prev;
   if (q->prev) q->prev->next=q->next; else rootquery=q->next;
   free(q->name);
   for (x=0;x<q->nrecords;x++) db_free_record(q->records[x]);
   for (x=0;x<q->header.nvalues;x++) free(q->header.values[x].value.string);
   if (q->records) free(q->records);
   return 1;
}
void opendb(char *name)
{
   if (!connected)
   {
      doerror("Not connected to database.\n");
      return;
   }
   if (db_open_database(globalsession,name))
      doerror("Can't open database '%s'",name);
}
void setnext(query *q, int x)
{
   int y, amount;
   char buffer[10000];
   record *thisr;
   if (!q) return;
   if (x>=q->nrecords) thisr=NULL, amount=255;
      else thisr=q->records[x], amount=thisr->nvalues;
   for (y=0;y<amount;y++)
   {
      char varname[10], *name, *value;
      sprintf(varname,"%d", y);
      name=strdup(varname);
      value=strdup(thisr?db_sprintvalue(thisr->values+y,buffer):"");
      setvar(name, value);
   }
}
void nextrow(char *n)
{
   query *q=getquery(n);   
   if (q) setnext(q,q->counter++);
}
void printrows(char *qn, char *s)
{
   char *res;
   int x;
   query *q=getquery(qn);   
   if (!q) return;
   if (q->defaultindex!=-1)
   {
      setnext(q,q->defaultindex);
      res=printstring(s);
      printf("%s\n",res);
      free(res);
   }
   for (x=0;x<q->nrecords;x++)
   { 
      if (x==q->defaultindex) continue;
      setnext(q,x);
      res=printstring(s);
      printf("%s\n",res);
      free(res);
   }
}
void printattr(char *qn, char *pre, char *mid, char *post)
{
   char *printpre, *printpost, buffer[10000], *res;
   int x, y;
   query *q=getquery(qn);   
   if (!q) return;
   printpre=printstring(pre);
   printpost=printstring(post);
   if (q->defaultindex!=-1)
   {
      record *thisr=q->records[q->defaultindex];
      printf("%s",printpre);
      for (y=0;y<thisr->nvalues;y++)
      {
         char *name, *value;
         name=strdup("0");
         value=strdup(db_sprintvalue(thisr->values+y,buffer));
         setvar(name, value);
         res=printstring(mid);
         printf("%s",res);
         free(res);
      }
      printf("%s\n",printpost);
   }
   for (x=0;x<q->nrecords;x++)
   { 
      record *thisr=q->records[x];
      if (x==q->defaultindex) continue;
      printf("%s",printpre);
      for (y=0;y<thisr->nvalues;y++)
      {
         char *name, *value;
         name=strdup("0");
         value=strdup(db_sprintvalue(thisr->values+y,buffer));
         setvar(name, value);
         res=printstring(mid);
         printf("%s",res);
         free(res);
      }
      printf("%s\n",printpost);
   }
   free(printpre);
   free(printpost);
}
void printheader(char *qn, char *string)
{
   int y;
   char buffer[10000], *res;
   query *q=getquery(qn);
   record *h;
   if (!q) return;
   h=&q->header;
   for (y=0;y<h->nvalues;y++)
   {
      char *name, *value;
      name=strdup("0");
      value=strdup(h->values[y].value.string);
      setvar(name, value);
      res=printstring(string);
      printf("%s\n",res);
      free(res);
   }
}
int expr(char *s1, char *s2, int type)
{
   double v1, v2;
   int ok1, ok2, res;
   ok1=sscanf(s1,"%lf",&v1)==1;
   ok2=sscanf(s2,"%lf",&v2)==1;
   if (!ok1||!ok2) res=strcmp(s1,s2); else
      res=v1<v2?-1:v1>v2?1:0;
   if (res<0) res=-1; else if (res>1) res=1;
   switch (type)
   {
      case EQ    : return res==0;
      case NOTEQ : return res!=0;
      case MORE  : return res==1;
      case LESS  : return res==-1;
      case LESSEQ: return res<=0;
      case MOREEQ: return res>=0;
   }
}
#define ascii_to_bin(c) ((c)>='a'?(c-59):(c)>='A'?((c)-53):(c)-'.')
#define bin_to_ascii(c) ((c)>=38?((c)-38+'a'):(c)>=12?((c)-12+'A'):(c)+'.')
char *mcrypt(char *, char*);
void docrypt(char *name)
{
   int x=getvar(name);
   char salt[2], *cryptstr;
   free(name);
   if (x==-1) return;
   salt[0] = bin_to_ascii(0x21);
   salt[1] = bin_to_ascii(0x12);
   cryptstr = mcrypt(vars[x].cont, salt);
   free(vars[x].cont);
   vars[x].cont=strdup(cryptstr);
}
void dodefault(char *identifier, char *string)
{
   int x;
   char buffer[10000], *s;
   record *thisr;
   query *q=getquery(identifier);   
   if (!q) return;
   s=printstring(string);
   for (x=0;x<q->nrecords;x++)
   {
      thisr=q->records[x];
      db_sprintvalue(thisr->values,buffer);
      if (!strcmp(s,buffer))
      {
         q->defaultindex=x;
         free(s);
         return;
      }
   }
   free(s);
}
int doexec(char *s, int needreturn)
{
   int a=0;
   char line[10000];
   FILE *io;
   char *str=printstring(s);
   free(s);
   if (needreturn)
   {
      a=system(str);
   } else
   {
      io=popen(str,"r");
      if (!io) doerror("Can't exec '%s'",str); else
      {
         while (fgets(line,10000,io))
         { 
            printf("%s",line);
         }
         pclose(io);
      }
   }
   free(str);
   return a;
}
void dopipe(char *var, char *s)
{
   FILE *io;
   int max;
   char *data=NULL, *str=printstring(s);
   free(s);
   io=popen(str, "r");
   if (!io) doerror("Can't exec '%s'", str); else
   {
      char line[1000], totallines=0;
      if (!data)
      {
         data=(char*)malloc(max=100);  
         data[0]='\0';
      }
      while (fgets(line, 1000, io))
      {
         int need=strlen(line)+strlen(data)+1;
         if (need>max) data=(char*)realloc(data, max=need);
         strcat(data, line);
         totallines++;
      }
      pclose(io);
      if (totallines==1)
      {
         char *pos=strlen(data)+data-1;
         while (*pos=='\n'||*pos=='\r') *(pos--)='\0';
      }
      setvar(var, data);
   }
   free(str);
}
void dosetuser(char *file)
{
   char user[1000],pwd[1000];
   FILE *io=fopen(file,"r");
   if (!io)
   {
      doerror("Could not open user file '%s'.",file);
      return;
   }
   if (fscanf(io,"user %s password %s",user,pwd)!=2)
   {
      doerror("Incorrect format.");
      fclose(io);
      return;
   }
   if (db_set_global_authentication(user,pwd))
   {
      doerror("Invalid user/password for user '%s'.",user);
      fclose(io);
      return;
   }
   fclose(io);
}
int dosethostname(char *s)
{
   int sock;
   db_sethostname(s);
   sock=db_opensocket();
   if (sock==-1)
   {
      doerror("Can't connect to server at '%s' port %d",s,db_server_port);
      return 0;
   }
   close(sock);
   return 1;
}
void doopentcp(char *var, char *host, int port)
{
   int fd;
   struct sockaddr_in sin;
   char *hostname=printstring(host);
   struct connection *cn;
   struct hostent *hent;
   if (getconnection(var))
   {
      doerror("OPENTCP: Connection '%s' already exists", var);
      free(var);
      return;
   }
   hent=gethostbyname(hostname);
   if (!hent)
   {
      doerror("OPENTCP: Cannot resolve hostname '%s'", hostname);
      free(hostname);
      free(var);
      return;
   }
   fd=socket(AF_INET, SOCK_STREAM, 0);
   if (fd==-1)
   {
      doerror("OPENTCP: Cannot create socket: %s", hostname,
         strerror(errno));
      free(hostname);
      free(var);
      return;
   }
   sin.sin_family=AF_INET;
   sin.sin_port=htons(port);
   memcpy((char*)&sin.sin_addr, (char*)hent->h_addr, hent->h_length);
   if (connect(fd, (struct sockaddr*) &sin, sizeof(sin))==-1)
   {
      doerror("OPENTCP: Cannot connect to %s:%d", hostname, port);
      close(fd);
      free(hostname);
      free(var);
      return;
   }
   cn=(connection*)malloc(sizeof(connection));
   cn->next=rootconnection, cn->prev=NULL;
   cn->name=var;
   if (cn->next) cn->next->prev=cn;
   cn->fd=fd;
   rootconnection=cn;
   free(hostname);
}
void doreadtcp(char *var, char *handle)
{
   connection *c=getconnection(handle);
   int bytes;
   char buf[102401];
   fd_set rmask;
   struct timeval timeout={10, 0};
   if (!c)
   {
      doerror("READTCP: No such connection '%s'", handle);
      return;
   }
   FD_ZERO(&rmask);
   FD_SET(c->fd, &rmask);
   if (select(c->fd+1, &rmask, NULL, NULL, &timeout)!=1)
   {
      doerror("READTCP: No data received after 10 seconds");
      return;
   }
   bytes=read(c->fd, buf, 102400);
   if (bytes==-1) buf[0]='\0';
      else buf[bytes]='\0';
   setvar(strdup(var), strdup(buf));
}
void dowritetcp(char *string, char *handle)
{
   connection *c=getconnection(handle);
   char *s, cr[]="\r\n";
   if (!c)
   {
      doerror("WRITETCP: No such connection '%s'", handle);
      return;
   }
   s=printstring(string);
   write(c->fd, s, strlen(s));
   write(c->fd, cr, 2);
   free(s);
}
