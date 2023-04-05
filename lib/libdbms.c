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
#include <stdarg.h>
#include <time.h>
#include <netdb.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "libdbms.h"
#include "protocol.h"

/* FORWARD DECLARATIONS */
int db_cmdnum(char *s);

db_session *db_root_ses=NULL;
char *db_server_name=NULL;                       /* Hostname of MDBMS server */
int db_server_port=2224;                         /* Port of MDBMS server */
char *globaluser=NULL, *globalpassword=NULL;     /* Global user and password */

char *months[]=
{
   "jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"
};

#define MAXSTACK 10
#define SOCKTIMEOUT 10

typedef struct sockstack
{
   int fd;
   time_t pushed;
   char *user, *pw, *db;
} sockstack;

sockstack sstack[MAXSTACK];
int stackp=0;

void db_suspend_session(db_session *session);

/**************************************************************************/
/******************************** SYSTEM FUNCTIONS ************************/
/**************************************************************************/


/*
 *  Open a new socket and connect to the database server.
 *  Returns -1 on failure or file descriptor on success.
 */
int db_opensocket()
{
   int sock;
   struct sockaddr_in sin;
   struct hostent *hent;
   hent=gethostbyname(db_server_name?db_server_name:"localhost");
   if (!hent) return -1;
   sock=socket(AF_INET,SOCK_STREAM,0);
   if (sock<0) return -1;
   memcpy((char*)&sin.sin_addr, (char*)hent->h_addr, hent->h_length);
   sin.sin_port=htons(db_server_port), sin.sin_family=AF_INET;
   if (connect(sock,(struct sockaddr*)&sin,sizeof(sin))==-1) return -1;
   return sock;
}



void db_lowerstack()
{
   stackp--;
   if (sstack[stackp].user) free(sstack[stackp].user);
   if (sstack[stackp].pw) free(sstack[stackp].pw);
   if (sstack[stackp].db) free(sstack[stackp].db);
}
/*
 *  Pop a socket on the stack
 */

int db_popsock(char *u, char *p, char *d)
{
   *u=*p=*d='\0';
   do
   {
      if (!stackp) return -1;
      if ((time(NULL)-sstack[stackp-1].pushed)>SOCKTIMEOUT)
      {
         close(sstack[stackp-1].fd);
         db_lowerstack();
      }
      else
      {
         int fd=sstack[stackp-1].fd;
         if (sstack[stackp-1].user) strcpy(u, sstack[stackp-1].user);
         if (sstack[stackp-1].pw) strcpy(p, sstack[stackp-1].pw);
         if (sstack[stackp-1].db) strcpy(d, sstack[stackp-1].db);
         db_lowerstack();
         return fd;
      }
   } while(1);
}

/*
 *  Push a socket on the stack
 */
void db_pushsock(int sock, char *u, char *p, char *d)
{
   if (stackp==MAXSTACK)
   {
      close(sock);
      return;
   }
   sstack[stackp].fd=sock;
   sstack[stackp].pushed=time(NULL);
   if (u) u=strdup(u);
   if (p) p=strdup(p);
   if (d) d=strdup(d);
   sstack[stackp].user=u;
   sstack[stackp].pw=p;
   sstack[stackp].db=d;
   stackp++;
}

/*
 * Get a new socket.
 */
int db_getsock(char *u, char *p, char *d)
{
   int fd=db_popsock(u, p, d);
   if (fd!=-1) return fd;
   return db_opensocket();
}
/*
 * Attempt to read more bytes from the server.
 * Returns 1 on success and 0 on failure.
 */
int db_readmore(db_session *session, int block)
{
   char buf[101];
   int nbytes;
   if (session->sock==-1) return 0;
   if (!block)
   {
      struct timeval timeout={0,0};
      fd_set rmask;
      FD_ZERO(&rmask);
      FD_SET(session->sock, &rmask);
      select(session->sock+1, &rmask, NULL, NULL, &timeout);
      if (!FD_ISSET(session->sock, &rmask)) return 0;
   }
   nbytes=read(session->sock,buf,100);
   if (nbytes<=0) return 0;
   buf[nbytes]='\0';
   strcat(session->buffer,buf);
   return 1;
}

/*
 * Sends a ping to the server in reply to a received ping.
 */
void db_sendping(db_session *session)
{
   char buf[10];
   sprintf(buf,"\\p\n");
   write(session->sock,buf,strlen(buf));
}
/*
 * Isolates the next line from the session's input.
 * When no line is available, read more data from the server
 * Returns the session's filled linebuffer on success, NULL on failure
 */
