#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 20 "parser.y"
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

#line 77 "parser.y"
typedef union
{
   char *s_value;
   double f_value;
   int i_value;
} YYSTYPE;
#line 74 "y.tab.c"
#define CONNECT 257
#define SET 258
#define EQUALS 259
#define PRINT 260
#define OPENPAREN 261
#define CLOSEPAREN 262
#define QUERY 263
#define DATABASE 264
#define FREE 265
#define THEN 266
#define PRINTROWS 267
#define NOT 268
#define AND 269
#define OR 270
#define EQ 271
#define NOTEQ 272
#define LESS 273
#define LESSEQ 274
#define MORE 275
#define MOREEQ 276
#define IF 277
#define ELSE 278
#define FI 279
#define EOL 280
#define GETROW 281
#define EXEC 282
#define CRYPT 283
#define USER 284
#define PRINTATTR 285
#define PRINTHEADER 286
#define DEFAULT 287
#define PIPE 288
#define OPENTCP 289
#define READTCP 290
#define WRITETCP 291
#define PLUS 292
#define MINUS 293
#define STAR 294
#define SLASH 295
#define VARIABLE 296
#define STRING 297
#define IDENTIFIER 298
#define NUMBER 299
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    6,    6,    7,    7,    7,    7,    7,    7,
    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
    7,    7,    7,    8,    8,    8,   15,   16,   28,   17,
   27,   27,   10,   11,   12,   13,   18,    9,   21,   14,
   14,   23,   19,   19,   20,   24,   24,   25,   26,    5,
    5,    5,    5,    4,    4,    4,    4,    4,    4,    4,
    1,    1,    1,    1,    1,    1,    3,    3,    2,    2,
   22,
};
short yylen[] = {                                         2,
    0,    1,    2,    3,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    3,    2,    2,    0,    7,
    2,    4,    2,    3,    5,    3,    2,    4,    2,    3,
    3,    3,    2,    4,    4,    5,    5,    3,    3,    1,
    3,    3,    2,    3,    3,    3,    3,    3,    3,    3,
    1,    3,    3,    3,    3,    3,    1,    1,    1,    1,
    2,
};
short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
   23,    0,    0,    0,   68,   69,   70,    0,   67,   61,
    0,    0,   27,   28,    0,    0,    0,    0,    0,    0,
   37,    0,   43,   39,   71,    0,    0,    0,    0,    0,
    0,    0,    0,    3,   26,    0,    0,    0,    0,    0,
    0,   41,   40,   34,    0,    0,   53,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   36,   42,
    0,    0,   48,   49,    4,    0,   66,    0,    0,   64,
   65,   60,    0,    0,    0,    0,    0,    0,   51,   52,
    0,   44,    0,   45,    0,   29,   35,   46,   47,    0,
    0,   30,    0,   31,    0,   32,
};
short yydgoto[] = {                                      20,
   58,   49,   50,   59,   60,   21,   22,   23,   24,   25,
   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
   36,   37,   38,   39,   40,   41,  132,  130,
};
short yysindex[] = {                                   -166,
 -286, -274, -251, -263, -271, -226, -224, -259, -219, -261,
 -265, -253, -218, -205, -202, -249, -198, -194, -193,    0,
 -166, -174,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -192, -151, -251,    0,    0,    0, -275,    0,    0,
 -189, -188,    0,    0, -185, -259, -247, -111, -203, -153,
    0, -145,    0,    0,    0, -169, -168, -165, -129, -128,
 -154, -152, -137,    0,    0, -251, -238, -251, -251, -251,
 -251,    0,    0,    0, -211, -116,    0, -251, -251, -251,
 -251, -251, -251, -259, -259, -119, -139, -131,    0,    0,
 -124, -118,    0,    0,    0, -275,    0, -236, -236,    0,
    0,    0, -275, -275, -275, -275, -275, -275,    0,    0,
 -112,    0, -117,    0, -267,    0,    0,    0,    0, -166,
 -210,    0, -166,    0, -109,    0,
};
short yyrindex[] = {                                    178,
  -94,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -92,    0,    0,    0,    0,    0,  -91,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -255,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -90,    0, -136, -121,    0,
    0,    0, -257, -254, -241, -167,  -95,  -93,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -208,
    0,    0,  -88,    0,    0,    0,
};
short yygindex[] = {                                   -127,
   -3,    0,    0,  135,  -52,    0,  172,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 280
short yytable[] = {                                      48,
    2,   56,  131,   86,   54,  135,   50,   59,   57,   44,
   42,   54,   54,   56,   59,   59,   78,   79,   80,   81,
   55,   43,   54,  107,   50,   59,   53,   55,   55,  128,
   64,  129,   51,   52,   62,   63,   45,   46,   55,   47,
   77,  119,  120,   65,   45,   46,   69,   47,   45,   46,
  107,   47,   85,   78,   79,   80,   81,   80,   81,   88,
   89,   90,   91,   92,   93,   94,   95,  133,  134,    1,
    1,   54,  106,   55,  108,  109,  110,  111,   61,   66,
   78,   79,   80,   81,  113,  114,  115,  116,  117,  118,
    1,    2,   67,    3,   57,   68,    4,    5,    6,   70,
    7,   57,   57,   71,   72,   74,   75,   76,   82,   83,
    8,   84,   57,   97,    9,   10,   11,   12,   13,   14,
   15,   16,   17,   18,   19,   62,   96,   98,   99,  101,
  102,  100,   62,   62,   62,   62,   62,   62,   62,   62,
   63,  103,  105,   62,  104,  112,  121,   63,   63,   63,
   63,   63,   63,   63,   63,   62,   62,  122,   63,   88,
   89,   90,   91,   92,   93,  123,   56,  126,   58,  136,
   63,   63,  124,   56,   56,   58,   58,    1,  125,  127,
   78,   79,   80,   81,   56,   24,   58,   25,   33,   38,
    1,   87,   73,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    2,    2,
};
short yycheck[] = {                                       3,
    0,  261,  130,   56,  262,  133,  262,  262,  268,  261,
  297,  269,  270,  261,  269,  270,  292,  293,  294,  295,
  262,  296,  280,  262,  280,  280,  298,  269,  270,  297,
  296,  299,  296,  297,  296,  297,  296,  297,  280,  299,
   44,   94,   95,  297,  296,  297,  296,  299,  296,  297,
  262,  299,   56,  292,  293,  294,  295,  294,  295,  271,
  272,  273,  274,  275,  276,  269,  270,  278,  279,  278,
  279,  298,   76,  298,   78,   79,   80,   81,  298,  298,
  292,  293,  294,  295,   88,   89,   90,   91,   92,   93,
  257,  258,  298,  260,  262,  298,  263,  264,  265,  298,
  267,  269,  270,  298,  298,  280,  299,  259,  298,  298,
  277,  297,  280,  259,  281,  282,  283,  284,  285,  286,
  287,  288,  289,  290,  291,  262,  280,  297,  297,  259,
  259,  297,  269,  270,  271,  272,  273,  274,  275,  276,
  262,  296,  280,  280,  297,  262,  266,  269,  270,  271,
  272,  273,  274,  275,  276,  292,  293,  297,  280,  271,
  272,  273,  274,  275,  276,  297,  262,  280,  262,  279,
  292,  293,  297,  269,  270,  269,  270,    0,  297,  297,
  292,  293,  294,  295,  280,  280,  280,  280,  280,  280,
  279,   57,   21,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  278,  279,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 299
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"CONNECT","SET","EQUALS","PRINT",
"OPENPAREN","CLOSEPAREN","QUERY","DATABASE","FREE","THEN","PRINTROWS","NOT",
"AND","OR","EQ","NOTEQ","LESS","LESSEQ","MORE","MOREEQ","IF","ELSE","FI","EOL",
"GETROW","EXEC","CRYPT","USER","PRINTATTR","PRINTHEADER","DEFAULT","PIPE",
"OPENTCP","READTCP","WRITETCP","PLUS","MINUS","STAR","SLASH","VARIABLE",
"STRING","IDENTIFIER","NUMBER",
};
char *yyrule[] = {
"$accept : start",
"start :",
"start : statement_list",
"statement_list : statement EOL",
"statement_list : statement_list statement EOL",
"statement : connect_statement",
"statement : set_statement",
"statement : print_statement",
"statement : printrows_statement",
"statement : printattr_statement",
"statement : printheader_statement",
"statement : query_statement",
"statement : database_statement",
"statement : free_statement",
"statement : if_statement",
"statement : nextrow_statement",
"statement : exec_statement",
"statement : pipe_statement",
"statement : crypt_statement",
"statement : user_statement",
"statement : default_statement",
"statement : opentcp_statement",
"statement : readtcp_statement",
"statement : writetcp_statement",
"connect_statement : CONNECT",
"connect_statement : CONNECT STRING",
"connect_statement : CONNECT STRING NUMBER",
"database_statement : DATABASE IDENTIFIER",
"free_statement : FREE IDENTIFIER",
"$$1 :",
"if_statement : IF condition EOL THEN EOL $$1 if_part2",
"if_part2 : start FI",
"if_part2 : start ELSE start FI",
"print_statement : PRINT expression",
"printrows_statement : PRINTROWS IDENTIFIER STRING",
"printattr_statement : PRINTATTR IDENTIFIER STRING STRING STRING",
"printheader_statement : PRINTHEADER IDENTIFIER STRING",
"nextrow_statement : GETROW IDENTIFIER",
"set_statement : SET VARIABLE EQUALS expression",
"crypt_statement : CRYPT VARIABLE",
"query_statement : QUERY STRING IDENTIFIER",
"query_statement : QUERY VARIABLE IDENTIFIER",
"default_statement : DEFAULT IDENTIFIER STRING",
"exec_statement : EXEC STRING",
"exec_statement : EXEC VARIABLE EQUALS STRING",
"pipe_statement : PIPE VARIABLE EQUALS STRING",
"opentcp_statement : OPENTCP IDENTIFIER EQUALS STRING STRING",
"opentcp_statement : OPENTCP IDENTIFIER EQUALS STRING NUMBER",
"readtcp_statement : READTCP IDENTIFIER VARIABLE",
"writetcp_statement : WRITETCP IDENTIFIER STRING",
"condition : single_condition",
"condition : single_condition AND condition",
"condition : single_condition OR condition",
"condition : NOT single_condition",
"single_condition : expression EQ expression",
"single_condition : expression LESS expression",
"single_condition : expression MORE expression",
"single_condition : expression LESSEQ expression",
"single_condition : expression MOREEQ expression",
"single_condition : expression NOTEQ expression",
"single_condition : OPENPAREN condition CLOSEPAREN",
"expression : value",
"expression : expression PLUS expression",
"expression : expression MINUS expression",
"expression : expression STAR expression",
"expression : expression SLASH expression",
"expression : OPENPAREN expression CLOSEPAREN",
"value : literal",
"value : VARIABLE",
"literal : STRING",
"literal : NUMBER",
"user_statement : USER STRING",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 321 "parser.y"
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
#line 944 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 24:
#line 126 "parser.y"
{ connected=dosethostname("localhost"); }
break;
case 25:
#line 128 "parser.y"
{ connected=dosethostname(yyvsp[0].s_value); free(yyvsp[0].s_value); }
break;
case 26:
#line 130 "parser.y"
{ db_setport((int)yyvsp[0].f_value);
			       connected=dosethostname(yyvsp[-1].s_value); free(yyvsp[-1].s_value); }
break;
case 27:
#line 134 "parser.y"
{
			     opendb(yyvsp[0].s_value);
			     free(yyvsp[0].s_value);
			  }
break;
case 28:
#line 139 "parser.y"
{
			     if (!dofree(yyvsp[0].s_value))
			        doerror("No query '%s'<BR>\n",yyvsp[0].s_value);
			     free(yyvsp[0].s_value);
			  }
break;
case 29:
#line 146 "parser.y"
{ if (yyvsp[-3].i_value==0) { startmeal=1; yyclearin; } }
break;
case 33:
#line 154 "parser.y"
{
			     char *v=printstring(yyvsp[0].s_value);
			     printf("%s\n",v);
			     free(v);
			  }
break;
case 34:
#line 161 "parser.y"
{
			     printrows(yyvsp[-1].s_value, yyvsp[0].s_value);
			     free(yyvsp[-1].s_value);
			     free(yyvsp[0].s_value);
			  }
break;
case 35:
#line 168 "parser.y"
{
			      printattr(yyvsp[-3].s_value,yyvsp[-2].s_value,yyvsp[-1].s_value,yyvsp[0].s_value);
			      free(yyvsp[-3].s_value);
			      free(yyvsp[-2].s_value);
			      free(yyvsp[-1].s_value);
			      free(yyvsp[0].s_value);
			   }
break;
case 36:
#line 177 "parser.y"
{
			      printheader(yyvsp[-1].s_value, yyvsp[0].s_value);
			      free(yyvsp[-1].s_value);
			      free(yyvsp[0].s_value);
			   }
break;
case 37:
#line 183 "parser.y"
{
			      nextrow(yyvsp[0].s_value);
			      free(yyvsp[0].s_value);
			   }
break;
case 38:
#line 189 "parser.y"
{ setvar(yyvsp[-2].s_value,yyvsp[0].s_value); }
break;
case 39:
#line 192 "parser.y"
{ docrypt(yyvsp[0].s_value); }
break;
case 40:
#line 195 "parser.y"
{ doquery(yyvsp[-1].s_value,yyvsp[0].s_value); }
break;
case 41:
#line 197 "parser.y"
{
			      char *s=printvar(yyvsp[-1].s_value);
			      free(yyvsp[-1].s_value);
			      s=strdup(s?s:"");
			      doquery(s,yyvsp[0].s_value);
			   }
break;
case 42:
#line 205 "parser.y"
{
			      dodefault(yyvsp[-1].s_value,yyvsp[0].s_value);
			      free(yyvsp[-1].s_value); free(yyvsp[0].s_value);
			   }
break;
case 43:
#line 211 "parser.y"
{
			      doexec(yyvsp[0].s_value,0);
			   }
break;
case 44:
#line 215 "parser.y"
{
			      char cont[10];
			      sprintf(cont,"%d",doexec(yyvsp[0].s_value,1));
			      setvar(yyvsp[-2].s_value,strdup(cont));
			   }
break;
case 45:
#line 222 "parser.y"
{
			      dopipe(yyvsp[-2].s_value, yyvsp[0].s_value);
			   }
break;
case 46:
#line 227 "parser.y"
{
			      int port;
			      if (sscanf(yyvsp[0].s_value, "%d")!=1)
			      {
			         doerror("OPENTCP: Invalid port: %s", yyvsp[0].s_value);
			         free(yyvsp[-3].s_value);
			      } else doopentcp(yyvsp[-3].s_value, yyvsp[-1].s_value, port);
			      free(yyvsp[-1].s_value);
			      free(yyvsp[0].s_value);
			   }
break;
case 47:
#line 238 "parser.y"
{
			      doopentcp(yyvsp[-3].s_value, yyvsp[-1].s_value, (int)yyvsp[0].f_value);
			      free(yyvsp[-1].s_value);
			   }
break;
case 48:
#line 244 "parser.y"
{
			      doreadtcp(yyvsp[0].s_value, yyvsp[-1].s_value);
			      free(yyvsp[-1].s_value);
			      free(yyvsp[0].s_value);
			   }
break;
case 49:
#line 251 "parser.y"
{
			      dowritetcp(yyvsp[0].s_value, yyvsp[-1].s_value);
			      free(yyvsp[-1].s_value);
			      free(yyvsp[0].s_value);
			   }
break;
case 51:
#line 260 "parser.y"
{ yyval.i_value=yyvsp[-2].i_value&&yyvsp[0].i_value; }
break;
case 52:
#line 262 "parser.y"
{ yyval.i_value=yyvsp[-2].i_value||yyvsp[0].i_value; }
break;
case 53:
#line 264 "parser.y"
{ yyval.i_value=!yyvsp[0].i_value; }
break;
case 54:
#line 267 "parser.y"
{ yyval.i_value=expr(yyvsp[-2].s_value, yyvsp[0].s_value, EQ); }
break;
case 55:
#line 269 "parser.y"
{ yyval.i_value=expr(yyvsp[-2].s_value, yyvsp[0].s_value, LESS); }
break;
case 56:
#line 271 "parser.y"
{ yyval.i_value=expr(yyvsp[-2].s_value, yyvsp[0].s_value, MORE); }
break;
case 57:
#line 273 "parser.y"
{ yyval.i_value=expr(yyvsp[-2].s_value, yyvsp[0].s_value, LESSEQ); }
break;
case 58:
#line 275 "parser.y"
{ yyval.i_value=expr(yyvsp[-2].s_value, yyvsp[0].s_value, MOREEQ); }
break;
case 59:
#line 277 "parser.y"
{ yyval.i_value=expr(yyvsp[-2].s_value, yyvsp[0].s_value, NOTEQ); }
break;
case 60:
#line 279 "parser.y"
{ yyval.i_value=yyvsp[-1].i_value; }
break;
case 62:
#line 284 "parser.y"
{ yyval.s_value=calc(yyvsp[-2].s_value,yyvsp[0].s_value,PLUS); }
break;
case 63:
#line 286 "parser.y"
{ yyval.s_value=calc(yyvsp[-2].s_value,yyvsp[0].s_value,MINUS); }
break;
case 64:
#line 288 "parser.y"
{ yyval.s_value=calc(yyvsp[-2].s_value,yyvsp[0].s_value,STAR); }
break;
case 65:
#line 290 "parser.y"
{ yyval.s_value=calc(yyvsp[-2].s_value,yyvsp[0].s_value,SLASH); }
break;
case 66:
#line 292 "parser.y"
{ yyval.s_value=yyvsp[-1].s_value; }
break;
case 68:
#line 296 "parser.y"
{
			     yyval.s_value=varlookup(yyvsp[0].s_value);
			     free(yyvsp[0].s_value);
			  }
break;
case 69:
#line 302 "parser.y"
{
			     yyval.s_value=printstring(yyvsp[0].s_value);
			     free(yyvsp[0].s_value);
			  }
break;
case 70:
#line 307 "parser.y"
{
			     char buf[100];
			     printnum(buf,yyvsp[0].f_value);
			     yyval.s_value=strdup(buf);
			  }
break;
case 71:
#line 314 "parser.y"
{
			       dosetuser(yyvsp[0].s_value);
			       free(yyvsp[0].s_value);
			   }
break;
#line 1331 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
