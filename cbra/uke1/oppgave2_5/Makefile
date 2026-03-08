all: program

program: funksjon.o program.o
	 gcc -o program funksjon.o program.o

program.o: program.c
	 gcc -c program.c

funksjon.o: funksjon.c
	 gcc -c funksjon.c

clean:
	 rm -f *.o program