char *db_nextline(db_session *session, int block)
{
   char *where;
   while ((where=strchr(session->buffer,'\n'))==NULL)
      if (db_readmore(session, block)==0) return NULL;
   *(where++)='\0';
   strcpy(session->linebuf, session->buffer);
   memmove(session->buffer,where,strlen(where)+1);
   if (db_cmdnum(session->linebuf)==MDP_PING)
      db_sendping(session);
   return session->linebuf;
}
/*
 * Detect if this is a FOP code
 * Return -1 if it's not a code, otherwise return the code number.
 */
int db_cmdnum(char *s)
{
   if (s[0]!='!') return -1;
   return atoi(s+1);
}
/*
 * Flush all data from to server, until the specified code is found.
 */
int db_flush_to_code(db_session *session, int code)
{
   char *p;
   do
   {
      p=db_nextline(session, 1);
      if (!p) return 0;      
   } while (db_cmdnum(p)!=code);
   return 1;
}
/*
 * Create a new record structure with the indicated amount of values
 */
record *db_create_record(int nattributes)
{
   record *newrecord=(record*)malloc(sizeof(record));
   if (!newrecord) return NULL;
   newrecord->nvalues=nattributes;
   newrecord->values=(value*)calloc(nattributes,sizeof(value));
   return newrecord;
}
/*
 *  Free a record structure and all its data.
 */
void db_free_record(record *r)
{
   int x;
   for (x=0;x<r->nvalues;x++)
      if (r->values[x].type==ATT_CHAR||r->values[x].type==ATT_VARCHAR)
      free(r->values[x].value.string);
   free(r->values);
   free(r);
}

/*
 * Execute an SQL statement
 * Returns -1 on read error, 0 on success, other numbers indicate
 * errors. See errors.h and errors.c for this numbers.
 */
int db_sql(db_session *session, int wait, char *s, ...)
{
   int x;
   char b[1000], *line;
   va_list ap;
   session->dataquery=0;
   if (wait)
   {
      /* If we have to wait for the query result, we first need to output
         a synchronisation code to the server, so we can recognize the
         correct query result */
      sprintf(b,"\\%d\n",++session->counter);
      write(session->sock,b,strlen(b));
   }
   va_start(ap,s);
   vsprintf(b,s,ap);
   va_end(ap);
   strcat(b,"\n");
   write(session->sock,b,strlen(b));
   if (!wait) return 0;     /* Return if the query result is not needed */

   do
   {
      /* Wait until we have the right query result */
      if (!db_flush_to_code(session, MDP_QUERYRESPONSE)) return -1;
      line=db_nextline(session, 1);
   } while (atoi(line)!=session->counter);

   line=db_nextline(session, 1);
   if (!line) return -1;
   if (db_cmdnum(line)!=MDP_OK)             /* An error occured */
   {
      line=db_nextline(session, 1);            /* Amount of errors */
      if (!line) return -1;
      line=db_nextline(session, 1);            /* Error line */
      if (!line) return -1;
      return atoi(line);
   }
   line=db_nextline(session, 1);
   if (!line) return -1;
   if (db_cmdnum(line)==MDP_READY) return 0;  /* No data will follow */

   /* Data will follow */
   session->dataquery=1;
   line=db_nextline(session, 1);
   if (!line) return -1;
   if (session->currentheader) db_free_record(session->currentheader);
   session->currentheader=db_create_record(atoi(line));
   for (x=0;x<session->currentheader->nvalues;x++)
   {
      char buf[100];
      value *v=&session->currentheader->values[x];
      line=db_nextline(session, 1);
      sscanf(line,"%d %d %s",&v->type, &v->size, buf);
      v->value.string=strdup(buf);
   }
   return 0;
}

/*
 * Send user and password information for this session.
 */
void db_authenticate_session(db_session *session)
{
   char *user, *password;
   if (session->sock==-1) return;
   if (session->user) user=session->user, password=session->password; else
      user=globaluser, password=globalpassword;
   if (user==NULL) return;
   if (password) db_sql(session, 1, "user %s password '%s'", user, password);
      else db_sql(session, 1, "user %s", user);
}

/*
 * Prepare a session for usage. If there's no socket associated with
 * the session, assign one now.
 * Send authentication information if nessecary.
 */
int db_prepare_session(db_session *session)
{
   char user[100]="", pw[100]="", db[200]="";
   char *su=session->user?session->user:globaluser;
   char *sp=session->password?session->password:globalpassword;
   char *sd=session->database?session->database:"";
   if (session->sock!=-1) return 1;
   session->sock=db_getsock(user, pw, db);
   if (session->sock==-1) return -1;
   session->buffer[0]='\0';
   if (su&&sp) if (strcmp(user, su)||strcmp(pw, sp))
      db_authenticate_session(session);
   if (session->database&&strcmp(sd, db))
      db_sql(session,0,"open %s",session->database);
   session->buffer[0]='\0';
   return 1;
}

