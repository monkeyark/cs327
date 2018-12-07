all: times_table hello hello.class

times_table: times_table.c
	gcc -Wall -Werror -g times_table.c -o times_table

hello: hello.c
	gcc -Wall -Werror -g hello.c -o hello

hello.class: hello.java
	javac hello.java

clean:
	rm -f times_table hello hello.class

