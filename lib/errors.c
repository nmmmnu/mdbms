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

#include "errors.h"

char *errors[]=
{
   "No error",
   "Page not found",
   "Read error",
   "Write error",
   "Open error",
   "No storage files", 
   "Bad argument",
   "Database exists",
   "Table exists",
   "No such database",
   "No such table in database",
   "Too many values",
   "No current database",
   "Type incompatible in insert",
   "Value NULL not allowed",
   "Not supported",
   "No such attribute in table",
   "Mathematic error",
   "Can't mix aggregate and non aggregate expressions",
   "'*' only allowed in COUNT()",
   "Invalid parameter",
   "Compare error",
   "Table not empty",
   "Search pending",
   "Parse error",
   "Can't modify primary key",
   "More values needed",
   "Duplicate primary key",
   "Subselect:When using expressions, one column should be selected",
   "Already a primary key",
   "More attributes needed",
   "Too many attributes",
   "Types incompatible",
   "Already a foreign key",
   "Can't set attribute to NULL",
   "Can't refer to table, it has no primary key",
   "Permission denied",
   "User exists",
   "Invalid user/password",
   "No user selected",
   "Convert error",
   "Database not empty",
   "Database in use"
};
