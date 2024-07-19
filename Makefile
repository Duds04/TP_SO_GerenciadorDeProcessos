CFLAGS=-Wall -Wextra -ggdb -std=c11
INCDIR=include

controle: main.o gerencia.o
	$(CC) -o $@ $^

main.o: src/main.c
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

gerencia.o: src/gerencia.c
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

.PHONY: clean
clean:
	rm *.o controle
