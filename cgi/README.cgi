General

Mgen is a cgi-bin that parses a special HTML file and filters and executes
special database commands in the html file.
Mgen can be called as a cgi-bin (either pre or post); the variable 'file'
should be passed to the program and should contain the mhtml file to be parsed.
The file variable is checked first. It's not accepted if it contains ".." or
if it starts with a slash.

Here's a HTML example to call the cgi-bin:

   <form method=post action="./mgen.cgi">
      <input type=hidden name=file value="myfile.mhtml">
      <input type=submit>
   </form>

Variables passed to the cgi-bin can be retrieved as normal variables in the
mhtml file ($test, $file, etc). There are some extra variables available:

$NOW  : contains the current date and time in compact format
$DATE : contains the current date in standard format (like '19 sep 1997')
$TIME : contains the current time in standard format (like 10:12:01)
$NUM_ROWS : contains the resulting rows after a query has been executed

All lines in the file will be copied, except the special mdbms commands
which all look like this:

<! mdbms <command> >

Below is a summary of commands that can be used:

CONNECT
=============================================================================
connect
connect <host>
connect <host> <port>

   Will connect to the mdbms database at localhost or at <host> <port>.

   Examples:
   <!mdbms connect>
   <!mdbms connect "mdbms.marty.net">
   <!mdbms connect "mdbms.marty.net" 2225>


CRYPT
=============================================================================
crypt <variable>
   Will crypt the contents of <variable> using DES encryption.
   The resulting contents can be used for password authentification.

   Examples:
   <! mdbms crypt $var>      : Will crypt the contents of $var


DATABASE
=============================================================================
database <database>
   Will open the database <database>. Before use, the CONNECT statement
   should be used.

   Examples:
   <! mdbms database test> : Will open database "test".


EXEC
=============================================================================
exec <variable>=<string>
exec <string>
   Will execute <string> in a subshell, possibly storing the return value
   of the command into <variable>. Note that when a result code is needed,
   the statement is executed in a different way, and text printed at standard
   out is not copied. ( exec $test="cat /etc/passwd" will not show the password
   file)

   Examples:
   <! mdbms exec "cat /etc/passwd">     : Will show the password file
   <! mdbms exec $test="test -d /home"> : Will check if /home is a directory
                                          and store the result in $test.


FREE
=============================================================================
free <handle>
   Will drop all data returned by query <handle>.

   Examples:
   <! mdbms free q1>


GETROW
=============================================================================
getrow <handle>
   Will get the first row, returned by query <handle> and store the attributes
   into $0, $1, $2, ...
   Subsequent calls to GETROW will get more rows.

   Example:
   <!mdbms getrow q1>
   <!mdbms print "Your name is: $0">


INCLUDE
=============================================================================
include <string>
   Will include the file <string>. Include files can contain mdbms codes and
   other includes.

   Example
   <!mdbms include "header.mhtml">


IF THEN ELSE
=============================================================================
if <expression>
then
   [ifpart]
[else
   [elsepart]
]
fi

   Checks for <expression>. When evaluated true, the [ifpart] is executed.
   Otherwise the [elsepart] is executed. These parts can contain HTML code
   as well as other MDBMS statements, even other IF's.

   Examples:
   <! mdbms if $var<3>
   <! mdbms then>
      HTML CODE
      <! mdbms print "$var">
   <! mdbms else>
      HTML CODE
      <! mdbms print "false>
   <! mdbms fi>


PIPE
=============================================================================
pipe <variable>=<string>
   Will execute the command <string> and store it's output in <variable>.
   If the command returns only 1 line of output, the final \n is removed
   from the output.

   Examples:
   <! mdbms pipe $calendar="cal">
   <! mdbms print "Below is a calendar:">
   <! mdbms print $calendar>


PRINT
=============================================================================
print <string>
   Will print <string>. Strings can contain variables, which will be
   substituted.
   
   Examples:
   <! mdbms print "Amount of rows:$NUM_ROWS">


PRINT_ROWS
=============================================================================
print_rows <handle> <string>
   Will print <string> exactly once, for every row returned by query <handle>.
   In order to acquire a handle, the query must first be send to the database
   with the QUERY command.
   The attributes from the query are put into the variables $0, $1, $2, ...
   The amount of rows, return by the query can be retrieved with the
   variable $NUM_ROWS.
   
   Examples:
   <! mdbms print_rows q1 "Name: $0, Age: $3">
   <! mdbms print "Amount of rows: $NUM_ROWS">


PRINT_ATTRIBUTES
=============================================================================
print_attributes <handle> <prestring> <midstring> <poststring>
   Will print all attributes from the query <handle>.
   <prestring> and <poststring> are printed before and after every row.
   <midstring> is printed for every attribute in the query. $0 in this string
   is substituted for the value of the attribute.

   Examples:
   <! mdbms print_attributes q1 "<tr>" "<td>$0</td>" "</tr>">


PRINT_HEADER
=============================================================================
print_header <handle> <string>
   Will print the header names of query <handle>.
   For every column in the resulting table <string> is printed. In this
   string $0 is substituted for the value.

   Examples:
   <! mdbms print_header q1 "<td>$0</td>">


QUERY
=============================================================================
query <string> <handle>
query <variable> <handle>
   Will send the SQL query <string> to the database, using handle <handle>.
   The handle is used later to refer to this query.
   Data from the query can be printed with PRINT_ROWS or interpreted with
   GETROW.

   Examples:
   <! mdbms query "select max(id) from users;" q1>
   <! mdbms query $queryvar q1>


SET
=============================================================================
set <variable>=<expression>
  Will set the contents of <variable> to <expression>.

  Examples:
  <! mdbms set $var=5+5>      : Will put "10" into $var
  <! mdbms set $var="test"+4> : Will put "test4" into $var
  <! mdbms set $var=(4+2)*2>  : Will put "12" into $var
  <! mdbms set $var="test"+$a>: Will concatenate the contents of a to "test",
                                an put the result into $var


SETDEFAULT
=============================================================================
setdefault <handle> <string>
  This command can be used to move one record from the query <handle> to the
  top. This command is useful if you want to implement selection boxes, with
  a default value without the usage of the HTML "default" tag. The <string>
  is used to search through the first attribute of the records. If the
  first attribute of a record matches <string>, it is moved to the top.

  Example:
  <! mdbms query "select id, name, telephone from customers" q1>
  <! mdbms setdefault q1 "$thiscustomer">
  <select>
     <! mdbms print_rows q1 "<select value=$0>$1, $2">
  </select>


OPENTCP
=============================================================================
opentcp <handle>=<string> <port>
opentcp <handle>=<string> <string>
   This command will open a TCP connection to host <string> port <port>.
   The handle can be used later to read and write data with readtcp and
   writetcp.

   Example:
   <! mdbms opentcp mailport="localhost" 25>

   <! mdbms opentcp mailport="$myhost" "$myport">


READTCP
=============================================================================
readtcp <handle> <variable>
   This command reads data from connection <handle> and places it in <variable>.

   Example:
   <! mdbms readtcp myconnection $var>


WRITETCP
=============================================================================
writetcp <handle> <string>
   This command writes <string> to connection <handle>. The string send to
   the TCP port will be terminated by a \r\n.

   Example:
   <! mdbms writetcp myconnection "user $user">


USER
=============================================================================
user <userfile>
   Will attempt to switch to a different user. The argument to user should
   be a filename. This file should look like this:

   user <user>
   password <pwd>

   Example:
   <! mdbms user "/etc/user1">
