
/*  A Bison parser, made from parser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	IDENTIFIER	257
#define	STRING	258
#define	STRING1	259
#define	NUMBER	260
#define	FUNCTION	261
#define	FLOATVAL	262
#define	MONEYVAL	263
#define	DATEVAL	264
#define	CREATE	265
#define	AUTHORIZATION	266
#define	DATABASE	267
#define	DROP	268
#define	USER	269
#define	PASSWORD	270
#define	SET	271
#define	TABLE	272
#define	TNULL	273
#define	INTEGER	274
#define	OPEN	275
#define	VARCHAR	276
#define	DATE	277
#define	MONEY	278
#define	FLOAT	279
#define	SYNC	280
#define	INSERT	281
#define	INTO	282
#define	VALUES	283
#define	CHAR	284
#define	HELP	285
#define	SELECT	286
#define	FROM	287
#define	WHERE	288
#define	ORDER	289
#define	GROUP	290
#define	BY	291
#define	HAVING	292
#define	AS	293
#define	IS	294
#define	IN	295
#define	PRIVILEGES	296
#define	SUM	297
#define	COUNT	298
#define	AVG	299
#define	MIN	300
#define	MAX	301
#define	LESS	302
#define	MORE	303
#define	LEQ	304
#define	MOQ	305
#define	NEQ	306
#define	VIEW	307
#define	UPDATE	308
#define	PRIMARY	309
#define	KEY	310
#define	OPENPAREN	311
#define	CLOSEPAREN	312
#define	SEMICOLON	313
#define	EQUALS	314
#define	COMMA	315
#define	DOT	316
#define	CASCADE	317
#define	RESTRICT	318
#define	DELETE	319
#define	BETWEEN	320
#define	EXISTS	321
#define	FOREIGN	322
#define	REFERENCES	323
#define	ON	324
#define	GRANT	325
#define	WITH	326
#define	OPTION	327
#define	REVOKE	328
#define	TO	329
#define	ALL	330
#define	ASCENDING	331
#define	DESCENDING	332
#define	ROWNUM	333
#define	LIKE	334
#define	PLUS	335
#define	MINUS	336
#define	STAR	337
#define	SLASH	338
#define	OR	339
#define	AND	340
#define	NOT	341

#line 19 "parser.y"

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

#line 33 "parser.y"
typedef union
{
   char *s_value;
   int i_value;
   double f_value;
   time_t d_value;
   node *n;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		295
#define	YYFLAG		-32768
#define	YYNTBASE	88

#define YYTRANSLATE(x) ((unsigned)(x) <= 341 ? yytranslate[x] : 146)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     9,    11,    12,    14,    16,    18,    20,
    22,    24,    26,    28,    30,    32,    34,    37,    39,    42,
    47,    52,    55,    59,    63,    69,    73,    75,    79,    83,
    87,    89,    95,    97,    99,   102,   103,   109,   111,   114,
   123,   124,   128,   130,   132,   135,   137,   141,   143,   147,
   151,   154,   156,   161,   163,   165,   167,   169,   171,   174,
   179,   184,   185,   187,   189,   192,   197,   202,   210,   212,
   216,   218,   221,   224,   226,   228,   230,   232,   234,   236,
   238,   245,   246,   248,   250,   252,   254,   256,   260,   262,
   266,   271,   273,   275,   277,   279,   281,   283,   285,   287,
   289,   293,   297,   301,   305,   308,   311,   316,   320,   322,
   326,   328,   330,   332,   336,   338,   342,   344,   347,   348,
   351,   355,   359,   363,   367,   371,   375,   379,   383,   387,
   390,   394,   399,   403,   409,   415,   420,   426,   430,   434,
   436,   440,   442,   445,   448,   450,   454,   459,   467,   470,
   473,   480,   482,   484,   486,   488,   490,   494,   496,   498,
   500,   502,   504,   506,   509,   510
};

static const short yyrhs[] = {    89,
     0,    91,    90,     0,    89,    91,    90,     0,    59,     0,
     0,    94,     0,    93,     0,   109,     0,   111,     0,    92,
     0,   136,     0,   112,     0,   116,     0,   137,     0,   138,
     0,   139,     0,    31,     3,     0,    26,     0,    21,     3,
     0,    15,     3,    16,     3,     0,    15,     3,    16,     4,
     0,    15,     3,     0,    11,    13,    96,     0,    11,    18,
    97,     0,    11,    18,     3,    39,   116,     0,    11,    15,
    95,     0,     3,     0,     3,    16,     3,     0,     3,    16,
     4,     0,     3,    12,     3,     0,     3,     0,     3,    57,
   105,    98,    58,     0,    99,     0,   100,     0,    99,   100,
     0,     0,    55,    56,    57,   104,    58,     0,   101,     0,
   101,   100,     0,    68,    56,    57,   104,    58,    69,     3,
   102,     0,     0,    70,    65,   103,     0,    64,     0,    63,
     0,    17,    19,     0,     3,     0,     3,    61,   104,     0,
   106,     0,   106,    61,   105,     0,     3,   107,   108,     0,
     3,   107,     0,    20,     0,    22,    57,     6,    58,     0,
    23,     0,    24,     0,    25,     0,    30,     0,    19,     0,
    87,    19,     0,    14,    18,     3,   110,     0,    14,    13,
     3,   110,     0,     0,    63,     0,    64,     0,    17,     3,
     0,    17,     3,    60,     3,     0,    17,     3,    60,     4,
     0,    27,    28,     3,    29,    57,   113,    58,     0,   114,
     0,   114,    61,   113,     0,   115,     0,    82,     6,     0,
    82,     8,     0,     4,     0,     5,     0,     6,     0,     8,
     0,     9,     0,    10,     0,    19,     0,    32,   118,    33,
   127,   129,   117,     0,     0,   131,     0,   132,     0,    83,
     0,   119,     0,   120,     0,   120,    61,   119,     0,   121,
     0,   121,    39,     3,     0,   122,    57,   123,    58,     0,
   124,     0,    45,     0,    44,     0,    47,     0,    46,     0,
    43,     0,    83,     0,   124,     0,   126,     0,   124,    81,
   124,     0,   124,    82,   124,     0,   124,    84,   124,     0,
   124,    83,   124,     0,    82,   124,     0,    81,   124,     0,
     7,    57,   124,    58,     0,   125,    57,    58,     0,   115,
     0,    57,   124,    58,     0,    23,     0,    79,     0,     3,
     0,     3,    62,     3,     0,   128,     0,   128,    61,   127,
     0,     3,     0,     3,     3,     0,     0,    34,   130,     0,
   124,    60,   124,     0,   124,    48,   124,     0,   124,    49,
   124,     0,   124,    50,   124,     0,   124,    51,   124,     0,
   124,    52,   124,     0,    57,   130,    58,     0,   130,    86,
   130,     0,   130,    85,   130,     0,    87,   130,     0,   126,
    40,    19,     0,   126,    40,    87,    19,     0,   126,    80,
     4,     0,   124,    41,    57,   113,    58,     0,   124,    66,
   124,    86,   124,     0,    67,    57,   116,    58,     0,   124,
    41,    57,   116,    58,     0,    35,    37,   133,     0,    36,
    37,   135,     0,   134,     0,   134,    61,   133,     0,   124,
     0,    77,   124,     0,    78,   124,     0,   124,     0,   124,
    61,   135,     0,    65,    33,   128,   129,     0,    54,   128,
    17,     3,    60,   124,   129,     0,    71,   140,     0,    74,
   140,     0,   143,    70,   104,   142,   141,   145,     0,    76,
     0,   104,     0,    75,     0,    33,     0,   144,     0,   143,
    61,   144,     0,    32,     0,    27,     0,    65,     0,    54,
     0,    14,     0,    76,     0,    76,    42,     0,     0,    72,
    71,    73,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    75,    78,    80,    83,    84,    86,    88,    90,    92,    94,
    96,    97,    98,    99,   100,   101,   102,   104,   108,   112,
   114,   116,   120,   122,   124,   126,   129,   131,   133,   136,
   138,   141,   145,   147,   149,   152,   153,   156,   158,   161,
   165,   166,   169,   171,   173,   177,   179,   182,   185,   189,
   191,   194,   196,   198,   200,   202,   204,   207,   209,   213,
   215,   218,   220,   222,   226,   228,   230,   234,   238,   240,
   243,   244,   246,   250,   252,   254,   256,   259,   262,   265,
   269,   277,   279,   281,   284,   286,   288,   290,   293,   295,
   298,   300,   302,   303,   304,   305,   306,   308,   310,   312,
   313,   315,   317,   319,   321,   323,   325,   327,   329,   330,
   333,   336,   339,   341,   345,   347,   350,   352,   356,   358,
   361,   363,   365,   367,   369,   371,   373,   375,   377,   379,
   381,   383,   385,   387,   390,   396,   398,   403,   406,   409,
   411,   414,   416,   418,   421,   423,   426,   436,   450,   453,
   456,   460,   462,   464,   465,   467,   468,   471,   473,   475,
   477,   479,   481,   484,   488,   490
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","IDENTIFIER",
"STRING","STRING1","NUMBER","FUNCTION","FLOATVAL","MONEYVAL","DATEVAL","CREATE",
"AUTHORIZATION","DATABASE","DROP","USER","PASSWORD","SET","TABLE","TNULL","INTEGER",
"OPEN","VARCHAR","DATE","MONEY","FLOAT","SYNC","INSERT","INTO","VALUES","CHAR",
"HELP","SELECT","FROM","WHERE","ORDER","GROUP","BY","HAVING","AS","IS","IN",
"PRIVILEGES","SUM","COUNT","AVG","MIN","MAX","LESS","MORE","LEQ","MOQ","NEQ",
"VIEW","UPDATE","PRIMARY","KEY","OPENPAREN","CLOSEPAREN","SEMICOLON","EQUALS",
"COMMA","DOT","CASCADE","RESTRICT","DELETE","BETWEEN","EXISTS","FOREIGN","REFERENCES",
"ON","GRANT","WITH","OPTION","REVOKE","TO","ALL","ASCENDING","DESCENDING","ROWNUM",
"LIKE","PLUS","MINUS","STAR","SLASH","OR","AND","NOT","start","statement_list",
"semi","statement","open_statement","user_statement","create_statement","create_user",
"create_database","create_table","key_clause","primary_clause","foreign_clause",
"foreign_key","foreign_rules","delete_rule","ident_list","table_def","table_def_entry",
"attrtype","nullspec","drop_statement","cascade_or_restrict","set_statement",
"insert_statement","insert_values","insert_value_neg","insert_value","select_statement",
"ordergroup","select_clause","select_clause_list","select_element","column_expression",
"column_function","func_ident","attribute_expression","procedure","attribute",
"from_clause","from_item","where_clause","condition","order_clause","group_clause",
"attributeorder_list","attributeorder_exp","attributeexp_list","delete_statement",
"update_statement","grant_statement","revoke_statement","grant_revoke_body",
"grantwho","tofrom","gritems","gritem","with_grant", NULL
};
#endif

static const short yyr1[] = {     0,
    88,    89,    89,    90,    90,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    91,    91,    91,    91,    92,    93,
    93,    93,    94,    94,    94,    94,    95,    95,    95,    96,
    96,    97,    98,    98,    98,    99,    99,   100,   100,   101,
   102,   102,   103,   103,   103,   104,   104,   105,   105,   106,
   106,   107,   107,   107,   107,   107,   107,   108,   108,   109,
   109,   110,   110,   110,   111,   111,   111,   112,   113,   113,
   114,   114,   114,   115,   115,   115,   115,   115,   115,   115,
   116,   117,   117,   117,   118,   118,   119,   119,   120,   120,
   121,   121,   122,   122,   122,   122,   122,   123,   123,   124,
   124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
   125,   125,   126,   126,   127,   127,   128,   128,   129,   129,
   130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
   130,   130,   130,   130,   130,   130,   130,   131,   132,   133,
   133,   134,   134,   134,   135,   135,   136,   137,   138,   139,
   140,   141,   141,   142,   142,   143,   143,   144,   144,   144,
   144,   144,   144,   144,   145,   145
};

static const short yyr2[] = {     0,
     1,     2,     3,     1,     0,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     2,     1,     2,     4,
     4,     2,     3,     3,     5,     3,     1,     3,     3,     3,
     1,     5,     1,     1,     2,     0,     5,     1,     2,     8,
     0,     3,     1,     1,     2,     1,     3,     1,     3,     3,
     2,     1,     4,     1,     1,     1,     1,     1,     2,     4,
     4,     0,     1,     1,     2,     4,     4,     7,     1,     3,
     1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
     6,     0,     1,     1,     1,     1,     1,     3,     1,     3,
     4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     3,     3,     3,     3,     2,     2,     4,     3,     1,     3,
     1,     1,     1,     3,     1,     3,     1,     2,     0,     2,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
     3,     4,     3,     5,     5,     4,     5,     3,     3,     1,
     3,     1,     2,     2,     1,     3,     4,     7,     2,     2,
     6,     1,     1,     1,     1,     1,     3,     1,     1,     1,
     1,     1,     1,     2,     0,     3
};

static const short yydefact[] = {     0,
     0,     0,     0,     0,     0,    18,     0,     0,     0,     0,
     0,     0,     0,     1,     5,    10,     7,     6,     8,     9,
    12,    13,    11,    14,    15,    16,     0,     0,     0,     0,
     0,    22,    65,    19,     0,    17,   113,    74,    75,    76,
     0,    77,    78,    79,    80,   111,    97,    94,    93,    96,
    95,     0,   112,     0,     0,    85,   109,     0,    86,    87,
    89,     0,    92,     0,   100,   117,     0,     0,   162,   159,
   158,   161,   160,   163,   149,     0,   156,   150,     5,     4,
     2,    31,    23,    27,    26,     0,    24,    62,    62,     0,
     0,     0,     0,     0,     0,   106,   105,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   118,     0,   119,   164,
     0,     0,     3,     0,     0,     0,     0,    63,    64,    61,
    60,    20,    21,    66,    67,     0,   114,     0,   110,   119,
   115,    88,    90,    98,     0,    99,   101,   102,   104,   103,
   108,     0,     0,   147,   157,    46,     0,    30,    28,    29,
    25,     0,    36,    48,     0,   107,    82,     0,    91,     0,
     0,     0,     0,     0,   100,   120,     0,   155,   154,     0,
    52,     0,    54,    55,    56,    57,    51,     0,     0,     0,
    33,    34,    38,     0,     0,     0,    69,    71,     0,     0,
    81,    83,    84,   116,   119,     0,     0,     0,   130,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    47,   152,   153,   165,     0,    58,     0,    50,     0,
     0,    32,    35,    39,    49,    72,    73,    68,     0,     0,
     0,   148,   127,     0,     0,   122,   123,   124,   125,   126,
   121,     0,   131,     0,   133,   129,   128,     0,   151,     0,
    59,     0,     0,    70,     0,     0,   142,   138,   140,   145,
   139,   136,     0,     0,     0,   132,     0,    53,     0,     0,
   143,   144,     0,     0,   134,   137,   135,   166,    37,     0,
   141,   146,     0,    41,     0,    40,     0,     0,    44,    43,
    42,    45,     0,     0,     0
};

static const short yydefgoto[] = {   293,
    14,    81,    15,    16,    17,    18,    85,    83,    87,   180,
   181,   182,   183,   286,   291,   147,   153,   154,   177,   219,
    19,   120,    20,    21,   186,   187,    57,    22,   191,    58,
    59,    60,    61,    62,   135,   164,    64,    65,   130,   131,
   144,   166,   192,   193,   258,   259,   261,    23,    24,    25,
    26,    75,   215,   170,    76,    77,   249
};

static const short yypact[] = {   264,
    90,    89,    13,    27,    30,-32768,    26,    46,   120,   109,
    99,   222,   222,   264,    79,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   147,   165,   166,   167,
   170,   156,   114,-32768,   173,-32768,   149,-32768,-32768,-32768,
   160,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   263,-32768,   263,   263,-32768,-32768,   185,-32768,   162,
   180,   175,   169,   178,-32768,   234,   226,   109,-32768,-32768,
-32768,-32768,-32768,   202,-32768,   -17,-32768,-32768,    79,-32768,
-32768,   233,-32768,   232,-32768,   -36,-32768,    47,    47,   118,
   133,   228,   252,   263,   124,-32768,-32768,   109,   181,   255,
    52,   263,   263,   263,   263,   201,-32768,   258,   240,-32768,
   222,   274,-32768,   280,   137,   256,   281,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   235,-32768,   158,-32768,   240,
   236,-32768,-32768,-32768,   231,   169,    61,    61,-32768,-32768,
-32768,   239,    19,-32768,-32768,   243,   -23,-32768,-32768,-32768,
-32768,   190,    36,   244,    60,-32768,   113,   109,-32768,   263,
    19,   237,    19,   302,   -29,    68,   274,-32768,-32768,     4,
-32768,   250,-32768,-32768,-32768,-32768,   -13,   253,   254,   261,
   225,-32768,   225,   281,     7,   266,   247,-32768,   275,   284,
-32768,-32768,-32768,-32768,    33,   265,   -38,   256,-32768,   269,
   263,   263,   263,   263,   263,   263,   263,   -10,   307,    19,
    19,-32768,-32768,-32768,   260,   316,-32768,   308,-32768,   271,
   273,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    60,   152,
   263,-32768,-32768,   276,    31,   169,   169,   169,   169,   169,
   169,    97,-32768,   314,-32768,   251,-32768,   268,-32768,   278,
-32768,   274,   274,-32768,   263,   263,   169,-32768,   279,   219,
-32768,-32768,   283,   297,   263,-32768,   285,-32768,   298,   299,
   169,   169,   152,   263,-32768,-32768,   169,-32768,-32768,   290,
-32768,-32768,   357,   291,   300,-32768,    55,   344,-32768,-32768,
-32768,-32768,   364,   366,-32768
};

static const short yypgoto[] = {-32768,
-32768,   288,   355,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  -164,-32768,-32768,-32768,  -165,   186,-32768,-32768,-32768,
-32768,   282,-32768,-32768,  -217,-32768,  -151,  -115,-32768,-32768,
   277,-32768,-32768,-32768,-32768,    -9,-32768,  -129,   214,    21,
  -122,   -64,-32768,-32768,   100,-32768,   101,-32768,-32768,-32768,
-32768,   361,-32768,-32768,-32768,   267,-32768
};


#define	YYLAST		386


static const short yytable[] = {    63,
   151,   212,   116,   188,   214,   217,   146,   157,   243,   168,
   208,   254,   226,   165,   227,    32,   223,   263,   224,   233,
   117,    37,    38,    39,    40,    41,    42,    43,    44,    33,
    67,   165,    34,   165,    38,    39,    40,    45,    42,    43,
    44,    46,    95,   111,    96,    97,   210,   211,    36,    45,
   209,   169,   112,    35,    37,    38,    39,    40,    41,    42,
    43,    44,     9,    38,    39,    40,   143,    42,    43,    44,
    45,   288,   232,   218,    46,   161,   244,   188,    45,   213,
   165,   165,   234,   188,   128,   162,   269,   270,   109,    63,
   178,   136,   137,   138,   139,   140,   197,    53,   199,    54,
    55,    30,    27,   179,    28,   163,    31,    29,    52,   118,
   119,    66,   185,   102,   103,   104,   105,   289,   290,   264,
   122,   123,    37,    38,    39,    40,    41,    42,    43,    44,
    53,    68,    54,    55,   134,   124,   125,    80,    45,   149,
   150,   185,    46,   104,   105,   246,   247,   189,   190,    82,
   195,   196,   210,   211,    37,    38,    39,    40,    41,    42,
    43,    44,    47,    48,    49,    50,    51,    84,    86,    88,
    45,    90,    89,    91,    46,    92,    52,   102,   103,   104,
   105,   129,   265,    37,    38,    39,    40,    41,    42,    43,
    44,   236,   237,   238,   239,   240,   241,   242,    53,    45,
    54,    55,    56,    46,   102,   103,   104,   105,    52,   171,
    93,   172,   173,   174,   175,   156,    94,    98,   100,   176,
   257,   260,    99,    47,    48,    49,    50,    51,   255,   256,
    53,   101,    54,    55,   106,    69,   107,    52,   102,   103,
   104,   105,   108,   110,   114,   271,   272,   115,    70,   102,
   103,   104,   105,    71,   127,   277,   126,   133,   141,    53,
   142,    54,    55,   257,   260,    37,    38,    39,    40,    41,
    42,    43,    44,   143,     1,    72,   146,     2,     3,   274,
     4,    45,   148,   152,     5,    46,    73,     9,   159,     6,
     7,   155,   179,   198,     8,     9,   158,    74,   160,   102,
   103,   104,   105,   167,   184,   200,   216,   229,   220,   221,
   245,   230,   201,   202,   203,   204,   205,    10,   222,    52,
   231,   250,   129,   228,   206,   235,   251,   252,    11,   253,
   207,   248,   266,   262,    12,   268,   211,    13,   267,   273,
   275,    53,   200,    54,    55,   102,   103,   104,   105,   201,
   202,   203,   204,   205,   276,   279,   280,   278,   283,   284,
   285,   206,   292,   294,   287,   295,   113,   207,    79,   225,
   121,   194,   281,    78,   282,   132,     0,   145,     0,     0,
     0,     0,   102,   103,   104,   105
};

static const short yycheck[] = {     9,
   116,   167,    39,   155,   170,    19,     3,   130,    19,    33,
    40,   229,     6,   143,     8,     3,   181,   235,   183,    58,
    57,     3,     4,     5,     6,     7,     8,     9,    10,     3,
    10,   161,     3,   163,     4,     5,     6,    19,     8,     9,
    10,    23,    52,    61,    54,    55,    85,    86,     3,    19,
    80,    75,    70,    28,     3,     4,     5,     6,     7,     8,
     9,    10,    32,     4,     5,     6,    34,     8,     9,    10,
    19,    17,   195,    87,    23,    57,    87,   229,    19,    76,
   210,   211,   198,   235,    94,    67,   252,   253,    68,    99,
    55,   101,   102,   103,   104,   105,   161,    79,   163,    81,
    82,    13,    13,    68,    15,    87,    18,    18,    57,    63,
    64,     3,    82,    81,    82,    83,    84,    63,    64,   235,
     3,     4,     3,     4,     5,     6,     7,     8,     9,    10,
    79,    33,    81,    82,    83,     3,     4,    59,    19,     3,
     4,    82,    23,    83,    84,   210,   211,    35,    36,     3,
   160,   161,    85,    86,     3,     4,     5,     6,     7,     8,
     9,    10,    43,    44,    45,    46,    47,     3,     3,     3,
    19,    16,     3,    60,    23,     3,    57,    81,    82,    83,
    84,    58,    86,     3,     4,     5,     6,     7,     8,     9,
    10,   201,   202,   203,   204,   205,   206,   207,    79,    19,
    81,    82,    83,    23,    81,    82,    83,    84,    57,    20,
    62,    22,    23,    24,    25,    58,    57,    33,    39,    30,
   230,   231,    61,    43,    44,    45,    46,    47,    77,    78,
    79,    57,    81,    82,    57,    14,     3,    57,    81,    82,
    83,    84,    17,    42,    12,   255,   256,    16,    27,    81,
    82,    83,    84,    32,     3,   265,    29,     3,    58,    79,
     3,    81,    82,   273,   274,     3,     4,     5,     6,     7,
     8,     9,    10,    34,    11,    54,     3,    14,    15,    61,
    17,    19,     3,     3,    21,    23,    65,    32,    58,    26,
    27,    57,    68,    57,    31,    32,    61,    76,    60,    81,
    82,    83,    84,    61,    61,    41,    57,    61,    56,    56,
     4,    37,    48,    49,    50,    51,    52,    54,    58,    57,
    37,     6,    58,    58,    60,    57,    19,    57,    65,    57,
    66,    72,    19,    58,    71,    58,    86,    74,    71,    61,
    58,    79,    41,    81,    82,    81,    82,    83,    84,    48,
    49,    50,    51,    52,    58,    58,    58,    73,    69,     3,
    70,    60,    19,     0,    65,     0,    79,    66,    14,   184,
    89,   158,   273,    13,   274,    99,    -1,   111,    -1,    -1,
    -1,    -1,    81,    82,    83,    84
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 76 "parser.y"
{ rootnode=yyvsp[0].n; ;
    break;}
case 2:
#line 79 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-1].n); ;
    break;}
case 3:
#line 81 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-1].n, yyvsp[-2].n); ;
    break;}
case 6:
#line 87 "parser.y"
{ yyval.n=new node(NODE_CREATE, yyvsp[0].n); ;
    break;}
case 7:
#line 89 "parser.y"
{ yyval.n=new node(OPEN_USER, yyvsp[0].n); ;
    break;}
case 8:
#line 91 "parser.y"
{ yyval.n=new node(NODE_DROP, yyvsp[0].n); ;
    break;}
case 9:
#line 93 "parser.y"
{ yyval.n=new node(ROOT_SET, yyvsp[0].n); ;
    break;}
case 10:
#line 95 "parser.y"
{ yyval.n=new node(NODE_OPEN, yyvsp[0].n); ;
    break;}
case 17:
#line 103 "parser.y"
{ yyval.n=new node(NODE_HELP, yyvsp[0].s_value); ;
    break;}
case 18:
#line 105 "parser.y"
{ yyval.n=new node(NODE_SYNC, (node*)NULL); ;
    break;}
case 19:
#line 109 "parser.y"
{ yyval.n=new node(NODE_IDENT, yyvsp[0].s_value); ;
    break;}
case 20:
#line 113 "parser.y"
{ yyval.n=new node(NODE_USER,yyvsp[-2].s_value,yyvsp[0].s_value); ;
    break;}
case 21:
#line 115 "parser.y"
{ yyval.n=new node(NODE_USER,yyvsp[-2].s_value,yyvsp[0].s_value); ;
    break;}
case 22:
#line 117 "parser.y"
{ yyval.n=new node(NODE_USER,yyvsp[0].s_value); ;
    break;}
case 23:
#line 121 "parser.y"
{ yyval.n=new node(NODE_CREATEDB, yyvsp[0].n); ;
    break;}
case 24:
#line 123 "parser.y"
{ yyval.n=new node(NODE_CREATETBL, yyvsp[0].n); ;
    break;}
case 25:
#line 125 "parser.y"
{ yyval.n=new node(NODE_CREATEVIEW, yyvsp[-2].s_value, NULL, yyvsp[0].n); ;
    break;}
case 26:
#line 127 "parser.y"
{ yyval.n=yyvsp[0].n; ;
    break;}
case 27:
#line 130 "parser.y"
{ yyval.n=new node(NODE_CREATEUSER, yyvsp[0].s_value); ;
    break;}
case 28:
#line 132 "parser.y"
{ yyval.n=new node(NODE_CREATEUSER, yyvsp[-2].s_value, yyvsp[0].s_value); ;
    break;}
case 29:
#line 134 "parser.y"
{ yyval.n=new node(NODE_CREATEUSER, yyvsp[-2].s_value, yyvsp[0].s_value); ;
    break;}
case 30:
#line 137 "parser.y"
{ yyval.n=new node(NODE_DB, yyvsp[-2].s_value, yyvsp[0].s_value); ;
    break;}
case 31:
#line 139 "parser.y"
{ yyval.n=new node(NODE_DB, yyvsp[0].s_value); ;
    break;}
case 32:
#line 143 "parser.y"
{ yyval.n=new node(NODE_TABLEDEF, yyvsp[-4].s_value, NULL, yyvsp[-2].n, yyvsp[-1].n); ;
    break;}
case 33:
#line 146 "parser.y"
{ yyval.n=new node(NODE_KEYS, yyvsp[0].n, NULL); ;
    break;}
case 34:
#line 148 "parser.y"
{ yyval.n=new node(NODE_KEYS, NULL, yyvsp[0].n); ;
    break;}
case 35:
#line 150 "parser.y"
{ yyval.n=new node(NODE_KEYS, yyvsp[-1].n, yyvsp[0].n); ;
    break;}
case 36:
#line 152 "parser.y"
{ yyval.n=NULL;;
    break;}
case 37:
#line 154 "parser.y"
{ yyval.n=yyvsp[-1].n; ;
    break;}
case 38:
#line 157 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n, NULL); ;
    break;}
case 39:
#line 159 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-1].n, yyvsp[0].n); ;
    break;}
case 40:
#line 163 "parser.y"
{ yyval.n=new node(NODE_FOREIGN, yyvsp[-1].s_value, NULL, yyvsp[-4].n, yyvsp[0].n); ;
    break;}
case 41:
#line 165 "parser.y"
{ yyval.n=NULL; ;
    break;}
case 42:
#line 167 "parser.y"
{ yyval.n=new node(NODE_DELRULE, yyvsp[0].i_value); ;
    break;}
case 43:
#line 170 "parser.y"
{ yyval.i_value=DELRULE_RESTRICT; ;
    break;}
case 44:
#line 172 "parser.y"
{ yyval.i_value=DELRULE_CASCADE; ;
    break;}
case 45:
#line 174 "parser.y"
{ yyval.i_value=DELRULE_SETNULL; ;
    break;}
case 46:
#line 178 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].s_value, NULL); ;
    break;}
case 47:
#line 180 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].s_value, NULL, NULL, yyvsp[0].n); ;
    break;}
case 48:
#line 183 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n);
			     yyval.n->num2=-1; ;
    break;}
case 49:
#line 186 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n);
			     yyval.n->num2=-1; ;
    break;}
case 50:
#line 190 "parser.y"
{ yyval.n=new node(NODE_TABLEENT, yyvsp[-2].s_value, NULL, yyvsp[-1].n, NULL, yyvsp[0].i_value); ;
    break;}
case 51:
#line 192 "parser.y"
{ yyval.n=new node(NODE_TABLEENT, yyvsp[-1].s_value, NULL, yyvsp[0].n, NULL, 1); ;
    break;}
case 52:
#line 195 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, ATT_INT, sizeof(int)); ;
    break;}
case 53:
#line 197 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, ATT_VARCHAR, yyvsp[-1].i_value); ;
    break;}
case 54:
#line 199 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, ATT_DATE, sizeof(int)); ;
    break;}
case 55:
#line 201 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, ATT_MONEY, sizeof(int)); ;
    break;}
case 56:
#line 203 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, ATT_FLOAT, sizeof(double)); ;
    break;}
case 57:
#line 205 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, ATT_CHAR, 1); ;
    break;}
case 58:
#line 208 "parser.y"
{ yyval.i_value=1; ;
    break;}
case 59:
#line 210 "parser.y"
{ yyval.i_value=0; ;
    break;}
case 60:
#line 214 "parser.y"
{ yyval.n=new node(NODE_IDENT,yyvsp[-1].s_value,NULL,NULL,NULL,1,yyvsp[0].i_value); ;
    break;}
case 61:
#line 216 "parser.y"
{ yyval.n=new node(NODE_DROPDB,yyvsp[-1].s_value,NULL,NULL,NULL,1,yyvsp[0].i_value); ;
    break;}
case 62:
#line 219 "parser.y"
{ yyval.i_value=1; ;
    break;}
case 63:
#line 221 "parser.y"
{ yyval.i_value=0; ;
    break;}
case 64:
#line 223 "parser.y"
{ yyval.i_value=1; ;
    break;}
case 65:
#line 227 "parser.y"
{ yyval.n=new node(NODE_SET, yyvsp[0].s_value); ;
    break;}
case 66:
#line 229 "parser.y"
{ yyval.n=new node(NODE_SET, yyvsp[-2].s_value, yyvsp[0].s_value); ;
    break;}
case 67:
#line 231 "parser.y"
{ yyval.n=new node(NODE_SET, yyvsp[-2].s_value, yyvsp[0].s_value); ;
    break;}
case 68:
#line 236 "parser.y"
{ yyval.n=new node(NODE_INSERT, yyvsp[-4].s_value, NULL, yyvsp[-1].n); ;
    break;}
case 69:
#line 239 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n, NULL); ;
    break;}
case 70:
#line 241 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 72:
#line 245 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_INT, -yyvsp[0].i_value); ;
    break;}
case 73:
#line 247 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_FLOAT); 
			     yyval.n->fval=-yyvsp[0].f_value; ;
    break;}
case 74:
#line 251 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, yyvsp[0].s_value, NULL, NULL, NULL, ATT_VARCHAR); ;
    break;}
case 75:
#line 253 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, yyvsp[0].s_value, NULL, NULL, NULL, ATT_CHAR); ;
    break;}
case 76:
#line 255 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_INT, yyvsp[0].i_value); ;
    break;}
case 77:
#line 257 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_FLOAT); 
			     yyval.n->fval=yyvsp[0].f_value; ;
    break;}
case 78:
#line 260 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_MONEY); 
			     yyval.n->fval=yyvsp[0].f_value; ;
    break;}
case 79:
#line 263 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_DATE);
			     yyval.n->dval=yyvsp[0].d_value; ;
    break;}
case 80:
#line 266 "parser.y"
{ yyval.n=new node(NODE_ATTRIBUTE, NULL, NULL, NULL, NULL, ATT_NULL);;
    break;}
case 81:
#line 271 "parser.y"
{
			     node *wherenode=new node(NODE_WHERE, yyvsp[-1].n, yyvsp[0].n);
			     node *fromnode=new node(NODE_FROM, yyvsp[-2].n, wherenode);
			     yyval.n=new node(NODE_SELECT,yyvsp[-4].n, fromnode);
                          ;
    break;}
case 82:
#line 278 "parser.y"
{ yyval.n=new node(NODE_GROUP, (node*)NULL, NULL);;
    break;}
case 83:
#line 280 "parser.y"
{ yyval.n=new node(NODE_ORDER, yyvsp[0].n, NULL); ;
    break;}
case 84:
#line 282 "parser.y"
{ yyval.n=new node(NODE_GROUP, yyvsp[0].n, NULL); ;
    break;}
case 85:
#line 285 "parser.y"
{ yyval.n=new node(NODE_STAR, (char*)NULL); ;
    break;}
case 87:
#line 289 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n); ;
    break;}
case 88:
#line 291 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 89:
#line 294 "parser.y"
{ yyval.n=new node(NODE_COLEXP, yyvsp[0].n); ;
    break;}
case 90:
#line 296 "parser.y"
{ yyval.n=new node(NODE_COLEXP, yyvsp[0].s_value, NULL, yyvsp[-2].n); ;
    break;}
case 91:
#line 299 "parser.y"
{ yyval.n=new node(NODE_COLFUNC, NULL, NULL, yyvsp[-1].n, NULL, yyvsp[-3].i_value); ;
    break;}
case 93:
#line 302 "parser.y"
{ yyval.i_value=FUNC_AVG; ;
    break;}
case 94:
#line 303 "parser.y"
{ yyval.i_value=FUNC_COUNT; ;
    break;}
case 95:
#line 304 "parser.y"
{ yyval.i_value=FUNC_MAX; ;
    break;}
case 96:
#line 305 "parser.y"
{ yyval.i_value=FUNC_MIN; ;
    break;}
case 97:
#line 306 "parser.y"
{ yyval.i_value=FUNC_SUM; ;
    break;}
case 98:
#line 309 "parser.y"
{ yyval.n=new node(NODE_STAR, (char*)NULL); ;
    break;}
case 101:
#line 314 "parser.y"
{ yyval.n=new node(NODE_ADD, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 102:
#line 316 "parser.y"
{ yyval.n=new node(NODE_SUBSTRACT, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 103:
#line 318 "parser.y"
{ yyval.n=new node(NODE_DIVIDE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 104:
#line 320 "parser.y"
{ yyval.n=new node(NODE_MULTIPLY, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 105:
#line 322 "parser.y"
{ yyval.n=new node(NODE_UMINUS, yyvsp[0].n); ;
    break;}
case 106:
#line 324 "parser.y"
{ yyval.n=new node(NODE_UPLUS, yyvsp[0].n); ;
    break;}
case 107:
#line 326 "parser.y"
{ yyval.n=new node(NODE_FUNCTION, NULL, NULL, yyvsp[-1].n, NULL, yyvsp[-3].i_value); ;
    break;}
case 108:
#line 328 "parser.y"
{ yyval.n=yyvsp[-2].n; ;
    break;}
case 110:
#line 331 "parser.y"
{ yyval.n=yyvsp[-1].n; ;
    break;}
case 111:
#line 335 "parser.y"
{ yyval.n=new node(NODE_PROCEDURE, FUNC_DATE); ;
    break;}
case 112:
#line 337 "parser.y"
{ yyval.n=new node(NODE_PROCEDURE, FUNC_ROWNUM); ;
    break;}
case 113:
#line 340 "parser.y"
{ yyval.n=new node(NODE_IDENT, yyvsp[0].s_value); ;
    break;}
case 114:
#line 342 "parser.y"
{ yyval.n=new node(NODE_IDENT, yyvsp[0].s_value, yyvsp[-2].s_value); ;
    break;}
case 115:
#line 346 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n); ;
    break;}
case 116:
#line 348 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 117:
#line 351 "parser.y"
{ yyval.n=new node(NODE_IDENT, yyvsp[0].s_value); ;
    break;}
case 118:
#line 353 "parser.y"
{ yyval.n=new node(NODE_IDENT, yyvsp[-1].s_value, yyvsp[0].s_value); ;
    break;}
case 119:
#line 357 "parser.y"
{ yyval.n=NULL; ;
    break;}
case 120:
#line 359 "parser.y"
{ yyval.n=yyvsp[0].n; ;
    break;}
case 121:
#line 362 "parser.y"
{ yyval.n=new node(NODE_EQUALS, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 122:
#line 364 "parser.y"
{ yyval.n=new node(NODE_LESS, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 123:
#line 366 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 124:
#line 368 "parser.y"
{ yyval.n=new node(NODE_LEQ, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 125:
#line 370 "parser.y"
{ yyval.n=new node(NODE_MOQ, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 126:
#line 372 "parser.y"
{ yyval.n=new node(NODE_NEQ, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 127:
#line 374 "parser.y"
{ yyval.n=yyvsp[-1].n; ;
    break;}
case 128:
#line 376 "parser.y"
{ yyval.n=new node(NODE_AND,yyvsp[-2].n,yyvsp[0].n); ;
    break;}
case 129:
#line 378 "parser.y"
{ yyval.n=new node(NODE_OR,yyvsp[-2].n,yyvsp[0].n); ;
    break;}
case 130:
#line 380 "parser.y"
{ yyval.n=new node(NODE_NOT, yyvsp[0].n); ;
    break;}
case 131:
#line 382 "parser.y"
{ yyval.n=new node(NODE_NULL, yyvsp[-2].n); ;
    break;}
case 132:
#line 384 "parser.y"
{ yyval.n=new node(NODE_NOT_NULL, yyvsp[-3].n); ;
    break;}
case 133:
#line 386 "parser.y"
{ yyval.n=new node(NODE_LIKE, yyvsp[0].s_value, NULL, yyvsp[-2].n); ;
    break;}
case 134:
#line 389 "parser.y"
{ yyval.n=new node(NODE_INLIST, yyvsp[-4].n, yyvsp[-1].n); ;
    break;}
case 135:
#line 392 "parser.y"
{
			      node *bet=new node(NODE_BETWEEN, yyvsp[-2].n, yyvsp[0].n);
			      yyval.n=new node(NODE_BETWEEN, yyvsp[-4].n, bet);
			   ;
    break;}
case 136:
#line 397 "parser.y"
{ yyval.n=new node(NODE_EXISTS, yyvsp[-1].n); ;
    break;}
case 137:
#line 400 "parser.y"
{ yyval.n=new node(NODE_IN, yyvsp[-4].n, yyvsp[-1].n); ;
    break;}
case 138:
#line 404 "parser.y"
{ yyval.n=yyvsp[0].n; ;
    break;}
case 139:
#line 407 "parser.y"
{ yyval.n=yyvsp[0].n; ;
    break;}
case 140:
#line 410 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n, NULL); ;
    break;}
case 141:
#line 412 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 142:
#line 415 "parser.y"
{ yyval.n=new node(NODE_ORDER, yyvsp[0].n); yyval.n->num1=1; ;
    break;}
case 143:
#line 417 "parser.y"
{ yyval.n=new node(NODE_ORDER, yyvsp[0].n); yyval.n->num1=1; ;
    break;}
case 144:
#line 419 "parser.y"
{ yyval.n=new node(NODE_ORDER, yyvsp[0].n); yyval.n->num1=-1; ;
    break;}
case 145:
#line 422 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[0].n, NULL); ;
    break;}
case 146:
#line 424 "parser.y"
{ yyval.n=new node(NODE_MORE, yyvsp[-2].n, yyvsp[0].n); ;
    break;}
case 147:
#line 427 "parser.y"
{ 
			      node *group=new node(NODE_GROUP, (node*)NULL);
			      node *where=new node(NODE_WHERE, yyvsp[0].n, group);
                              node *from=new node(NODE_MORE, yyvsp[-1].n);
                              from=new node(NODE_FROM, from, where);
                              yyval.n=new node(NODE_DELETE, NULL, from);
                           ;
    break;}
case 148:
#line 438 "parser.y"
{
			      node *group=new node(NODE_GROUP, (node*)NULL);
			      node *where=new node(NODE_WHERE, yyvsp[0].n, group);
                              node *from=new node(NODE_MORE, yyvsp[-5].n);
			      node *att=new node(NODE_MORE, new
			         node(NODE_COLEXP, yyvsp[-1].n), new node(NODE_MORE,
                                 new node(NODE_COLEXP,new node(NODE_IDENT,
                                 yyvsp[-3].s_value))));
                              from=new node(NODE_FROM, from, where);
                              yyval.n=new node(NODE_UPDATE, att, from);
			   ;
    break;}
case 149:
#line 451 "parser.y"
{ yyval.n=new node(NODE_GRANT, yyvsp[0].n); ;
    break;}
case 150:
#line 454 "parser.y"
{ yyval.n=new node(NODE_REVOKE, yyvsp[0].n); ;
    break;}
case 151:
#line 457 "parser.y"
{ yyval.n=new node(NODE_GRANTREVOKE, yyvsp[-3].n, yyvsp[-1].n);
                             yyval.n->num1=yyvsp[-5].i_value, yyval.n->num2=yyvsp[0].i_value; ;
    break;}
case 152:
#line 461 "parser.y"
{ yyval.n=new node(NODE_MORE,strdup("all")); ;
    break;}
case 157:
#line 469 "parser.y"
{ yyval.i_value|=yyvsp[0].i_value; ;
    break;}
case 158:
#line 472 "parser.y"
{ yyval.i_value=GRANTSELECT; ;
    break;}
case 159:
#line 474 "parser.y"
{ yyval.i_value=GRANTINSERT; ;
    break;}
case 160:
#line 476 "parser.y"
{ yyval.i_value=GRANTDELETE; ;
    break;}
case 161:
#line 478 "parser.y"
{ yyval.i_value=GRANTUPDATE; ;
    break;}
case 162:
#line 480 "parser.y"
{ yyval.i_value=GRANTDROP; ;
    break;}
case 163:
#line 482 "parser.y"
{ yyval.i_value=GRANTSELECT|GRANTINSERT|GRANTDELETE|GRANTUPDATE|
                              GRANTDROP; ;
    break;}
case 164:
#line 485 "parser.y"
{ yyval.i_value=GRANTSELECT|GRANTINSERT|GRANTDELETE|GRANTUPDATE|
                              GRANTDROP; ;
    break;}
case 165:
#line 489 "parser.y"
{ yyval.i_value=0; ;
    break;}
case 166:
#line 491 "parser.y"
{ yyval.i_value=1; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 493 "parser.y"

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
