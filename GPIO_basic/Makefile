all:

gpio:
	gcc gpio_access.c -o pin_access

build_test: gpio
	rm pin_access

test:
	gcc gpio_access.c -o test
	./test 1 5,WRITE=HIGH
# correct input for a write request

	./test 1 5,READ=
# correct input for a read request

	./test 2 5,WRITE=HIGH 2,READ=
# correct input for a read and write request

	./test 3 5,WRITE= 2,READ= 3,WRITE=HIGH
# missing write value for pin 1 request

	./test 3 5,WRITE=HIGH 2,READ=LOW 3,WRITE=HIGH
# write value specified for pin 2 request -- don't know if intent was read or write!

	./test 4 5,WRITE=HIGH 2,READ= 3,write=HIGH 4,WRITE=HIGH
# misspelled write for pin 3 request

	./test 4 5,WRITE=HIGH 2,READ= 3,WRITE=high 60,WRITE=HIGH
# misspelled value for pin 3 request

	./test 4 5,WRITE=HIGH 2,READ= 3,write=HIGH taxation,is=teft
# bad form for pin 4 request

	./test 4 5,WRITE=HIGH 2,READ= ,= 4,WRITE=HIGH
# nothing given for the third request except demarkers

	./test 4 5,WRITE=HIGH 2,READ= , 4,WRITE=HIGH
# missing data and demarker for third request

	./test 4 5,WRITE=HIGH 2,READ= = 4,WRITE=HIGH
# missing data and demarker for third request

	./test 4 5WRITEHIGH 2,READ= 3,WRITE=HIGH 4,WRITE=HIGH
# missing demarkers in first request

	./test 4 WRITE,HIGH=5 2,READ= 3,WRITE=HIGH 4,WRITE=HIGH
# wrong order for args in first request

	./test 4 HIGH,5=WRITE 2,READ= 3,WRITE=HIGH 4,WRITE=HIGH
# wrong order for args in first request

	./test 4 5,WRITE=HIGH 2,READ= 3,WRITE=HIGH 4,WRITE=HIGH
# correct input for multiple read and write requests

	./test 4 28,WRITE=HIGH 2,READ= 3,WRITE=HIGH 4,WRITE=HIGH
# pin number on first pin request is too high

	./test 4 1,WRITE=HIGH 2,READ= 3,WRITE=HIGH 4,WRITE=HIGH
# pin number on first request is too low	
