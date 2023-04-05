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
#include "lexyacc.h"
#include "nodes.h"
#include "attributes.h"
#include "relation.hh"

void yyerror(char *s);
extern int yylex();
node *rootnode=NULL;
%}

%union
{
   char *s_value;
   int i_value;
   double f_value;
   time_t d_value;
   node *n;
}

%token <s_value> IDENTIFIER STRING STRING1
%token <i_value> NUMBER FUNCTION
%token <f_value> FLOATVAL MONEYVAL
%token <d_value> DATEVAL
%token CREATE AUTHORIZATION DATABASE DROP USER PASSWORD SET TABLE TNULL 
%token INTEGER OPEN VARCHAR DATE MONEY FLOAT SYNC INSERT INTO VALUES CHAR
%token HELP SELECT FROM WHERE ORDER GROUP BY HAVING AS IS IN PRIVILEGES
%token SUM COUNT AVG MIN MAX LESS MORE LEQ MOQ NEQ VIEW UPDATE PRIMARY KEY
%token OPENPAREN CLOSEPAREN SEMICOLON EQUALS COMMA DOT CASCADE RESTRICT DELETE
%token BETWEEN EXISTS FOREIGN REFERENCES ON GRANT WITH OPTION REVOKE TO ALL
%token ASCENDING DESCENDING ROWNUM LIKE
%left PLUS MINUS
%left STAR SLASH
%left OR
%left AND
%left NOT
%type <n> user_statement insert_statement
%type <n> drop_statement statement start statement_list
%type <n> create_statement create_database create_table set_statement create_user
%type <n> table_def table_def_entry attrtype open_statement insert_values
%type <n> insert_value select_statement select_clause from_clause where_clause
%type <n> order_clause group_clause select_clause_list insert_value_neg
%type <n> select_element column_expression func_ident procedure
%type <n> attribute_expression from_item attribute condition 
%type <n> delete_statement update_statement primary_clause ident_list
%type <n> attributeexp_list key_clause foreign_clause foreign_key foreign_rules
%type <n> grant_statement revoke_statement grant_revoke_body grantwho ordergroup
%type <n> attributeorder_list attributeorder_exp 
%type <i_value> nullspec column_function cascade_or_restrict delete_rule
%type <i_value> with_grant gritem gritems

%%

start			: statement_list
                           { rootnode=$1; }
			;
statement_list		: statement semi
                           { $$=new node(NODE_MORE, $1); }
			| statement_list statement semi
                           { $$=new node(NODE_MORE, $2, $1); }
			;
semi			: SEMICOLON
			|
			;
statement		: create_statement
                           { $$=new node(NODE_CREATE, $1); }
			| user_statement
                           { $$=new node(OPEN_USER, $1); }
			| drop_statement
                           { $$=new node(NODE_DROP, $1); }
			| set_statement
                           { $$=new node(ROOT_SET, $1); }
			| open_statement
                           { $$=new node(NODE_OPEN, $1); }
			| delete_statement
			| insert_statement
			| select_statement
			| update_statement
			| grant_statement
			| revoke_statement
			| HELP IDENTIFIER
			   { $$=new node(NODE_HELP, $2); }
			| SYNC
                           { $$=new node(NODE_SYNC, (node*)NULL); }
			;

open_statement		: OPEN IDENTIFIER
			   { $$=new node(NODE_IDENT, $2); }
			;

user_statement		: USER IDENTIFIER PASSWORD IDENTIFIER
                           { $$=new node(NODE_USER,$2,$4); }
			| USER IDENTIFIER PASSWORD STRING
                           { $$=new node(NODE_USER,$2,$4); }
			| USER IDENTIFIER
                           { $$=new node(NODE_USER,$2); }
			;

create_statement	: CREATE DATABASE create_database
                           { $$=new node(NODE_CREATEDB, $3); }
			| CREATE TABLE create_table
                           { $$=new node(NODE_CREATETBL, $3); }
			| CREATE TABLE IDENTIFIER AS select_statement
			   { $$=new node(NODE_CREATEVIEW, $3, NULL, $5); }
			| CREATE USER create_user
			   { $$=$3; }
			;
