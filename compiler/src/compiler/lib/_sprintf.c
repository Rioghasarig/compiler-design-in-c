/*@A (C) 1992 Allen I. Holub                                                */
/* The ANSI sprintf() returns the size of the target string. Provide an
 * ANSI-compatible version for UNIX. (It's called _sprintf to avoid possible
 * redefinition conflicts with declarations in header files).
 */

#include <stdarg.h>

int 	_sprintf(char *buf, char *fmt, ... )
{
    va_list   args;
    va_start( args,   fmt );
    vsprintf( buf, fmt, args );     /* defined in /src/compiler/lib/prnt.c */
    return( strlen( buf ) );
}
