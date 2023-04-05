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
#include <ctype.h>

char *names[]=
{
   "time",
   "timerange",
   "lowercase",
   "email",
   "noquotes",
   "nospaces",
   "ipadr",
   "time2",
   "numerical",
   "lowerword",
   "netmask",
   "lowernum",
   "dotcase",
   NULL
};

#define CHECKTIME        0
#define CHECKTIMERANGE   1
#define CHECKLOWERCASE   2
#define CHECKEMAIL       3
#define CHECKNOQUOTES    4
#define CHECKNOSPACES    5
#define CHECKIPADR       6
#define CHECKTIME2       7
#define CHECKNUMERICAL   8
#define CHECKLOWERWORD   9
#define CHECKNETMASK     10
#define CHECKLOWERNUM    11
#define CHECKDOTCASE     12

int verbose=0;

void usage()
{
   int x=0;
   char *s;
   printf("Usage : checktype [-v] -c <checktype> <parameter> [ <parameter> ...]\n");
   printf("<checktype> can be:\n");
   for (s=names[x];s;s=names[++x]) printf("   %s\n",s);
   exit(0);
}
int gettype(char *type)
{
   char *s;
   int x=0;
   for (s=names[x];s;s=names[++x])
      if (!strcasecmp(s,type)) return x;
   return -1;
}
int checktime(char *s)
{
   char buf[100], *p;
   unsigned num1, num2, num3;
   if (sscanf(s,"%u:%u:%u",&num1,&num2,&num3)!=3) return 0;
   if (num1>23||num2>59||num3>59) return 0;
   p=strchr(s,':'); 
   if (!p) return 0;
   while (isdigit(*(++p)));
   if (*p!=':') return 0;
   while (isdigit(*(++p)));
   if (*p) return 0;
   return 1;
}
int checktime2(char *s)
{
   char buf[100], *p;
   unsigned num1, num2;
   if (sscanf(s,"%u:%u",&num1,&num2)!=2) return 0;
   if (num1>23||num2>59) return 0;
   p=strchr(s,':'); 
   if (!p) return 0;
   while (isdigit(*(++p)));
   if (*p) return 0;
   return 1;
}
int checktimerange(char *s1, char *s2)
{
   unsigned num1, num2, num3;
   unsigned long time1, time2;
   sscanf(s1,"%u:%u:%u",&num1,&num2,&num3);
   time1=num3+num2*60+num1*60*60;
   sscanf(s2,"%u:%u:%u",&num1,&num2,&num3);
   time2=num3+num2*60+num1*60*60;
   if (time1>=time2) return 0;
   return 1;
}
int checklowercase(char *s)
{
   char *p;
   for (p=s;*p;p++) if (isupper(*p)) return 0;
   return 1;
}
int checkemail(char *s)
{
   int ats=0;
   char *p;
   for (p=s;*p;p++)
   {
      if (*p=='@') ats++;
      if (*p=='.'||*p=='_'||*p=='@'||isalnum(*p)) continue;
      return 0;
   }
   return ats==1;
}
int checknoquotes(char *s)
{
   char *p;
   for (p=s;*p;p++)
      if (*p=='\"'||*p=='\'') return 0;
   return 1;
}
int checknospaces(char *s)
{
   char *p;
   for (p=s;*p;p++)
      if (isspace(*p)) return 0;
   return 1;
}
int checkipadr(char *s)
{
   int num1,num2,num3,num4;
   char buf[100];
   if (sscanf(s,"%d.%d.%d.%d%s",&num1,&num2,&num3,&num4,buf)!=4) return 0;
   if (num1>255||num2>255||num3>255||num4>255||num1<0||num2<0||num3<0||num4<0)
      return 0;
   return 1;
}
int checknumerical(char *s)
{
   char *p;
   for (p=s;*p;p++) if (!isdigit(*p)) return 0;
   return 1;
}
int checklowerword(char *s)
{
   char *p;
   for (p=s;*p;p++) if (!islower(*p)) return 0;
   return 1;
}
int checklowernum(char *s)
{
   char *p;
   for (p=s;*p;p++) if (!islower(*p)&&!isdigit(*p)) return 0;
   return 1;
}
int checkdotcase(char *s)
{
   char *p;
   for (p=s;*p;p++) if (!isalnum(*p)&&*p!='.') return 0;
   return 1;
}
int checknmdigit(unsigned char num)
{
   int x;
   unsigned char subvalue=0;
   for (x=0;x<9;x++)
   {
      if ((unsigned char)num==(unsigned char)(~subvalue)) return 1;
      subvalue|=(1<<x);
   }
   return 0;
}
int checknetmask(char *s)
{
   int num1,num2,num3,num4;
   char buf[100];
   if (sscanf(s,"%d.%d.%d.%d%s",&num1,&num2,&num3,&num4,buf)!=4) return 0;
   if (num1>255||num2>255||num3>255||num4>255||num1<0||num2<0||num3<0||num4<0)
      return 0;
   if (num2>num1||num3>num2||num4>num3) return 0;
   if (!checknmdigit((unsigned char)num1)) return 0;
   if (!checknmdigit((unsigned char)num2)) return 0;
   if (!checknmdigit((unsigned char)num3)) return 0;
   if (!checknmdigit((unsigned char)num4)) return 0;
   return 1;
}
void end(char *err, char *arg)
{
   if (!verbose) exit(0);
   printf("%s: %s\n",err,arg);
   exit(0);
}
int main(int argc, char **argv)
{
   int ch, checknum=-1, x;
   while ((ch=getopt(argc,argv,"c:v"))!=EOF) switch(ch)
   {
      case 'c' : checknum=gettype(optarg); break;
      case 'v' : verbose=1; break;
      default  : usage(); break;
   }
   if (checknum==-1) usage();
   argc-=optind, argv+=optind;
   for (x=0;x<argc;x++) switch (checknum)
   {
      case CHECKTIME :
         if (!checktime(argv[x]))
            end("Not a valid time",argv[x]);
         break;
      case CHECKTIMERANGE :
         if (!checktimerange(argv[x], argv[x+1]))
         {
            char s[100];
            sprintf(s,"%s - %s",argv[x],argv[x+1]);
            end("Not a valid timerange",s);
         }
         x++;
         break;
      case CHECKLOWERCASE:
         if (!checklowercase(argv[x]))
            end("Contains uppercase characters",argv[x]);
         break;
      case CHECKEMAIL:
         if (!checkemail(argv[x]))
            end("Not a valid email address",argv[x]);
         break;
      case CHECKNOQUOTES:
         if (!checknoquotes(argv[x]))
            end("Contains quotes",argv[x]);
         break;
      case CHECKNOSPACES:
         if (!checknospaces(argv[x]))
            end("Contains spaces",argv[x]);
         break;
      case CHECKIPADR:
         if (!checkipadr(argv[x]))
            end("Invalid IP address",argv[x]);
         break;
      case CHECKTIME2:
         if (!checktime2(argv[x]))
            end("Not a valid time",argv[x]);
         break;
      case CHECKNUMERICAL:
         if (!checknumerical(argv[x]))
            end("Not a number",argv[x]);
         break;
      case CHECKLOWERWORD:
         if (!checklowerword(argv[x]))
            end("Not a lowerword",argv[x]);
         break;
      case CHECKNETMASK:
         if (!checknetmask(argv[x]))
            end("Not a valid netmask",argv[x]);
         break;
      case CHECKLOWERNUM:
         if (!checklowernum(argv[x]))
            end("Not a lowernum",argv[x]);
         break;
      case CHECKDOTCASE:
         if (!checkdotcase(argv[x]))
            end("Not a dotcase",argv[x]);
         break;
      
   }
   exit(1);
}
