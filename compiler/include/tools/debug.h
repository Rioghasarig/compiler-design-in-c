/*@A (C) 1992 Allen I. Holub                                                */
#ifndef __DEBUG_H	/* Makes sure that debug.h isn't included more than */
#define __DEBUG_H	/* once. Matching endif is at end of file.	    */

#ifdef  DEBUG
#	define D(x) x   /* expand only when debugging     */
#	define ND(x)    /* expand only when not debugging */
#else
#	define D(x)
#	define ND(x) x
#endif

			/* Unix compiler */
#	define BCC(x)
#	define MSC(x)
#	define MSC5(x)
#	define MSC6(x)
#       define MSC7(x)
#	define UNIX(x) x
#	define ANSI(x) x
#	define FARPTR *
#	define VA_LIST ,...

#	define O_BINARY 0	/* No binary input mode in UNIX open().     */
#	define far		/* Microsoft/Turbo keyword for an 8086      */
				/* 32-bit, far pointer. Ignore in UNIX.     */

#	define memmove(d,s,n) bcopy(s,d,n)
#	define memcpy(d,s,n)  bcopy(s,d,n)
#	define raise(sig) kill( (int)getpid(), sig )


#if (0 ANSI(+1))
#	define KnR(x)
#	define P(x)  x	/* function prototypes supported */
#else
#	define KnR(x)  x
#	define P(x)  ()	  /* Otherwise, discard argument lists and */
#	define void  char /* translate void keyword to int.	   */
#endif

#if ( 0 MSC(+1) BCC(+1) ) 	/* Microsoft or Borland Compiler */
#	define MS(x) x
#else
#	define MS(x)
#endif

/* SEG(p)	Evaluates to the segment portion of an 8086 address.
 * OFF(p)	Evaluates to the offset portion of an 8086 address.
 * PHYS(p)	Evaluates to a long holding a physical address
 */

#ifdef _8086
#define SEG(p)  ( ((unsigned *)&(p))[1] )
#define OFF(p)  ( ((unsigned *)&(p))[0] )
#define PHYS(p) (((unsigned long)OFF(p)) + ((unsigned long)SEG(p) << 4))
#else
#define PHYS(p)  (p)
#endif

/* NUMELE(array)	Evaluates to the array size in elements
 * LASTELE(array)	Evaluates to a pointer to the last element
 * INBOUNDS(array,p)    Evaluates to true if p points into the array.
 * RANGE(a,b,c)		Evaluates to true if a <= b <= c
 * max(a,b)		Evaluates to a or b, whichever is larger
 * min(a,b)		Evaluates to a or b, whichever is smaller
 *			associated with a pointer
 * NBITS(type)		Returns number of bits in a variable of the indicated
 *			type;
 * MAXINT		Evaluates to the value of the largest signed integer
 */

#define NUMELE(a)	(sizeof(a)/sizeof(*(a)))
#define LASTELE(a)	((a) + (NUMELE(a)-1))
#define TOOHIGH(a,p)	((p) - (a) > (NUMELE(a) - 1))
#define TOOLOW(a,p)	((p) - (a) <  0 )
#define INBOUNDS(a,p)	( ! (TOOHIGH(a,p) || TOOLOW(a,p)) )

/* Portability note: Some systems won't allow UL for unsined long in the _IS
 * macro. You can use the following if so:
 *
 *	(unsigned long)1
 *
 * Bob Muller, who suggested the foregoing, also reports:
 * "There also seems to be an issue with the notion of shifting; the DEC Ultrix
 * compiler, for example, says that (unsigned long)((unsigned long)1 << 32)
 * == 1, while the Sun SunOS 4 compiler says that it is 0."
 */

#define _IS(t,x) ( ((t)(1UL << (x))) !=0)
				      /* Evaluate true if the width of a      */
				      /* variable of type of t is < x. The != */
				      /* 0 assures that the answer is 1 or 0  */

#define NBITS(t) (4 * (1  + _IS(t, 4) + _IS(t, 8) + _IS(t,12) + _IS(t,16) \
		          + _IS(t,20) + _IS(t,24) + _IS(t,28) + _IS(t,32) ) )

#define MAXINT (((unsigned)~0) >> 1)

#if ( 0 UNIX(+1) )
#    ifndef MAX
#        define MAX(a,b) ( ((a) > (b)) ? (a) : (b))
#    endif
#    ifndef MIN
#        define MIN(a,b) ( ((a) < (b)) ? (a) : (b))
#    endif
#endif

#define RANGE(a,b,c)	( (a) <= (b) && (b) <= (c) )

   /* The distribution disk doesn't include dmalloc.h, so don't define
    * MAP_MALLOC anywhere.
    */
#ifdef MAP_MALLOC
#    include <tools/dmalloc.h>
#endif

#endif	/* #ifdef __DEBUG_H */


