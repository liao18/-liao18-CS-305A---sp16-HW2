solve: main.o maze.o
	gcc -o solve main.o maze.o

%.o: %.c %.h
	gcc -c $<

clean:
	/bin/rm -f *.o solve
