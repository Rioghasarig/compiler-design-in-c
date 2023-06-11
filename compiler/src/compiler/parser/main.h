/*compiler.h*/
extern  "C" void printv (FILE *fp,char **argv);

/*l.h*/
extern  "C" int ii_newfile (char *name);
extern  "C" void ii_unterm (void );
extern  "C" int ii_advance (void );
extern  "C" int ii_flush (int force);
extern  "C" unsigned char ii_mark_start (void );
extern  "C" int ii_pushback (int n);
extern  "C" int ii_lineno (void );

/*parser.h*/
#define NONFATAL	0	/* Values to pass to error() and lerror(), */
#define FATAL		1	/* defined in main.c.			   */
#define WARNING		2
#define NOHDR		3

#define NAME_MAX 32				/* Max name length + 1 */
#define MAXRHS	 31	

#define EXIT_ILLEGAL_ARG	255	/* Illegal command-line switch	    */
#define EXIT_TOO_MANY		254	/* Too many command-line args	    */
#define EXIT_NO_DRIVER		253	/* Can't find llama.par		    */
#define EXIT_OTHER		252	/* Other error (syntax error, etc.) */
#define EXIT_USR_ABRT		251	/* Ctrl-Break			    */



#define TOKEN_FILE	"llout.h"	/* output file for token #defines     */
#define PARSE_FILE	"llout.c"	/* output file for parser	      */
#define SYM_FILE	"llout.sym"	/* output file for symbol table	      */
#define DOC_FILE	"llout.doc"	/* LALR(1) State machine description  */
#define DEF_EXT	"lma"		/* foo.lma is default input extension */
#define PAR_TEMPL	"llama.par" 	/* template file for parser	      */
#define PROG_NAME   "llama"

#define MAXNAME     32	/* Maximum length of a terminal or nonterminal name   */
#define MAXPROD	   512	/* Maximum number of productions in the input grammar */

#define MINTERM	     1	/* Token values assigned to terminals start here    */
#define MINNONTERM 256	/* nonterminals start here			    */
#define MINACT	   512	/* acts start here				    */

				/* Maximum numeric values used for terminals
				 * and nonterminals (MAXTERM and MINTERM), as
				 * well as the maximum number of terminals and
				 * nonterminals (NUMTERMS and NUMNONTERMS).
				 * Finally, USED_TERMS and USED_NONTERMS are
				 * the number of these actually in use (i.e.
				 * were declared in the input file).
				 */

#define MAXTERM     (MINNONTERM -2)
#define MAXNONTERM  (MINACT     -1)

#define NUMTERMS    ((MAXTERM-MINTERM) +1)
#define NUMNONTERMS ((MAXNONTERM-MINNONTERM)+1)

#define USED_TERMS	((Cur_term    - MINTERM)    +1)
#define USED_NONTERMS	((Cur_nonterm - MINNONTERM) +1)

				/* These macros evaluate to true if x represents
				 * a terminal (ISTERM), nonterminal (ISNONTERM)
				 * or action (ISACT)
				 */

#define ISTERM(x)    ((x) && (MINTERM    <= (x)->val && (x)->val <= MAXTERM   ))
#define ISNONTERM(x) ((x) && (MINNONTERM <= (x)->val && (x)->val <= MAXNONTERM))
#define ISACT(x)     ((x) && (MINACT     <= (x)->val 		  	      ))

			         /* Epsilon's value is one more than the largest
			 	  * terminal actually used. We can get away with
				  * this only because EPSILON is not used until
				  * after all the terminals have been entered
				  * into the symbol table.
			 	  */
#define EPSILON  (Cur_term+1)
				  /* The following macros are used to adjust the
				   * nonterminal values so that the smallest
				   * nonterminal is zero. (You need to do this
				   * when you output the tables). ADJ_VAL does
				   * the adjustment, UNADJ_VAL translates the
				   * adjust value back to the original value.
				   */

#define ADJ_VAL(x)   ((x)-MINNONTERM )
#define UNADJ_VAL(x) ((x)+MINNONTERM )

#define  outc(c)  putc(c,Output);

int Symbols = 0;
int Debug = 0; 
int Uncompressed = 0; 
int No_lines = 0; 
int Make_yyoutab = 0; 
int No_warnings = 0;
int Threshold = 0; 

int Cur_term  = MINTERM-1;
int Cur_nonterm  = MINNONTERM-1; 
int Cur_act = MINACT-1; /*  "      action	      */
int Num_productions = 0; /* Number of productions    */

typedef struct _symbol_
{
    char	   name  [ NAME_MAX ];		/* symbol name. Must be first */
    char	   field [ NAME_MAX ];		/* %type <field>	      */
    unsigned	   val;				/* numeric value of symbol    */
    unsigned	   used;			/* symbol used on an rhs      */
    unsigned	   set;				/* symbol defined             */
    unsigned	   lineno;			/* input line num. of string  */
    char  	   *string;			/* code for actions.          */
    struct  _prod_ *productions;		/* right-hand sides if nonterm*/
 //   SET		   *first;			/* FIRST set		      */
 //   SET		   *follow; 			/* FOLLOW set		      */
} SYMBOL;

typedef struct _prod_
{
    unsigned  	  num;			    /* production number	      */
    SYMBOL	  *rhs[ MAXRHS + 1];	    /* Tokenized right-hand side      */
    SYMBOL	  *lhs;			    /* Left-hand side		      */
    unsigned char rhs_len;		    /* # of elements in rhs[] array   */
    unsigned char non_acts;		    /*        "  that are not actions */
 //   SET		  *select;		    /* LL(1) select set	              */
    struct _prod_ *next;		    /* pointer to next production     */
					    /*       for this left-hand side. */

} PRODUCTION;

#include <tools/set.h>
#include <tools/hash.h>
extern "C" void	init_acts	(void );
extern "C" void	file_header	( void );	
//extern "C" int	problems	(void );
//extern "C" void	first		( void );
//extern "C" void	follow		( void );
//extern "C" void	select		( void );	
extern "C" void	signon		( void );
extern "C" void	code_header	( void );
extern "C" void	tables		( void );
extern "C" void	driver		( void );
extern "C" void	print_symbols	(FILE *stream);
extern "C"   char  *do_dollar  	( int num, int rhs_size, int lineno,\
  			       PRODUCTION *prod, char *fname);
extern "C" void	lerror	  	( int fatal, char *fmt, ... );
extern "C" void	output		( char *fmt, ... );	
int yyparse();

int Use_stdout = 1;
FILE *Output;
int  Make_parser;
char* Input_file_name = "console"; 

#define DOLLAR_DOLLAR	((unsigned)~0 >>1)
/*globals.h*/
int Verbose = 0;
int Public = 0;
char* Template = "lex.par"; 

/*misc*/
extern "C" int nows();