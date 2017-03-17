all:
	gcc Server.c Server_Log.c Timestamp.c -o server

clean:
	rm server

run:
	./server

live: all run

test: all run clean

debug:
	gcc -g Server.c -o server_debug
	gdb server_debug
