all: main

HEADERS = globals.h cgen.h analyse.h hashtable.h parse.h symbol.h
SOURCES =   analyse.c cgen.c globals.c lex.yy.c main.c parse.c symbol.c
OUTPUTS = main lex.yy.c

main: lex.yy.c  $(SOURCES) $(HEADERS)
	gcc $(SOURCES) -o main

lex.yy.c: scanner.l
	flex $<

clean:
	rm $(OUTPUTS)
