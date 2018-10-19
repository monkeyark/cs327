main: main.o dungeon.o queue.o npcmove.o
	g++ dungeon.o queue.o npcmove.o main.o -o main -lncurses 

dungeon.o: dungeon.c dungeon.h queue.h npcmove.h
	g++ -c dungeon.c -Wall -Werror -ggdb -o dungeon.o -lncurses 

queue.o: queue.c queue.h
	g++ -c queue.c -Wall -Werror -ggdb -o queue.o

npcmove.o: npcmove.c npcmove.h
	g++ -c npcmove.c -Wall -Werror -ggdb -o npcmove.o

main.o: main.c dungeon.h queue.h
	g++ -c main.c -Wall -Werror -ggdb -o main.o

clean:
	rm -f dungeon.o queue.o npcmove.o main.o main
