typedef union
{
   char *s_value;
   int i_value;
   double f_value;
   time_t d_value;
   node *n;
} YYSTYPE;
#define	IDENTIFIER	258
#define	STRING	259
#define	STRING1	260
#define	NUMBER	261
#define	FUNCTION	262
#define	FLOATVAL	263
#define	MONEYVAL	264
#define	DATEVAL	265
#define	CREATE	266
#define	AUTHORIZATION	267
#define	DATABASE	268
#define	DROP	269
#define	USER	270
#define	PASSWORD	271
#define	SET	272
#define	TABLE	273
#define	TNULL	274
#define	INTEGER	275
#define	OPEN	276
#define	VARCHAR	277
#define	DATE	278
#define	MONEY	279
#define	FLOAT	280
#define	SYNC	281
#define	INSERT	282
#define	INTO	283
#define	VALUES	284
#define	CHAR	285
#define	HELP	286
#define	SELECT	287
#define	FROM	288
#define	WHERE	289
#define	ORDER	290
#define	GROUP	291
#define	BY	292
#define	HAVING	293
#define	AS	294
#define	IS	295
#define	IN	296
#define	PRIVILEGES	297
#define	SUM	298
#define	COUNT	299
#define	AVG	300
#define	MIN	301
#define	MAX	302
#define	LESS	303
#define	MORE	304
#define	LEQ	305
#define	MOQ	306
#define	NEQ	307
#define	VIEW	308
#define	UPDATE	309
#define	PRIMARY	310
#define	KEY	311
#define	OPENPAREN	312
#define	CLOSEPAREN	313
#define	SEMICOLON	314
#define	EQUALS	315
#define	COMMA	316
#define	DOT	317
#define	CASCADE	318
#define	RESTRICT	319
#define	DELETE	320
#define	BETWEEN	321
#define	EXISTS	322
#define	FOREIGN	323
#define	REFERENCES	324
#define	ON	325
#define	GRANT	326
#define	WITH	327
#define	OPTION	328
#define	REVOKE	329
#define	TO	330
#define	ALL	331
#define	ASCENDING	332
#define	DESCENDING	333
#define	ROWNUM	334
#define	LIKE	335
#define	PLUS	336
#define	MINUS	337
#define	STAR	338
#define	SLASH	339
#define	OR	340
#define	AND	341
#define	NOT	342


extern YYSTYPE yylval;
