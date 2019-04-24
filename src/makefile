prefixe = compile
includes = ast.c liste.c semantic_check.c codegen.c pile.c
FLAGS = -Wall -Wextra -Werror 

all: y.tab.o lex.yy.o
	gcc y.tab.o lex.yy.o $(includes) -ly -ll -o $(prefixe) $(FLAGS)

y.tab.o: $(prefixe).y
	yacc -d $(prefixe).y
	gcc -c y.tab.c

lex.yy.o: $(prefixe).l y.tab.h
	lex $(prefixe).l
	gcc -c lex.yy.c

clean:
	rm -f *.o y.tab.c y.tab.h lex.yy.c a.out $(prefixe)

test1:
	./compile tests/test1 >> tests/test_results
	echo "end test1\n" >> tests/test_results

test2:
	./compile tests/test2 >> tests/test_results

test3:
	./compile tests/test3 >> tests/test_results

test : test1 test2 test 3
	