create_user		: IDENTIFIER
			   { $$=new node(NODE_CREATEUSER, $1); }
			| IDENTIFIER PASSWORD IDENTIFIER
			   { $$=new node(NODE_CREATEUSER, $1, $3); }
			| IDENTIFIER PASSWORD STRING
			   { $$=new node(NODE_CREATEUSER, $1, $3); }
			;
create_database		: IDENTIFIER AUTHORIZATION IDENTIFIER
                           { $$=new node(NODE_DB, $1, $3); }
			| IDENTIFIER
                           { $$=new node(NODE_DB, $1); }
			;
create_table		: IDENTIFIER OPENPAREN table_def key_clause
			  CLOSEPAREN
			   { $$=new node(NODE_TABLEDEF, $1, NULL, $3, $4); }
			;
key_clause		: primary_clause
			   { $$=new node(NODE_KEYS, $1, NULL); }
			| foreign_clause
			   { $$=new node(NODE_KEYS, NULL, $1); }
			| primary_clause foreign_clause
			   { $$=new node(NODE_KEYS, $1, $2); }
			;
primary_clause		:  { $$=NULL;}
			| PRIMARY KEY OPENPAREN ident_list CLOSEPAREN
			   { $$=$4; }
			;
foreign_clause		: foreign_key
			   { $$=new node(NODE_MORE, $1, NULL); }
			| foreign_key foreign_clause
			   { $$=new node(NODE_MORE, $1, $2); }
			;
foreign_key		: FOREIGN KEY OPENPAREN ident_list CLOSEPAREN
			  REFERENCES IDENTIFIER foreign_rules
			   { $$=new node(NODE_FOREIGN, $7, NULL, $4, $8); }
			;
foreign_rules		: { $$=NULL; }
			| ON DELETE delete_rule
			  { $$=new node(NODE_DELRULE, $3); }
			;
delete_rule		: RESTRICT
			   { $$=DELRULE_RESTRICT; }
			| CASCADE
			   { $$=DELRULE_CASCADE; }
			| SET TNULL
			   { $$=DELRULE_SETNULL; }
			;

ident_list		: IDENTIFIER
			   { $$=new node(NODE_MORE, $1, NULL); }
			| IDENTIFIER COMMA ident_list
			   { $$=new node(NODE_MORE, $1, NULL, NULL, $3); }
			;
table_def		: table_def_entry
			   { $$=new node(NODE_MORE, $1);
			     $$->num2=-1; }
			| table_def_entry COMMA table_def
			   { $$=new node(NODE_MORE, $1, $3);
			     $$->num2=-1; }
			;
table_def_entry		: IDENTIFIER attrtype nullspec
			   { $$=new node(NODE_TABLEENT, $1, NULL, $2, NULL, $3); }
			| IDENTIFIER attrtype
			   { $$=new node(NODE_TABLEENT, $1, NULL, $2, NULL, 1); }
			;
attrtype		: INTEGER
			   { $$=new node(NODE_ATTRIBUTE, ATT_INT, sizeof(int)); }
			| VARCHAR OPENPAREN NUMBER CLOSEPAREN
			   { $$=new node(NODE_ATTRIBUTE, ATT_VARCHAR, $3); }
			| DATE
			   { $$=new node(NODE_ATTRIBUTE, ATT_DATE, sizeof(int)); }
			| MONEY
			   { $$=new node(NODE_ATTRIBUTE, ATT_MONEY, sizeof(int)); }
			| FLOAT
			   { $$=new node(NODE_ATTRIBUTE, ATT_FLOAT, sizeof(double)); }
			| CHAR
			   { $$=new node(NODE_ATTRIBUTE, ATT_CHAR, 1); }
			;
nullspec		: TNULL
			   { $$=1; }
			| NOT TNULL
			   { $$=0; }
			;

drop_statement		: DROP TABLE IDENTIFIER cascade_or_restrict
                           { $$=new node(NODE_IDENT,$3,NULL,NULL,NULL,1,$4); }
			| DROP DATABASE IDENTIFIER cascade_or_restrict
                           { $$=new node(NODE_DROPDB,$3,NULL,NULL,NULL,1,$4); }
			;
