CFLAGS = -g -O0 -std=c89 -DLLAMA -I../../../include/ -Wfatal-errors -Wno-incompatible-pointer-types \
		-Wno-format-security -Wno-format
PP = ../../../bin/pp
CC = gcc

COMOBJ = main.o acts.o lexyy.o first.o stok.o
LLOBJ  = llselect.o llcode.o lldriver.o follow.o lldollar.o
YYOBJ  = yypatch.o yycode.o yydriver.o yystate.o yydollar.o

LEX    = ../lex/LeX 
CLIB = ../../../lib/l.lib ../../../lib/comp.lib

# Common Objects
main.o:		main.c	   parser.h
acts.o:		acts.pp.c   parser.h
		$(CC) $(CFLAGS) -c -w acts.pp.c
		mv acts.pp.o acts.o
lexyy.o:	lexyy.c parser.h
first.o:	first.c	   parser.h
stok.o:		stok.c 	   parser.h

# Llama Objects
llselect.o:	llselect.c parser.h
llcode.o:	llcode.c   parser.h
lldriver.o:	lldriver.c parser.h
follow.o:	follow.c   parser.h
lldollar.o:	lldollar.c parser.h

# Occs Object
yypatch.o: yypatch.c parser.h
yycode.o:	yycode.c   parser.h
yydriver.o:	yydriver.c parser.h
yystate.o: yystate.c parser.h
yydollar.o:	yydollar.c parser.h

llout.o:	llout.c parser.h
		$(CC) $(CFLAGS) -Wno-format -c llout.c -o llout.o
llout.c:	parser.lma
		./llama -vl parser.lma
llpar.o:	llpar.c	   parser.h


acts.pp.c:	acts.c
		$(PP) acts.c "$(CFLAGS)"

lexyy.c:	parser.lex
		$(LEX)  -m../../../lib/lex.par parser.lex
		sed 's/^[ 	]*#[ 	]*undef/#undef/' lexyy.c >.mk.tmp.
		mv .mk.tmp. lexyy.c

occs:   llout.o $(YYOBJ) $(COMOBJ) signon.c
	echo \#define __DATE__ \"`date`\" >> date.h
	$(CC) -c $(CFLAGS) signon.c
	rm date.h
	$(CC) -g -O0 -o  occs  llout.o $(COMOBJ) $(YYOBJ) signon.o \
						$(CLIB) 

llama:  llpar.o $(LLOBJ) $(COMOBJ) signon.c
	echo \#define __DATE__ \"`date`\" > date.h
	$(CC) -c $(CFLAGS) signon.c
	rm date.h
	$(CC) -o llama llpar.o $(CFLAGS) $(COMOBJ) $(LLOBJ) signon.o \
						$(CLIB)

llama2: llout.o $(LLOBJ) $(COMOBJ) signon.c
	echo \#define __DATE__ \"`date`\" > date.h
	$(CC) -c $(CFLAGS) signon.c
	rm date.h
	$(CC) -o llama llout.o $(CFLAGS) $(COMOBJ) $(LLOBJ) signon.o \
						$(CLIB)

clean:
	rm -f $(YYOBJ) $(COMOBJ) $(LLOBJ) lexyy.c acts.pp.c llout.o llpar.o