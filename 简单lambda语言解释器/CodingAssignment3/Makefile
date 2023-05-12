lexer.h: lang.l
	flex lang.l

lexer.c: lang.l
	flex lang.l

parser.c: lang.y
	bison -o parser.c -d -v lang.y

parser.h: lang.y
	bison -o parser.c -d -v lang.y

lib.o: lib.c lib.h
	gcc -c lib.c

lang.o: lang.c lang.h lib.h
	gcc -c lang.c

parser.o: parser.c parser.h lexer.h lang.h
	gcc -c parser.c

lexer.o: lexer.c lexer.h parser.h lang.h
	gcc -c lexer.c

closure.o: closure.c lang.h
	gcc -c closure.c

interpreter.o: interpreter.c closure.h lang.h lib.h
	gcc -c interpreter.c

main.o: main.c lexer.h parser.h lang.h
	gcc -c main.c

main: lang.o parser.o lexer.o lib.o interpreter.o closure.o main.o
	gcc lang.o parser.o lexer.o lib.o interpreter.o main.o closure.o -o main

all: main

clean:
	rm -f lexer.h lexer.c parser.h parser.c *.o main

%.c: %.y

%.c: %.l

.DEFAULT_GOAL := all