void db_suspend_session(db_session *session)
{
   char *u, *p;
   if (session->user) u=session->user, p=session->password; else
      u=globaluser, p=globalpassword;
   if (session->sock==-1) return;
   db_pushsock(session->sock, u, p, session->database);
   session->sock=-1;
}
int db_check_authentication(char *user, char *password)
{
   int ret;
   db_session *newsession=NULL, *session;
   if (!db_root_ses) newsession=db_open_session();
   session=db_root_ses;
   if (password) ret=db_exec_sql(session, 1, "user %s password '%s'", user, password);
      else ret=db_exec_sql(session, 1, "user %s", user);
   if (newsession) db_close_session(newsession);
   return ret;
}
/**************************************************************************/
/********************************** USER FUNCTIONS ************************/
/**************************************************************************/

/*
 *   0  : Ok
 *   1  : Error
 */
int db_set_global_authentication(char *user, char *password)
{
   if (globaluser) free(globaluser);
   if (globalpassword) free(globalpassword);

   if (user) globaluser=strdup(user);
      else globaluser=NULL;
   if (password) globalpassword=strdup(password);
      else globalpassword=NULL;
   return db_check_authentication(user,password);
}

/*
 *   0  : Ok
 *   1  : Error
 */
int db_set_local_authentication(db_session *session, char *user, char *password)
{
   if (session->user) free(session->user);
   if (session->password) free(session->password);

   if (user) session->user=strdup(user);
      else session->user=NULL;
   if (password) session->password=strdup(password);
      else session->password=NULL;
   db_authenticate_session(session);
   return db_check_authentication(user,password);
}

db_session *db_open_session()
{
   db_session *newsession=(db_session*)calloc(1,sizeof(db_session));
   signal(SIGPIPE, SIG_IGN);
   if (!newsession) return NULL;
   newsession->next=db_root_ses;
   if (db_root_ses) db_root_ses->prev=newsession;
   db_root_ses=newsession;
   newsession->sock=-1;
   newsession->buffer=(char*)malloc(10000);
   return newsession;
}
void db_close_session(db_session *session)
{
   if (session->buffer) free(session->buffer);
   if (session->user) free(session->user);
   if (session->password) free(session->password);
   if (session->database) free(session->database);
   if (session->currentheader) db_free_record(session->currentheader);
   if (session->next) session->next->prev=session->prev;
   if (session->prev) session->prev->next=session->next; else
      db_root_ses=session->next;
   if (session->sock!=-1) close(session->sock);
   free(session);
}

int db_set_buffersize(db_session *session, int size)
{
   char *buffer=(char*)realloc(session->buffer,size);
   if (!buffer) return 0;
   session->buffer=buffer;
   return 1;
}

void db_sethostname(char *hostname)
{
   if (db_server_name) free(db_server_name);
   if (hostname) db_server_name=strdup(hostname);
      else hostname=NULL;
}

void db_setport(int port)
{
   db_server_port=port;
}

/*
 * Execute an SQL statement.
 * Returns 0 on succes or other on failure
 */
int db_exec_sql(db_session *session, int wait, char *s, ...)
{
   int retcode;
   char b[1000];
   va_list ap;
   va_start(ap,s);
   vsprintf(b,s,ap);
   va_end(ap);

   if (db_prepare_session(session)==-1) return -1;
   retcode=db_sql(session,wait,"%s",b);
   if (session->dataquery==0) db_suspend_session(session);
   return retcode;
}

record *db_get_next_record(db_session *session)
{
   int x;
   record *thisrec=NULL;
   record *header=session->currentheader;
   if (!header) return NULL;
   for (x=0;x<header->nvalues;x++)
   {
      char *line=db_nextline(session, 1);
      value *val;
      if (db_cmdnum(line)==MDP_READY)
      {
         db_free_record(header);
         session->currentheader=NULL;
         db_suspend_session(session);
         return NULL;
      }
      if (!thisrec)
      {
         thisrec=(record *)malloc(sizeof(record));
         thisrec->values=(value*)malloc(header->nvalues*sizeof(value));
         thisrec->nvalues=header->nvalues;
      }
      val=thisrec->values+x;
      val->type=header->values[x].type;
      if (!strcmp(line,"\\NULL"))
      {
         val->type=ATT_NULL;
         val->value.string=NULL;
      } else switch (header->values[x].type)
      {
         case ATT_INT:   sscanf(line,"%d",&val->value.integer); break;
         case ATT_MONEY:
         case ATT_FLOAT: sscanf(line,"%lf",&val->value.floating); break;
         case ATT_DATE:  sscanf(line,"%lu",&val->value.date); break;
         case ATT_VARCHAR:
         case ATT_CHAR:  if (line[0]=='\\') line++;
                         val->value.string=strdup(line); break;
      }
   }
   return thisrec;
}

