CFLAGS = -std=c89 -DOCCS -I../../../include/ -Wfatal-errors -Wno-incompatible-pointer-types
PP = ../../../bin/pp
CC = gcc

COMOBJ = main.o acts.o lexyy.o first.o stok.o
YYOBJ  = yypatch.o yycode.o yydriver.o yystate.o yydollar.o
LEX    = ../lex/LeX 

yystate.o: yystate.c parser.h
yypatch.o: yypatch.c parser.h
yycode.o:	yycode.c   parser.h
yydriver.o:	yydriver.c parser.h
yydollar.o:	yydollar.c parser.h

main.o:		main.c	   parser.h
acts.o:		acts.pp.c   parser.h
		$(CC) -c $(CFLAGS) acts.pp.c
		mv acts.pp.o acts.o
lexyy.o:	lexyy.c parser.h
first.o: first.c parser.h
stok.o:		stok.c 	   parser.h
llout.o:	llout.c parser.h


acts.pp.c:	acts.c
		$(PP) acts.c "$(CFLAGS)"

lexyy.c:	parser.lex
		$(LEX) -vl -m../../../lib/lex.par parser.lex
		sed 's/^[ 	]*#[ 	]*undef/#undef/' lexyy.c >.mk.tmp.
		mv .mk.tmp. lexyy.c

occs:   $(YYOBJ) $(COMOBJ) signon.c
	echo \#define __DATE__ \"`date`\" >> date.h
	$(CC) -c $(CFLAGS) signon.c
	rm date.h
	$(CC) -o occs  $(COMOBJ) $(YYOBJ) signon.o \
						$(CLIB) 

clean:
	rm -f $(YYOBJ) $(COMOBJ) lexyy.c acts.pp.c