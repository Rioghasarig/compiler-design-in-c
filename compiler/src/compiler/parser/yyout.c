#include "yyout.h"

#define YYACTION
#define YYPARSER
/*@A (C) 1992 Allen I. Holub                                                */


#include <stdio.h>
#include <stdarg.h>
#include <tools/yystack.h>					/* #pp */


#include <stdlib.h>	  /* Needed only for prototype for exit(). Can be  */
			  /* discarded in most non-ANSI systems.	   */

#include <tools/debug.h>  /* These includes are needed to get prototypes   */
#include <tools/l.h>	  /* for the ii_ and yy_ functions in l.lib        */
			  /* You can remove them if your compiler doesn't  */
			  /* require the prototypes. The UNIX and ANSI     */
			  /* macros in debug.h are also used here.	   */
			  /* The UNIX, ANSI, BCC, and P, macros in debug.h */
			  /* are also used here.			   */
			  /* Note that there's a nested include for 	   */
			  /* stdarg.h in  <tools/l.h>.			   */

FILE  *yycodeout = stdout ;		/* Output stream (code). */
FILE  *yybssout  = stdout ;		/* Output stream (bss ). */
FILE  *yydataout = stdout ;		/* Output stream (data). */
int   yylookahead ;			/* Lookahead token.      */

extern char *yytext;			/* Declared by lex in lexyy.c */
extern int  yylineno;
extern int  yyleng;

void	yycode	    P(( char *fmt, ...	));	/* Supplied below and */
void	yydata	    P(( char *fmt, ...	));	/* in yydebug.c       */
void	yybss 	    P(( char *fmt, ...	));
void 	yyerror     P(( char *fmt, ...  ));
void 	yycomment   P(( char *fmt, ...  ));
int	yy_nextoken P(( void		));


extern unsigned char *ii_ptext();	/* Lookback function used by lex  */
extern int  ii_plength() ;		/* in /src/compiler/lib/input.c.  */
extern int  ii_plineno() ;

#ifdef YYDEBUG			/* Define YYD here so that it can be used */
#   define YYD(x) x		/* in the user-supplied header.		  */
#else
#   define YYD(x) /* empty */
#endif

/*----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <tools/debug.h>	/* Misc. macros. (see Appendix A)	 */
#include <tools/hash.h>		/* Hash-table support. (see Appendix A)	 */
#include <tools/compiler.h> 	/* Prototypes for comp.lib functions.	 */
#include <tools/l.h>	    	/* Prototypes for l.lib functions.	 */
#include <tools/occs.h>	    	/* Prototypes for LeX/occs-generated stuff   */
#include <tools/c-code.h>    	/* Virtual-machine definitions.		 */

#ifdef __TURBOC__		/* Borland			*/
#include <dir.h>		/* for mktemp() prototype	*/
#endif
#ifdef MSDOS			/* Microsoft.			*/
#include <io.h>			/* for mktemp() prototype	*/
#endif

#ifdef YYACTION
#define ALLOC		/* Define ALLOC to create symbol table in symtab.h.  */
#endif

#include "symtab.h"	/* Definitions for the symbol-table.	     	     */
#include "value.h"	/* Definitions used for expression processing.	     */
#include "label.h"	/* Prefixes used for compiler-generated labels.	     */
#include "switch.h"	/* Definitions used for switch processing.	     */
#include "proto.h"	/* Function prototypes for all .c files used by the  */
			/* parser. It is not printed 