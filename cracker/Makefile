
cracker: cracker
	gcc -g src/cracker.c -o cracker -lpthread -Wall -Werror

clean:
	rm -f cracker

tests: tests/test.c
	gcc -g tests/test.c -o test -Wall -Werror -lcunit
	./a.out

all: clean cracker tests
