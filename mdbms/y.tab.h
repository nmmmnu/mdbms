typedef union
{
   char *s_value;
   int i_value;
   double f_value;
   time_t d_value;
   node *n;
} YYSTYPE;
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


extern YYSTYPE yylval;
