all:
	gcc Server.c Server_Log.c Timestamp.c -o server

clean:
	rm server

run:
	./server

bgrnd:
	./server &

live: all run

live_background: all bgrnd

test: all run clean

debug:
	gcc -g Server.c -o server_debug
	gdb server_debug

tm_test:
	gcc testTimestamp.c Timestamp.c -o tmTest

log_test:
	gcc test_Server_Log.c Server_Log.c Timestamp.c -o logTest
