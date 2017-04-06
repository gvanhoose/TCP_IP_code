all:

gpio:
	gcc gpio_access.c -o gpio_access

test: gpio
	./gpio_access
	rm gpio_access