cascade_or_restrict	:
			   { $$=1; }
			| CASCADE
			   { $$=0; }
			| RESTRICT
			   { $$=1; }
			;

set_statement		: SET IDENTIFIER
			   { $$=new node(NODE_SET, $2); }
			| SET IDENTIFIER EQUALS IDENTIFIER
			   { $$=new node(NODE_SET, $2, $4); }
			| SET IDENTIFIER EQUALS STRING
			   { $$=new node(NODE_SET, $2, $4); }
			;

insert_statement	: INSERT INTO IDENTIFIER VALUES OPENPAREN
			  insert_values CLOSEPAREN
			   { $$=new node(NODE_INSERT, $3, NULL, $6); }
			;
insert_values		: insert_value_neg
			   { $$=new node(NODE_MORE, $1, NULL); }
			| insert_value_neg COMMA insert_values
			   { $$=new node(NODE_MORE, $1, $3); }
			;
insert_value_neg	: insert_value
			| MINUS NUMBER
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_INT, -$2); } 
			| MINUS FLOATVAL
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_FLOAT); 
			     $$->fval=-$2; }
			;
insert_value		: STRING
			   { $$=new node(NODE_ATTRIBUTE, $1, NULL, NULL, NULL, ATT_VARCHAR); }
			| STRING1
			   { $$=new node(NODE_ATTRIBUTE, $1, NULL, NULL, NULL, ATT_CHAR); }
			| NUMBER
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_INT, $1); } 
			| FLOATVAL
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_FLOAT); 
			     $$->fval=$1; }
			| MONEYVAL
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_MONEY); 
			     $$->fval=$1; }
			| DATEVAL
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_DATE);
			     $$->dval=$1; }
			| TNULL
			   { $$=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_NULL);}
			;

select_statement	: SELECT select_clause FROM from_clause where_clause
			  ordergroup
			  {
			     node *wherenode=new node(NODE_WHERE, $5, $6);
			     node *fromnode=new node(NODE_FROM, $4, wherenode);
			     $$=new node(NODE_SELECT,$2, fromnode);
                          }
			;
ordergroup		:
			   { $$=new node(NODE_GROUP, (node*)NULL, NULL);}
			| order_clause
			   { $$=new node(NODE_ORDER, $1, NULL); }
			| group_clause
			   { $$=new node(NODE_GROUP, $1, NULL); }

select_clause		: STAR
			   { $$=new node(NODE_STAR, (char*)NULL); }
			| select_clause_list
			;
select_clause_list	: select_element
			   { $$=new node(NODE_MORE, $1); }
			| select_element COMMA select_clause_list 
			   { $$=new node(NODE_MORE, $1, $3); }
			;
select_element		: column_expression
			   { $$=new node(NODE_COLEXP, $1); }
			| column_expression AS IDENTIFIER
			   { $$=new node(NODE_COLEXP, $3, NULL, $1); }
			;
column_expression	: column_function OPENPAREN func_ident CLOSEPAREN
			   { $$=new node(NODE_COLFUNC, NULL, NULL, $3, NULL, $1); }
			| attribute_expression
			;
column_function		: AVG { $$=FUNC_AVG; }
			| COUNT { $$=FUNC_COUNT; }
			| MAX { $$=FUNC_MAX; }
			| MIN { $$=FUNC_MIN; }
			| SUM { $$=FUNC_SUM; }
			;
func_ident		: STAR
			   { $$=new node(NODE_STAR, (char*)NULL); }
			| attribute_expression
			;
