all:
	gcc Server.c -o server

clean:
	rm server

run:
	./server

live: all run

test: all run clean

debug:
	gcc -g Server.c -o server_debug
	gdb server_debug

tm_test:
	gcc testTimestamp.c Timestamp.c -o tmTest
