all: wrapper_only server_only

wrapper_only:
	gcc Server_Wrapper.c -o wrapper

server_only:
	gcc Server.c Arg_Builder.c Server_Log.c Timestamp.c -o server

clean:
	rm server
	rm wrapper

run:
	./wrapper

live: all run

test: all run clean

build_test: all clean

debug:
	gcc -g Server.c -o server_debug
	gdb server_debug

tm_test:
	gcc testTimestamp.c Timestamp.c -o tmTest

log_test:
	gcc test_Server_Log.c Server_Log.c Timestamp.c -o logTest

temp:
	gcc temp.c -o temp
