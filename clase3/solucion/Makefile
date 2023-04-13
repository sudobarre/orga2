CC=c99
CFLAGS= -Wall -Wextra -pedantic -O0 -g -lm -Wno-unused-variable -Wno-unused-parameter -no-pie
NASM=nasm
NASMFLAGS=-f elf64 -g -F DWARF

all: main tester

OBJS := checkpoint2_c.o checkpoint2_asm.o checkpoint3_c.o checkpoint3_asm.o checkpoint4_c.o checkpoint4_asm.o
tester: tester.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

main: main.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

checkpoint2_c.o: checkpoint2.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@

checkpoint2_asm.o: checkpoint2.asm checkpoints.h
	$(NASM) $(NASMFLAGS) $< -o $@

checkpoint3_c.o: checkpoint3.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@

checkpoint3_asm.o: checkpoint3.asm checkpoints.h
	$(NASM) $(NASMFLAGS) $< -o $@

checkpoint4_c.o: checkpoint4.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@

checkpoint4_asm.o: checkpoint4.asm checkpoints.h
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f main tester
	rm -f salida.propios.*

