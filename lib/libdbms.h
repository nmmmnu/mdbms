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

#ifndef LIBDBMSH
#define LIBDBMSH

#include <time.h>

/* Attribute types */
   #define ATT_INT         1
   #define ATT_FLOAT       2
   #define ATT_MONEY       3
   #define ATT_CHAR        4
   #define ATT_VARCHAR     5
   #define ATT_DATE        6
   #define ATT_FILE        7
   #define ATT_SOUND       8
   #define ATT_IMAGE       9
   #define ATT_NULL        10

/* On delete rules */
   #define DELRULE_RESTRICT 0
   #define DELRULE_CASCADE  1
   #define DELRULE_SETNULL  2


extern char *months[];
/*************************** TABLE DEFENITION STRUCTURES ****************/
typedef struct attribute
{
   char name[40];       /* The name of the attribute                          */
   int type;            /* The type of the attribute (ATT_INT, ATT_FLOAT,...) */
   int size;            /* The size in bytes of the attribute                 */
   int nulls;           /* Are NULLs allowed? 1=yes, 0=no                     */
   int primary;         /* Is this attr part of the primary key? 1=yes, 0=no  */
   int foreign;         /* What foreign key does this attr belong to?         */
                        /*    -1=no foreign key, other=index in fkey struct   */
} attribute;

typedef struct fkey
{
   char identifies[40];   /* Name of the table this foreign key references   */
   int action;            /* Action to take on delete (DELRULE_RESTRICT,...) */
} fkey;

typedef struct tabledef
{
   int nattributes;          /* The number of attributes in this table    */
   int nfkeys;               /* The number of foreign keys in this table  */
   attribute *attributes;    /* Array of attributes                       */
   fkey *fkeys;              /* Array of foreign keys                     */
} tabledef;

/********************************* RECORD STRUCTURES ********************/
typedef struct value
{
   int type;                     /* Type of this attribute */
   int size;                     /* Size of this attribute in bytes */
   union
   {
      int integer;
      double floating;
      time_t date;
      char *string;
   } value;                      /* Value of this attribute */
} value;
typedef struct record
{
   int nvalues;                  /* Number of attributes in this record */
   value *values;                /* Array of attributes */
} record;

/************************** INTERNAL STRUCTURES  ***********************/

typedef struct db_session
{
   struct db_session *next, *prev;
   int sock;
   int counter;
   int dataquery;
   char *database;
   char *user, *password;
   char *buffer;
   char linebuf[1000];
   record *currentheader;
} db_session;

extern db_session *db_root_ses;

/************************** FUNCTION DEFINITIONS ***********************/

#ifdef __cplusplus
extern "C"
{
#endif

void db_sethostname(char *);
void db_setport(int);

int db_set_global_authentication(char *, char *);
/* Returns : 0=success, other=error index */

int db_set_local_authentication(db_session *, char *, char *);
/* Returns : 0=success, other=error index */

db_session *db_open_session();

int db_open_database(db_session *, char *);
/* Returns : 0=success, other=error index */

int db_exec_sql(db_session *, int, char *, ...);
/* Returns : 0=success, other=error index */

int db_set_buffersize(db_session *, int);
/* Returns : 0=success, other=error */

void db_free_record(record *);
record *db_get_next_record(db_session *);
char *db_sprintvalue(value *, char *);
tabledef *db_get_table_def(db_session *, char *);
void db_close_session(db_session *);
void db_quote(char *, char *);

extern int db_server_port;

#ifdef __cplusplus
}
#endif
 
#endif