attribute_expression	: attribute
			| attribute_expression PLUS attribute_expression
			   { $$=new node(NODE_ADD, $1, $3); }
			| attribute_expression MINUS attribute_expression
			   { $$=new node(NODE_SUBSTRACT, $1, $3); }
			| attribute_expression SLASH attribute_expression
			   { $$=new node(NODE_DIVIDE, $1, $3); }
			| attribute_expression STAR attribute_expression
			   { $$=new node(NODE_MULTIPLY, $1, $3); }
			| MINUS attribute_expression  %prec STAR
			   { $$=new node(NODE_UMINUS, $2); }
			| PLUS attribute_expression  %prec STAR
			   { $$=new node(NODE_UPLUS, $2); }
			| FUNCTION OPENPAREN attribute_expression CLOSEPAREN
			   { $$=new node(NODE_FUNCTION, NULL, NULL, $3, NULL, $1); }
			| procedure OPENPAREN CLOSEPAREN
			   { $$=$1; }
			| insert_value
			| OPENPAREN attribute_expression CLOSEPAREN
			   { $$=$2; }
			;
procedure		: DATE
			   /*{ $$=new node(NODE_PROCEDURE, NULL, NULL, NULL, NULL, FUNC_DATE); }*/
			   { $$=new node(NODE_PROCEDURE, FUNC_DATE); }
			| ROWNUM
			   { $$=new node(NODE_PROCEDURE, FUNC_ROWNUM); }
			;
attribute		: IDENTIFIER
			   { $$=new node(NODE_IDENT, $1); }
			| IDENTIFIER DOT IDENTIFIER
			   { $$=new node(NODE_IDENT, $3, $1); }
			;

from_clause		: from_item
			   { $$=new node(NODE_MORE, $1); }
			| from_item COMMA from_clause
			   { $$=new node(NODE_MORE, $1, $3); }
			;
from_item		: IDENTIFIER
			   { $$=new node(NODE_IDENT, $1); }
			| IDENTIFIER IDENTIFIER
			   { $$=new node(NODE_IDENT, $1, $2); }
			;

where_clause		:
			   { $$=NULL; }
			| WHERE condition
			   { $$=$2; }
			;
condition		: attribute_expression EQUALS attribute_expression
			   { $$=new node(NODE_EQUALS, $1, $3); }
			| attribute_expression LESS attribute_expression
			   { $$=new node(NODE_LESS, $1, $3); }
			| attribute_expression MORE attribute_expression
			   { $$=new node(NODE_MORE, $1, $3); }
			| attribute_expression LEQ attribute_expression
			   { $$=new node(NODE_LEQ, $1, $3); }
			| attribute_expression MOQ attribute_expression
			   { $$=new node(NODE_MOQ, $1, $3); }
			| attribute_expression NEQ attribute_expression
			   { $$=new node(NODE_NEQ, $1, $3); }
			| OPENPAREN condition CLOSEPAREN
			   { $$=$2; }
			| condition AND condition
			   { $$=new node(NODE_AND,$1,$3); }
			| condition OR condition
			   { $$=new node(NODE_OR,$1,$3); }
			| NOT condition
			   { $$=new node(NODE_NOT, $2); }
			| attribute IS TNULL
			   { $$=new node(NODE_NULL, $1); }
			| attribute IS NOT TNULL
			   { $$=new node(NODE_NOT_NULL, $1); }
			| attribute LIKE STRING
			   { $$=new node(NODE_LIKE, $3, NULL, $1); }
			| attribute_expression IN OPENPAREN insert_values
			   CLOSEPAREN
			   { $$=new node(NODE_INLIST, $1, $4); }
			| attribute_expression BETWEEN attribute_expression
			   AND attribute_expression
			   {
			      node *bet=new node(NODE_BETWEEN, $3, $5);
			      $$=new node(NODE_BETWEEN, $1, bet);
			   }
			| EXISTS OPENPAREN select_statement CLOSEPAREN
			   { $$=new node(NODE_EXISTS, $3); }
			| attribute_expression IN OPENPAREN select_statement
			  CLOSEPAREN
			   { $$=new node(NODE_IN, $1, $4); }
			;

order_clause		: ORDER BY attributeorder_list 
			   { $$=$3; }
			;
group_clause		: GROUP BY attributeexp_list
			   { $$=$3; }
			;
attributeorder_list	: attributeorder_exp
			   { $$=new node(NODE_MORE, $1, NULL); }
			| attributeorder_exp COMMA attributeorder_list
			   { $$=new node(NODE_MORE, $1, $3); }
			;
