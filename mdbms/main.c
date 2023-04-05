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
#include <signal.h>
#include <syslog.h>
#include "dbfile.hh"
#include "dbunits.hh"
#include "relation.hh"
#include "interface.hh"
#include "dbms.hh"
#include "global.hh"
#include "support.h"

dbms *globaldb=NULL;

void usage()
{
   printf("Usage: db [-d] [-c] [-v] [-m <kb>] [-p <port>] [-f <port>]\n");
   printf("   -m : Specify the amount of cache memory to use in kilobytes\n");
   printf("        (default is 4000)\n");
   printf("   -p : specify human interface port (default is 2223)\n");
   printf("   -f : specify fast opendatabase port (default is 2224)\n");
   printf("   -v : Show version information\n");
   printf("   -d : Detach from terminal\n");
   printf("   -c : Change directory to server binary directory\n");
   printf("   -h : Show this help\n");
   exit(0);
}
void copyrights()
{
   char *version=getversion();
   printf("%s\n",PRODUCT);
   printf("This is beta software. This version can be distributed freely.\n");
   printf("MDBMS was created by Marty Bochane, HINT InterNetworking Technologies\n");
   printf("Email: marty@hinttech.com\n\n");
   if (!version) version="Not determined";
   printf("Database server version  : %s\n",VERSION);
   printf("Physical layer found     : %s\n",version);
   printf("Physical layer supported : %s\n",PHYSVERSION);
   printf("FOP interface version    : %s\n",FOPVERSION);
   exit(0);
}
void stopdb(int)
{
   while (globaldb->isbusy()) globaldb->run();
   delete globaldb;
   exit(0);
}
void dosignals()
{
   struct sigaction act;
   signal(SIGPIPE, SIG_IGN);
   signal(SIGHUP, SIG_IGN);
   sigemptyset(&act.sa_mask);
   sigaddset(&act.sa_mask, SIGTERM);
   sigaddset(&act.sa_mask, SIGINT);
   act.sa_handler=stopdb;
   act.sa_flags=0;
   sigaction(SIGTERM, &act, NULL);
   sigaction(SIGINT, &act, NULL);
   /*openlog("mdbms",0,LOG_DAEMON);*/
}
int main(int argc, char **argv)
{
   int ch, cachemem=-1, hip=2223, fop=2224, daemon=0, dochdir=0;
   while ((ch=getopt(argc,argv,"dm:vhf:p:"))!=EOF) switch(ch)
   {
      case 'd' : daemon=1; break;
      case 'm' : cachemem=atoi(optarg); break;
      case 'v' : copyrights(); break;
      case 'p' : hip=atoi(optarg); break;
      case 'f' : fop=atoi(optarg); break;
      case 'c' : dochdir=1; break;
      case 'h' :
      default  : usage(); break;
   }
   dosignals();
   if (dochdir)
   {
      char *startupdir, *ptr;
      startupdir=strdup(argv[0]);
      if ((ptr=strrchr(startupdir, '/'))!=NULL)
      {
         *ptr='\0';
         chdir(startupdir);
      }
      free(startupdir);
   }
   if (daemon)
   {
      if (fork())
         exit(0);
      setsid();
   }
   if (cachemem<0) cachemem=4000;
   globaldb=new dbms(cachemem, hip, fop);
   for(;;) globaldb->run();
   delete globaldb;
}
