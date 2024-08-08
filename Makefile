CFLAGS=-Wall -Wextra -ggdb -std=c11
INCDIR=include

controle: main.o gerencia.o instrucao.o processo.o tabela.o bloqueados.o \
	robin.o cpu.o filasID.o filasMultiplas.o escalonamento.o
	$(CC) -o $@ $^

main.o: src/main.c include/gerencia.h include/config.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

# Sim, essa linha é desconfortavelmente longa
gerencia.o: src/gerencia.c include/gerencia.h include/bloqueados.h \
	include/instrucao.h include/tabela.h include/cpu.h include/filasMultiplas.h \
	include/escalonamento.h include/robin.h include/config.h
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

filasID.o: src/filasID.c include/filasID.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

cpu.o: src/cpu.c include/cpu.h include/tabela.h include/escalonamento.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

filasMultiplas.o: src/filasMultiplas.c include/filasMultiplas.h \
	include/filasID.h include/processo.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

escalonamento.o: src/escalonamento.c include/escalonamento.h
	$(CC) -c -o $@ -I $(INCDIR) $(CFLAGS) $<

.PHONY: clean
clean:
	rm *.o controle
