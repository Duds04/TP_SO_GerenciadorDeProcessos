CFLAGS=-Wall -Wextra -ggdb -std=c11
INCDIR=include

controle: main.o gerencia.o instrucao.o processo.o tabela.o bloqueados.o robin.o
	$(CC) -o $@ $^

main.o: src/main.c
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

gerencia.o: src/gerencia.c include/gerencia.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

instrucao.o: src/instrucao.c include/instrucao.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

processo.o: src/processo.c include/processo.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

tabela.o: src/tabela.c include/tabela.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

bloqueados.o: src/bloqueados.c include/bloqueados.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

robin.o: src/robin.c include/robin.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

.PHONY: clean
clean:
	rm *.o controle
