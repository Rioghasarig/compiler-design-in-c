/*@A (C) 1992 Allen I. Holub                                                */

#include <stdio.h>
#include <stdarg.h>
#include <tools/debug.h>
#include <tools/compiler.h>
#include <tools/l.h>

#include "CFGSymbolTable.h"
#include "llout.h"
#include "parser.h"

/* LLPAR.C:	A recursive-descent parser for a very stripped down llama.
 *		There's a llama input specification for a table-driven parser
 *		in llama.lma.
 */

extern CFGSymbolTable cfg_symbol_table; 

void advance	 P(( void 		));	/* local */
void lookfor	 P(( int first,...	));
void definitions P(( void 		));
void body	 P(( void 		));
void right_sides P(( void 		));
void rhs	 P(( void 		));
int  yyparse	 P(( void 		));	/* public */
extern int	yylineno;			/* Created by lex.*/
extern char	*yytext;
extern "C" int	yylex P((void));
extern "C" void ws P(( void ));		/* from lexical analyser */
//extern "C" void lerror ( int fatal, char *fmt, ... );
/*----------------------------------------------------------------------*/

  int yynerrs;		/* Total error count		*/
 int Lookahead;		/* Lookahead token	    	*/

/*======================================================================
 * Low-level support routines for parser:
 */

#define match(x) ((x) == Lookahead)

	void advance()
{
    if( Lookahead != _EOI_ )
	while( (Lookahead = yylex()) == WHITESPACE )
		;
}
/*----------------------------------------------------------------------*/
ANSI( 	void	lookfor( int first, ... )	)
KnR ( 	void	lookfor(     first      )	)
KnR ( int	first;					)
{
    /* Read input until the current symbol is in the argument list. For example,
     * lookfor(OR, SEMI, 0) terminates when the current Lookahead symbol is an
     * OR or SEMI. Searching starts with the next (not the current) symbol.
     */

    int	*obj;

    for( advance() ;; advance() )
    {
	for( obj = &first;  *obj  &&  !match(*obj) ; obj++ )
	    ;

	if( *obj )
	    break;		/* Found a match */

	/*else if( match(_EOI_) )
	    lerror(FATAL, "Unexpected end of file\n");*/
    }
}
/*======================================================================
 * The Parser itself
 */

	int yyparse P(( void ))   /* spec : definitions body stuff */
{
	extern int yylineno;
	Lookahead = yylex();	   /* Get first input symbol */
	definitions();
	//first_sym();
	body();
	return 0;
}
/*----------------------------------------------------------------------*/
	void	definitions()
{
    /*								implemented at:
     * definitions  : TERM_SPEC tnames  definitions			1
     *		    | CODE_BLOCK 	definitions			2
     *		    | SYNCH snames      definitions			3
     *		    | SEPARATOR						4
     *		    | _EOI_						4
     * snames	    : NAME {add_synch} snames				5
     * tnames	    : NAME {make_term} tnames				6
     *
     * Note that LeX copies the CODE_BLOCK contents to the output file
     * automatically on reading it.
     */

    while( !match(SEPARATOR) && !match(_EOI_) )				/* 4 */
    {
	//if( Lookahead == SYNCH )					/* 3 */
	//{
	    //for( advance(); match(NAME); advance() )			/* 5 */
		//add_synch( yytext );
	//}
	/*else*/ if( Lookahead == TERM_SPEC )				/* 1 */
	{
	    for( advance(); match(NAME); advance() ) {			/* 6 */
			//make_term( yytext );
			cfg_symbol_table.make_terminal(std::string(yytext));
		}	
	}
	else if( Lookahead == CODE_BLOCK )				/* 2 */
	{
	    advance(); 	/* the block is copied out from yylex() */
	}
	else
	{
	    /*lerror(NONFATAL,"Ignoring illegal <%s> in definitions\n", yytext );*/
	    advance();
	}
    }

    advance();					/* advance past the %%	*/
}
/*----------------------------------------------------------------------*/
	void	body()
{
    /*								implemented at:
     * body	: rule body						1
     *		| rule SEPARATOR					1
     *		| rule _EOI_						1
     * rule	: NAME {new_nonterm} COLON right_sides			2
     *		: <epsilon>						3
     */

 

    while( !match(SEPARATOR)  &&  !match(_EOI_) )			/* 1 */
    {
	if( match(NAME) )						/* 2 */
	{
	   // new_nonterm( yytext, 1 );
	   cfg_symbol_table.new_lhs(yytext);
	   printf("Creating term: %s\n", yytext);
	    advance();
	}
	else								/* 3 */
	{
	   /*lerror(NONFATAL, "Illegal <%s>, nonterminal expected.\n", yytext)*/;
	    lookfor( SEMI, SEPARATOR, 0 );
	    if( match(SEMI) )
		advance();
	    continue;
	}

	if( match( COLON ) )
	    advance();
	/*else
	    lerror(NONFATAL, "Inserted missing ':'\n");*/

	right_sides();
    }
    ws();			       /* Enable white space (see parser.lex) */
    if( match(SEPARATOR)  )
	yylex();		       /* Advance past %% 		      */
}
/*----------------------------------------------------------------------*/
	void	right_sides()
{
    /* right_sides	: {new_rhs} rhs OR right_sides			1
     *			| {new_rhs} rhs SEMI				2
     */

    //new_rhs();
	printf("--- Starting New LHS ---\n");
	cfg_symbol_table.new_production(); 
    rhs();
    while( match(OR) )							/* 1 */
    {
	advance();
	//new_rhs();
	rhs();
    }
    if( match(SEMI) )							/* 2 */
		advance();
    /*else
		lerror(NONFATAL, "Inserted missing semicolon\n");*/
}
/*----------------------------------------------------------------------*/
	void	rhs()
{
    /* rhs	: NAME   {add_to_rhs} rhs  */
    /*		| ACTION {add_to_rhs} rhs  */
	printf("New Produciton\n");
    while( match(NAME) || match(ACTION) )
    {
	//add_to_rhs( yytext, match(ACTION) ? start_action() : 0 );
	if(match(NAME)) {
		printf("\tAdding non-action term: %s\n", yytext);
		cfg_symbol_table.add_nonaction(yytext);
	}
	if (match(ACTION))
	{
		printf("\tAdding action term: %s\n", yytext);
		cfg_symbol_table.add_action(yytext);
	}
	advance();
    }
    if( !match(OR) && !match(SEMI) )
    {
		/*lerror(NONFATAL, "illegal <%s>, ignoring rest of production\n", yytext);*/
		lookfor( SEMI, SEPARATOR, OR, 0 );
    }
}
