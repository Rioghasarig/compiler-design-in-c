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
			/* parser. It is not printed anywhere in the book,   */
			/* but is included on the distribution disk.	     */

 void clean_up P((void));
typedef union
{
    int   yy_def;  /* Default field, used when no %type found */
    char	*p_char;
    symbol	*p_sym;
    link	*p_link;
    structdef	*p_sdef;
    specifier	*p_spec;
    value	*p_val;
    int		num;		/* Make short if sizeof(int) > sizeof(int*) */
    int		ascii;
}
yystype;

#define YYSTYPE yystype

#ifdef YYACTION

int     Nest_lev;	 /* Current block-nesting level.		*/

int	Enum_val;	 /* Current enumeration constant value	*/

char	Vspace[16];
char	Tspace[16];	 /* The compiler doesn't know the stack-frame size
			  * when it creates a link() directive, so it outputs
			  * a link(VSPACE+TSPACE). Later on, it #defines VSPACE
			  * to the size of the local-variable space and TSPACE
			  * to the size of the temporary-variable space. Vspace
			  * holds the actual name of the VSPACE macro, and
			  * Tspace the TSPACE macro. (There's a different name
			  * for each subroutine.)
			  */

char	Funct_name[ NAME_MAX+1 ];	/* Name of the current function */

#define STR_MAX 512	  	/* Maximum size of a string constant.   */
char	Str_buf[STR_MAX]; 	/* Place to assemble string constants.  */

#include <tools/stack.h>    	/* Stack macros. (see Appendix A)	 */

int stk_err( o )	/* declared as int to keep the compiler happy */
int o;
{
    yyerror( o ? "Loop/switch nesting too deep or logical expr. too complex.\n"
	       : "INTERNAL, label stack underflow.\n"  );
    exit( 1 );
    BCC( return 0 );	/* keep the compiler happy */
}
#undef  stack_err
#define stack_err(o)	 stk_err(o)

stack_dcl (S_andor, int, 32);	/* This stack wouldn't be necessary if I were */
				/* willing to put a structure onto the value  */
				/* stack--or_list and and_list must both      */
				/* return 2 attributes; this stack will hold  */
				/* one of them.				      */

/* These stacks are necessary because there's no syntactic connection break,
 * continue, case, default and the affected loop-control statement.
 */

stack_dcl