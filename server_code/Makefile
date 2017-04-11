all:
	gcc Server.c Arg_Builder.c Server_Log.c Timestamp.c -o server

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

log_test:
	gcc test_Server_Log.c Server_Log.c Timestamp.c -o logTest
