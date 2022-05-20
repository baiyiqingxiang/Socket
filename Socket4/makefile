CC=gcc
CFLAGS=-g -Wall -std=c11
all:echosrv2.o echocli.o basicfun.o
	gcc -o echosrv2 echosrv2.o basicfun.o
	gcc -o echocli echocli.o basicfun.o
echosrv2.o echocli.o: basicfun.h
.PHONY:clean
clean:
	$(RM) *.o echosrv2 echocli
