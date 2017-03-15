all:
	gcc Client.c -o client

run:
	./client	

clean:
	rm client

test: all run clean

debug:
	gcc -g Client.c -o client_debug
	gdb client_debug
