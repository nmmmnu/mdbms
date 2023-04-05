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

#ifndef supporth
#define supporth

#include <time.h>
void seterror(int, char*);
void dberror(char *);
char *serror(char *);
char *mstrchr(char *, int);
char *getversion();
void docrypt(char *);
int getweekno(struct tm *);

extern char *months[];
extern char *funcnames[];
extern char *grantnames[];
#endif