int db_open_database(db_session *session, char *name)
{
   int res=db_exec_sql(session,1,"open %s",name);
   if (session->database) free(session->database);
   session->database=name?strdup(name):(char*)NULL;
   return res;
}

void sprintfdate(char *buf, time_t date)
{
   char b[100];
   struct tm *timep=localtime(&date);
   buf[0]='\0';
   if (timep->tm_year!=2||timep->tm_mday!=1||timep->tm_mon!=0)
      sprintf(buf,"%d %s %d",timep->tm_mday, months[timep->tm_mon],
      timep->tm_year+1900);
   if (timep->tm_hour||timep->tm_sec||timep->tm_min)
   {
      sprintf(b,"%s%02d:%02d:%02d", buf[0]?" ":"", timep->tm_hour, timep->tm_min,
         timep->tm_sec);
      strcat(buf,b);
   }
}
void sprintfloat(char *buf, double val)
{
   char *p;
   sprintf(buf,"%f",val);
   p=buf+strlen(buf)-1;
   while (*p=='0') *(p--)='\0';
   if (*p=='.') strcat(buf,"0");
}

char *db_sprintvalue(value *cv, char *buf)
{
   switch (cv->type)
   {
      case ATT_NULL    : sprintf(buf,"NULL"); break;
      case ATT_CHAR    :
      case ATT_VARCHAR : strcpy(buf,cv->value.string); break;
      case ATT_INT     : sprintf(buf,"%d",cv->value.integer); break;
      case ATT_FLOAT   : sprintfloat(buf,cv->value.floating); break;
      case ATT_DATE    : sprintfdate(buf,cv->value.date); break;
      case ATT_MONEY   : sprintf(buf,"$ %.2f",cv->value.floating); break;
   }
   return buf;
}

tabledef *db_get_table_def(db_session *session, char *tabname)
{
   int ident,x;
   char *res;
   tabledef *def;
   fkey *fkeys=NULL;
   char query[100];
   
   db_prepare_session(session);
   sprintf(query,"\\%d\n\\h %s\n",++session->counter,tabname);
   write(session->sock,query,strlen(query));
   do
   {
      db_flush_to_code(session,MDP_QUERYRESPONSE);
      res=db_nextline(session, 1); if (!res) return NULL;
      sscanf(res,"%d",&ident);
      res=db_nextline(session, 1); if (!res) return NULL;
      if (ident!=session->counter) continue;
      if (db_cmdnum(res)!=MDP_STARTHELP) return NULL;
   } while (ident!=session->counter);
   res=db_nextline(session, 1); if (!res) return NULL;

   def=(tabledef*)malloc(sizeof(tabledef));
   sscanf(res,"%d",&def->nattributes);
   def->nfkeys=0, def->fkeys=NULL;
   def->attributes=(attribute*)malloc(sizeof(attribute)*def->nattributes);
   for (x=0;x<def->nattributes;x++)
   {
      attribute *at=def->attributes+x;
      at->foreign=-1;
      res=db_nextline(session, 1); if (!res) return NULL;
      sscanf(res,"%d %d %d %d %s",&at->type,&at->size,&at->nulls,
         &at->primary, at->name);
   }
   for (;;)
   {
      char *p,name[50];
      fkey *fk;
      res=db_nextline(session, 1); if (!res) return NULL;
      if (db_cmdnum(res)==MDP_READY) break;
      def->nfkeys++;
      if (fkeys) fkeys=(fkey*)realloc(fkeys,sizeof(fkey)*def->nfkeys);
         else fkeys=(fkey*)malloc(sizeof(fkey));
      fk=fkeys+def->nfkeys-1;
      sscanf(res,"%s %d",fk->identifies,&fk->action);
      p=res;
      while (!isspace(*p)) p++;
      while (isspace(*p)) p++;
      do
      {
         while (!isspace(*p)&&*p) p++;
         while (isspace(*p)) p++;
         if (sscanf(p,"%s",name)==0) break;
         for (x=0;x<def->nattributes;x++)
         {
            attribute *at=def->attributes+x;
            if (!strcmp(at->name,name))
            {
               at->foreign=def->nfkeys-1;
               break;
            }
         }
      } while (*p);
   }
   def->fkeys=fkeys;
   db_suspend_session(session);
   return def;
}
void db_quote(char *buf, char *text)
{
   if (*text) do
   {
      if (*text=='\'') *(buf++)='\\'; else
      if (*text=='\\') *(buf++)='\\';
      *(buf++)=*(text++);
   } while (*text);
   *(buf)='\0';
}
