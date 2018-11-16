a.out: test_encode.o encode.o decode.o
	gcc -o a.out test_encode.c encode.c decode.c
clean:
	rm *.o
