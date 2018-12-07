main: main.o f2c.o c2f.o 
	gcc c2f.o f2c.o main.o -o main

f2c.o: f2c.c cf.h
	gcc -c f2c.c -Wall -Werror -ggdb -o f2c.o

c2f.o: c2f.c cf.h
	gcc -c c2f.c -Wall -Werror -ggdb -o c2f.o

main.o: main.c cf.h
	gcc -c main.c -Wall -Werror -ggdb -o main.o

clean:
	rm -f f2c.o c2f.o main.o main *~
