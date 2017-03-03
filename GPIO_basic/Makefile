all:

gpio:
	gcc gpio_access.c -o test

build_test: gpio
	rm gpio_access

test: gpio
	./test
	rm test
