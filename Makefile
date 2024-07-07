CFLAGS=-std=c99 -Wall -Wextra -Werror

.PHONY: clean run

build: main.c
	gcc -g main.c orase.h Cerinta1.c Cerinta2.c $(CFLAGS) -o tema3

run: tema3
	./tema3

clean:
	rm -f tema3