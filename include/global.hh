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

#ifndef globalhh
#define globalhh

#define PRODUCT "MDBMS V1.0"
#define VERSION "V 1.0"
#define FOPVERSION "V 1.2"
#define PHYSVERSION "V 2.0"
#define METAFILE "files.bin"

#define USERTIMEOUT 900
#define USERPINGTIMEOUT 200

#include <malloc.h>
#include <string.h>
#include <strings.h>
#include "manage.hh"

#include "errors.h"
extern int dberrno;
extern char *dbcontext;
extern manage *manager;


#define DEBUG
#endif
