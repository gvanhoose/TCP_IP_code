all:

export:
	gcc export_pin.c -o exportPin

unexport:
	gcc unexport_pin.c -o unexportPin

gpio:
	gcc gpio_wrapper.c gpio_access.c -o pin_access

build_test: test
	rm test

live_test: test
	./test 1 4,out=LOW
#	./test 2 4,out=LOW 17,out=HIGH
	rm test

test_all: test test00 test01 test02 test03 test04 test05 test06 test07 test08 test09 test0A test0B test0C test0D test0E test0F test10
	rm test

test: 
	gcc gpio_wrapper.c gpio_access.c -o test

test00:
	./test 1 5,out=HIGH > test_results.txt
# correct input for a write request

test01:
	./test 1 5,in= >> test_results.txt
# correct input for a read request

test02:
	./test 2 5,out=HIGH 2,in= >> test_results.txt
# correct input for a read and write request

test03:
	./test 3 5,out= 2,in= 3,out=HIGH >> test_results.txt
# missing write value for pin 1 request

test04:
	./test 3 5,out=HIGH 2,in=LOW 3,out=HIGH >> test_results.txt
# write value specified for pin 2 request -- don't know if intent was read or write!

test05:
	./test 4 5,out=HIGH 2,in= 3,Out=HIGH 4,out=HIGH >> test_results.txt
# misspelled write for pin 3 request

test06:
	./test 4 5,out=HIGH 2,in= 3,out=HiGH 60,out=HIGH >> test_results.txt
# misspelled value for pin 3 request

test07:
	./test 4 5,out=HIGH 2,in= 3,out=HIGH taxation,is=theft >> test_results.txt
# bad form for pin 4 request

test08:
	./test 4 5,out=HIGH 2,in= ,= 4,out=HIGH >> test_results.txt
# nothing given for the third request except demarkers

test09:
	./test 4 5,out=HIGH 2,in= , 4,out=HIGH >> test_results.txt
# missing data and demarker for third request

test0A:
	./test 4 5,out=HIGH 2,in= = 4,out=HIGH >> test_results.txt
# missing data and demarker for third request

test0B:
	./test 4 5outHIGH 2,in= 3,out=HIGH 4,out=HIGH >> test_results.txt
# missing demarkers in first request

test0C:
	./test 4 out,HIGH=5 2,in= 3,out=HIGH 4,out=HIGH >> test_results.txt
# wrong order for args in first request

test0D:
	./test 4 HIGH,5=out 2,in= 3,out=HIGH 4,out=HIGH >> test_results.txt
# wrong order for args in first request

test0E:
	./test 4 5,out=HIGH 2,in= 3,out=HIGH 4,out=HIGH >> test_results.txt
# correct input for multiple read and write requests

test0F:
	./test 4 28,out=HIGH 2,in= 3,out=HIGH 4,out=HIGH >> test_results.txt
# pin number on first pin request is too high

test10:
	./test 4 1,out=HIGH 2,in= 3,out=HIGH 4,out=HIGH >> test_results.txt
# pin number on first request is too low	
