all:
	gcc Server.c -o server

clean:
	rm server

run:
	./server

test: all run clean

debug:
	gcc -g Server.c -o server_debug
	gdb server_debug
