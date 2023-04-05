#define CONNECT 257
#define SET 258
#define EQUALS 259
#define PRINT 260
#define OPENPAREN 261
#define CLOSEPAREN 262
#define QUERY 263
#define DATABASE 264
#define FREE 265
#define THEN 266
#define PRINTROWS 267
#define NOT 268
#define AND 269
#define OR 270
#define EQ 271
#define NOTEQ 272
#define LESS 273
#define LESSEQ 274
#define MORE 275
#define MOREEQ 276
#define IF 277
#define ELSE 278
#define FI 279
#define EOL 280
#define GETROW 281
#define EXEC 282
#define CRYPT 283
#define USER 284
#define PRINTATTR 285
#define PRINTHEADER 286
#define DEFAULT 287
#define PIPE 288
#define OPENTCP 289
#define READTCP 290
#define WRITETCP 291
#define PLUS 292
#define MINUS 293
#define STAR 294
#define SLASH 295
#define VARIABLE 296
#define STRING 297
#define IDENTIFIER 298
#define NUMBER 299
typedef union
{
   char *s_value;
   double f_value;
   int i_value;
} YYSTYPE;
extern YYSTYPE yylval;
