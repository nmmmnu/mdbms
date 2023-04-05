/*  Errors returned by execquery() and execstatement() */
extern char *errors[];

#define ERR_OK 0                      /* No error */
#define ERR_NOSUCHPAGE  1             /* Nu such page */
#define ERR_READ 2                    /* Read error */
#define ERR_WRITE 3                   /* Write error */
#define ERR_OPEN 4                    /* Open error */
#define ERR_NODBFILES 5               /* No physical files present */
#define ERR_BADVALUE 6                /* Bad argument */
#define ERR_DBEXISTS 7                /* Database exists */
#define ERR_TBLEXISTS 8               /* Table exists */
#define ERR_NOSUCHDB 9                /* No such database */
#define ERR_NOSUCHTBL 10              /* No such table */
#define ERR_TOOMANYVAL 11             /* Too many values on insert */
#define ERR_NODB 12                   /* No current database */
#define ERR_COMPATIBLEINSERT 13       /* Type incompatible on insert */
#define ERR_NULLNOTALLOWED 14         /* NULL value not allowed */
#define ERR_NOTSUPPORTED 15           /* Not supported */
#define ERR_NOSUCHATTR 16             /* No such attribute in table */
#define ERR_MATHERROR 17              /* Mathematic error */
#define ERR_AGGREGATEMIX 18           /* Can't mix aggregate and non aggregate functions */
#define ERR_STAR 19                   /* '*' only allowed in count() */
#define ERR_PARAMETER 20              /* Invalid parameter */
#define ERR_COMPARE 21                /* Compare error */
#define ERR_TBLNOTEMPTY 22            /* Table not empty on drop */
#define ERR_SEARCHPENDING 23          /* Search pending */
#define ERR_PARSEERROR 24             /* Generic parse error */
#define ERR_MODIFYPRIMARY 25          /* Can't modify primary key */
#define ERR_MOREVALUESNEEDED 26       /* More values needed on insert */
#define ERR_DUPPRIMARY 27             /* Duplicate primary key */
#define ERR_SUBMORECOLS 28            /* Subselect:When using expressions, one column should be selected */
#define ERR_ISPRIMARY 29              /* Already a primary key */
#define ERR_MOREATTNEEDED 30          /* More attributes needed */
#define ERR_TOOMANYATT 31             /* Too many attributes */
#define ERR_TYPES 32                  /* Types incompatible */
#define ERR_ISFOREIGN 33              /* Already a foreign key */
#define ERR_CANTSETNULL 34            /* Can't set attribute to NULL */
#define ERR_CANTREFERE 35             /* Can't refere to table, it has no primary key */
#define ERR_PERMISSION 36             /* Permission denied */
#define ERR_USEREXISTS 37             /* User exists */
#define ERR_INVALIDPWD 38             /* Invalid user/password combination */
#define ERR_NOUSER 39                 /* No user selected */
#define ERR_CONVERT 40                /* Convert error */
#define ERR_DBNOTEMPTY 41             /* Database not empty */
#define ERR_DBINUSE    42             /* Database in use */
