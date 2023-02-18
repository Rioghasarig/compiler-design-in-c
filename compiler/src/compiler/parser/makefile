CFLAGS = -std=c89 -DOCCS -I../../../include/ -Wfatal-errors -Wno-incompatible-pointer-types
CC = gcc
first.o : first.c parser.h
	$(CC) -c $(CFLAGS) $<

yystate.o : yystate.c parser.h
	$(CC) -c $(CFLAGS) $<