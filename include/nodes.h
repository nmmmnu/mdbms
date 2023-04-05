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

#ifndef nodesh
#define nodesh
#include <time.h>

#define FUNC_AVG 1
#define FUNC_COUNT 2
#define FUNC_MAX 3
#define FUNC_MIN 4
#define FUNC_SUM 5

#define FUNC_YEAR   6
#define FUNC_MONTH  7
#define FUNC_DAY    8
#define FUNC_HOUR   9
#define FUNC_MINUTE 10
#define FUNC_SECOND 11
#define FUNC_DATE   12
#define FUNC_STRIPDATE 13
#define FUNC_STRIPTIME 14
#define FUNC_ROWNUM 15
#define FUNC_WEEKNUM 16
#define FUNC_DAYOFWEEK 17

#define DELRULE_RESTRICT 0
#define DELRULE_CASCADE  1
#define DELRULE_SETNULL  2

enum nodetype
{
OPEN_USER, NODE_DROP, NODE_CREATE, NODE_USER, NODE_IDENT, NODE_CREATEDB,
NODE_DB, NODE_MORE, ROOT_SET, NODE_SET, NODE_CREATETBL, NODE_TABLEDEF,
NODE_TABLEENT, NODE_ATTRTYPE, NODE_OPEN, NODE_ATTRIBUTE, NODE_SYNC,
NODE_INSERT, NODE_HELP, NODE_SELECT, NODE_STAR, NODE_COLEXP, NODE_COLFUNC,
NODE_MULTIPLY, NODE_DIVIDE, NODE_ADD, NODE_SUBSTRACT, NODE_UMINUS, NODE_UPLUS,
NODE_AND, NODE_OR, NODE_EQUALS, NODE_WHERE, NODE_FROM, NODE_NULL, NODE_NOT_NULL,
NODE_IN, NODE_LESS, NODE_LEQ, NODE_MOQ, NODE_NEQ, NODE_DELETE, NODE_CREATEVIEW,
NODE_UPDATE, NODE_INLIST, NODE_BETWEEN, NODE_EXISTS, NODE_NOT, NODE_ORDER,
NODE_GROUP, NODE_FUNCTION, NODE_KEYS, NODE_FOREIGN, NODE_DELRULE, NODE_PROCEDURE,
NODE_CREATEUSER, NODE_GRANT, NODE_REVOKE, NODE_GRANTREVOKE, NODE_DROPDB, NODE_LIKE
};


struct node
{
   nodetype type;
   char *s1;
   char *s2;
   node *n1;
   node *n2;
   int num1, num2;
   int result;
   time_t dval;
   double fval;

   node(nodetype, char *_s1=NULL, char *_s2=NULL, node *_n1=NULL, node *_n2=NULL, int _num1=0, int _num2=0);
   node(nodetype, int _num1, int _num2=0);
   node(nodetype, node *_n1=NULL, node *_n2=NULL);
   ~node();
};
extern node *rootnode;
#endif