attributeorder_exp	: attribute_expression
			   { $$=new node(NODE_ORDER, $1); $$->num1=1; }
			| ASCENDING attribute_expression
			   { $$=new node(NODE_ORDER, $2); $$->num1=1; }
			| DESCENDING attribute_expression
			   { $$=new node(NODE_ORDER, $2); $$->num1=-1; }
			;
attributeexp_list	: attribute_expression
			   { $$=new node(NODE_MORE, $1, NULL); }
			| attribute_expression COMMA attributeexp_list
			   { $$=new node(NODE_MORE, $1, $3); }
			;
delete_statement	: DELETE FROM from_item where_clause
			   { 
			      node *group=new node(NODE_GROUP, (node*)NULL);
			      node *where=new node(NODE_WHERE, $4, group);
                              node *from=new node(NODE_MORE, $3);
                              from=new node(NODE_FROM, from, where);
                              $$=new node(NODE_DELETE, NULL, from);
                           }
			;

update_statement	: UPDATE from_item SET IDENTIFIER EQUALS
			  attribute_expression where_clause
			   {
			      node *group=new node(NODE_GROUP, (node*)NULL);
			      node *where=new node(NODE_WHERE, $7, group);
                              node *from=new node(NODE_MORE, $2);
			      node *att=new node(NODE_MORE, new
			         node(NODE_COLEXP, $6), new node(NODE_MORE,
                                 new node(NODE_COLEXP,new node(NODE_IDENT,
                                 $4))));
                              from=new node(NODE_FROM, from, where);
                              $$=new node(NODE_UPDATE, att, from);
			   }
			;
grant_statement		: GRANT grant_revoke_body
			   { $$=new node(NODE_GRANT, $2); }
			;
revoke_statement	: REVOKE grant_revoke_body
			   { $$=new node(NODE_REVOKE, $2); }
			;
grant_revoke_body	: gritems ON ident_list tofrom grantwho with_grant
			   { $$=new node(NODE_GRANTREVOKE, $3, $5);
                             $$->num1=$1, $$->num2=$6; }
			;
grantwho		: ALL
			   { $$=new node(NODE_MORE,strdup("all")); }
			| ident_list
			;
tofrom			: TO
			| FROM
			;
gritems			: gritem
			| gritems COMMA gritem
			   { $$|=$3; }
			;
gritem			: SELECT
			   { $$=GRANTSELECT; }
			| INSERT
			   { $$=GRANTINSERT; }
			| DELETE
			   { $$=GRANTDELETE; }
			| UPDATE
			   { $$=GRANTUPDATE; }
			| DROP
			   { $$=GRANTDROP; }
			| ALL
			   { $$=GRANTSELECT|GRANTINSERT|GRANTDELETE|GRANTUPDATE|
                              GRANTDROP; }
			| ALL PRIVILEGES
			   { $$=GRANTSELECT|GRANTINSERT|GRANTDELETE|GRANTUPDATE|
                              GRANTDROP; }
			;
with_grant		:
			   { $$=0; }
			| WITH GRANT OPTION
			   { $$=1; }
			;
%%
void yyerror(char *s)
{
   extern int lines;
   char buf[100];
   sprintf(buf,"Line %d: %s",lines,s);
   errorstrings[nerrors++]=strdup(buf);
}

node::node(nodetype t, char *_s1, char *_s2, node *_n1, node *_n2, int _num1, int _num2)
{
   type=t, n1=_n1, n2=_n2, s1=_s1, s2=_s2, num1=_num1, num2=_num2;
}
node::node(nodetype t, int _num1, int _num2)
{
   type=t, num1=_num1, num2=_num2, n1=NULL, n2=NULL, s1=NULL, s2=NULL;
}
node::node(nodetype t, node *_n1, node *_n2)
{
   type=t, n1=_n1, n2=_n2, s1=NULL, s2=NULL, num1=num2=0;
}
node::~node()
{
   if (s1) delete s1;
   if (s2) delete s2;
   if (n1) delete n1;
   if (n2) delete n2;
}
